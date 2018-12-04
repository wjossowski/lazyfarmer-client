/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "providers/resourceimageprovider.h"
#include "application.h"
#include "translator.h"

#ifdef DEBUG_MODE
#include "model/storagemodel.h"
#include "model/buildingmodel.h"
#include "model/playerfactorymodel.h"
#include "core/api/apigateway.h"
#include "core/api/messages/messages.h"
#include "core/player.h"
#include <QTimer>
#endif

#include <ios>

#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QMutex>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QCommandLineParser>
#include <QtCore/QTranslator>

#include <QtGui/QGuiApplication>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtQuickControls2/QQuickStyle>

#include <QtDebug>

#ifdef DEBUG_MODE
using namespace Core;
using namespace Core::Api;
using namespace Core::Api::Messages;
#endif

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

static QFile debugFile;
static QMutex debugMutex;
static QTextStream debugStream;

void handleMessage(QtMsgType type,
                   const QMessageLogContext &context,
                   const QString &buf)
{
#ifndef DEBUG_MODE
    if (type == QtDebugMsg){
        return;
    }
#endif

    QMutexLocker locker(&debugMutex);
    const auto message = qFormatLogMessage(type, context, buf);

    auto out = (type == QtInfoMsg || type == QtDebugMsg) ? stdout : stderr;
    fprintf(out, "\n%s", message.toLocal8Bit().constData());
    fflush(out);

    if (debugStream.status() == QTextStream::Ok) {
        debugStream << '\n' << message << flush;
    }
}

Api::ApiOptions extractApiOptions(const QCommandLineParser &parser)
{
    qDebug() << "Selected following configuration:";
    qDebug() << "Domain:  " << parser.value("domain");
    qDebug() << "Server:  " << parser.value("server");
    qDebug() << "Login:   " << parser.value("login");
    qDebug() << "Password:" << parser.value("password");

    return {
        parser.value("server"),
        parser.value("domain"),
        parser.value("login"),
        parser.value("password")
    };
}

#ifdef DEBUG_MODE

void queryDebug(Api::ApiGateway &debugGateway)
{
    const auto getInfo = [&](){
        QVector<int> fields {};
        debugGateway.queueMessage(GetFarmInfo::Ptr(new GetFarmInfo(&debugGateway)));

        for (int i = 0; i < 120; i++) {
            const Data::BuildingDetails buildingDetails = {1, 1};
            const Data::ProductDetails produceDetails = {17, 1, i};
            debugGateway.queueMessage(GetCollect::Ptr(new GetCollect(&debugGateway, buildingDetails, produceDetails)));
            debugGateway.queueMessage(SetPlant::Ptr(new SetPlant(&debugGateway, buildingDetails, produceDetails)));
            debugGateway.queueMessage(SetPour::Ptr(new SetPour(&debugGateway, buildingDetails, produceDetails)));
        }

        debugGateway.start();
    };

    getInfo();

}

#endif

void initializeCacheEnvironment()
{
    // Inilialize cache directory
    QDir applicationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!applicationDir.mkpath("./logs") && !applicationDir.cd("./logs"))
        throw std::ios_base::failure(qApp->translate("main", "Unable to create path to application data.").toStdString());

    // Initalize log directory
    const QString fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd-HH_mm.log");
    debugFile.setFileName(applicationDir.absoluteFilePath(QString("logs/%1").arg(fileName)));

    // Create debug prompt device
    debugStream.setDevice(&debugFile);
    if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
        throw std::ios_base::failure(qApp->translate("main", "Unable to open logging file.").toStdString());

    // Initialize message handler
    qSetMessagePattern("[%{time HH:mm:ss.zzz}] %{type}: %{message} (%{function}, %{file}, %{line})");
    qInstallMessageHandler(handleMessage);
}

void registerCustomMetatypes()
{
    qRegisterMetaType<Core::Data::BuildingType>("Core::Data::BuildingType");
    qRegisterMetaType<Core::Data::BuildingDetails>("Core::Data::BuildingDetails");
    qRegisterMetaType<Core::Data::ProductDetails>("Core::Data::ProductDetails");
    qRegisterMetaType<Core::Data::ProductionDetails>("Core::Data::ProductionDetails");
    qRegisterMetaType<Core::Api::ApiOptions>("Core::Api::ApiOptions");
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // Windows console handler
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONOUT$", "r", stdin );
        qInfo() << ""; // add Empty line
    }
#endif

    // Initialize Applicatio
    Application lazyFarmerApp(argc, argv);

    // Initialize command-line parser
    QCommandLineParser parser;

    try {
        lazyFarmerApp.initializeCommandLineInterface(parser);
        lazyFarmerApp.initializeStaticGameData();

        initializeCacheEnvironment();
        registerCustomMetatypes();
    } catch (std::exception &e) {
        qCritical() << e.what();
        qInfo() << parser.helpText();
        return -1;
    }

    if (parser.isSet("no-gui")) {
        return lazyFarmerApp.exec();
    } else {
        QQuickStyle::setStyle("Material");
    }

    Model::PlayerFactoryModel playerFactory;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("LazyFarmer", &lazyFarmerApp);
    engine.rootContext()->setContextProperty("PlayerFactoryModel", &lazyFarmerApp.playerFactory());
    engine.rootContext()->setContextProperty("AvailableDomains", lazyFarmerApp.reader().availableDomains());
    engine.addImageProvider("resources", new ResourceImageProvider);

    Translator t(&lazyFarmerApp);
    engine.rootContext()->setContextProperty("t", &t);

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()){
        return -1;
    }

    return lazyFarmerApp.exec();
}
