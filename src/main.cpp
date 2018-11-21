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

#include "core/globalgamedata.h"
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

#include <QtGui/QGuiApplication>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

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

#ifdef DEBUG_MODE
void createDebugEnvironment(Api::ApiGateway &gateway, const QCommandLineParser &parser)
{
    qDebug() << "Entering" << APPLICATION_NAME << " v." << CURRENT_VERSION << "debug mode";
    qDebug() << "Selected following configuration:";
    qDebug() << "Domain:  " << parser.value("domain");
    qDebug() << "Server:  " << parser.value("server");
    qDebug() << "Login:   " << parser.value("login");
    qDebug() << "Password:" << parser.value("password");

    gateway.setApiOptions({
        parser.value("server"),
        parser.value("domain"),
        parser.value("login"),
        parser.value("password")
    });

    qDebug() << "Account" << (gateway.isConfigured() ? "configured!" : "unconfigured ;(");
}

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

void initializeCommandLineInterface(const QCoreApplication &application, QCommandLineParser &parser)
{
    parser.setApplicationDescription(qApp->translate("main", "Lazy farmer - My Free Farm bot"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        { { "l", "login" },
          qApp->translate("main", "Specifies login on startup."),
          qApp->translate("main", "User's login.") },
        { { "p", "password" },
          qApp->translate("main", "Specifies password on startup."),
          qApp->translate("main", "User's password.") },
        { { "d", "domain" },
          qApp->translate("main", "Specifies game's domain on startup."),
          qApp->translate("main", "Domain (myfreefarm.de / wolnifarmerzy.pl).") },
        { { "s", "server" },
          qApp->translate("main", "Specifies server number on startup."),
          qApp->translate("main", "Server number.") },
        { { "c", "config" },
          qApp->translate("main", "Specifies config file on startup."),
          qApp->translate("main", "Task manager's configuration file.") },
        { { "n", "no-gui" },
          qApp->translate("main", "Disables UI Mode.") }
    });
    // Execute CLI Parser
    parser.process(application);
}

void initializeLoggingEnvironment()
{
    // Inilialize cache directory
    QDir applicationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!applicationDir.mkpath("."))
        throw std::ios_base::failure(qApp->translate("main", "Unable to create path to application data.").toStdString());

    // Initalize log directory
    const QString fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd-HH_mm.log");
    debugFile.setFileName(applicationDir.absoluteFilePath(fileName));

    // Create debug prompt device
    debugStream.setDevice(&debugFile);
    if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
        throw std::ios_base::failure(qApp->translate("main", "Unable to open logging file.").toStdString());

    // Initialize message handler
    qSetMessagePattern("[%{time HH:mm:ss.zzz}] %{type}: %{message} (%{function}, %{file}, %{line})");
    qInstallMessageHandler(handleMessage);

    qDebug() << debugFile.fileName();

}

void initializeStaticGameData()
{
#ifdef DEBUG_MODE
    QDir assetsDirectory(ASSETS_DIRECTORY);
#else
    QDir assetsDirectory(qApp->applicationDirPath());
#endif
    QFile buildingConfig(assetsDirectory.absoluteFilePath("building-config.json"));
    if (buildingConfig.open(QIODevice::ReadOnly)) {
        if (!GlobalGameData::loadBuildingTypes(buildingConfig.readAll())) {
            throw std::ios_base::failure(qApp->translate("main", "Unable to read building-config.json").toStdString());
        }
    } else {
        throw std::ios_base::failure(qApp->translate("main", "Unable to load building-config.json").toStdString());
    }
}

void registerCustomMetatypes()
{
    qRegisterMetaType<Core::Data::BuildingType>("Core::Data::BuildingType");
    qRegisterMetaType<Core::Data::BuildingDetails>("Core::Data::BuildingDetails");
    qRegisterMetaType<Core::Data::ProductDetails>("Core::Data::ProductDetails");
    qRegisterMetaType<Core::Data::ProductionDetails>("Core::Data::ProductionDetails");
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

    // Initialize Application
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication lazyFarmerApp(argc, argv);
    lazyFarmerApp.setApplicationVersion(CURRENT_VERSION);
    lazyFarmerApp.setApplicationName(APPLICATION_NAME);
    lazyFarmerApp.setOrganizationName(COMPANY_NAME);

    // Set default configuration format
    QSettings::setDefaultFormat(QSettings::IniFormat);

    // Initialize command-line parser
    QCommandLineParser parser;
    initializeCommandLineInterface(lazyFarmerApp, parser);

    try {
        initializeLoggingEnvironment();
        initializeStaticGameData();
        registerCustomMetatypes();
    } catch (std::exception &e) {
        qCritical() << e.what();
        return -1;
    }

#ifdef DEBUG_MODE
    qDebug() << "Debug storage located in:" << QFileInfo(debugFile).absoluteFilePath();

//    createDebugEnvironment(debugGateway, parser);
//    queryDebug(debugGateway);
#endif

    if (parser.isSet("no-gui")) {
        return lazyFarmerApp.exec();
    }

    QQmlApplicationEngine engine;

    Model::PlayerFactoryModel playerFactory;
    engine.rootContext()->setContextProperty("PlayerFactoryModel", &playerFactory);
    auto player = playerFactory.create();

    QTimer::singleShot(5000, [&] () {
       createDebugEnvironment(player->gateway(), parser);
       queryDebug(player->gateway());
    });

//    Model::StorageModel storageModel(p.storage());
//    engine.rootContext()->setContextProperty("StorageModel", &storageModel);

//    Model::BuildingModel buildingModel(p.buildings());
//    engine.rootContext()->setContextProperty("BuildingModel", &buildingModel);

    Translator translator;
    engine.rootContext()->setContextProperty("t", &translator);

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()){
        return -1;
    }

    return lazyFarmerApp.exec();
}
