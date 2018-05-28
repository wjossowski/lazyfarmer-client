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

#ifdef DEBUG_MODE
#include "core/api/apigateway.h"
#include "core/api/messages.h"
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

#include <QtDebug>

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

QFile debugFile;
QMutex debugMutex;
QTextStream debugStream;

void handleMessage(QtMsgType type, const QMessageLogContext &context,
                   const QString &buf)
{
    QMutexLocker locker(&debugMutex);
    const auto message = qFormatLogMessage(type, context, buf);

    auto out = (type == QtInfoMsg || type == QtDebugMsg) ? stdout : stderr;
    fprintf(out, "\n%s", message.toLocal8Bit().constData());
    fflush(out);

    if (debugStream.status() == QTextStream::Ok) {
        debugStream << message << flush;
    }
}

#ifdef DEBUG_MODE
void createDebugEnvironment(ApiGateway &gateway, const QCommandLineParser &parser)
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
#endif


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
    parser.setApplicationDescription(QGuiApplication::translate("main", "Lazy farmer - My Free Farm bot"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        { { "l", "login" },
          QGuiApplication::translate("main", "Specifies login on startup."),
          QGuiApplication::translate("main", "User's login.") },
        { { "p", "password" },
          QGuiApplication::translate("main", "Specifies password on startup."),
          QGuiApplication::translate("main", "User's password.") },
        { { "d", "domain" },
          QGuiApplication::translate("main", "Specifies game's domain on startup."),
          QGuiApplication::translate("main", "Domain (myfreefarm.de / wolnifarmerzy.pl).") },
        { { "s", "server" },
          QGuiApplication::translate("main", "Specifies server number on startup."),
          QGuiApplication::translate("main", "Server number.") },
        { { "c", "config" },
          QGuiApplication::translate("main", "Specifies config file on startup."),
          QGuiApplication::translate("main", "Task manager's configuration file.") },
        { { "n", "no-gui" },
          QGuiApplication::translate("main", "Disables UI Mode.") }
    });
    // Execute CLI Parser
    parser.process(lazyFarmerApp);

    try {
        // Inilialize cache directory
        QDir applicationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (!applicationDir.mkpath("."))
            throw std::ios_base::failure(QGuiApplication::translate("main", "Unable to create path to application data.").toStdString());

        // Initalize log directory
        const QString fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd-HH_mm.log");
        debugFile.setFileName(applicationDir.absoluteFilePath(fileName));

        // Create debug prompt device
        debugStream.setDevice(&debugFile);
        if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
            throw std::ios_base::failure(QGuiApplication::translate("main", "Unable to open logging file.").toStdString());

        // Initialize message handler
        qSetMessagePattern("[%{time HH:mm:ss.zzz}] %{type}: %{message} (%{function}, %{file}, %{line})");
        qInstallMessageHandler(handleMessage);

    } catch (std::exception &e) {
        qCritical() << e.what();
    }

#ifdef DEBUG_MODE
    qDebug() << "Debug storage located in:" << QFileInfo(debugFile).absoluteFilePath();

    ApiGateway debugGateway;
    createDebugEnvironment(debugGateway, parser);
    debugGateway.queueMessage(ApiMessage::create<LoginMessage>(&debugGateway));
//    debugGateway.queueMessage(ApiMessage::create<GetFarmInfoMessage>(&debugGateway));
    debugGateway.queueMessage(ApiMessage::create<SetPlant>(&debugGateway));
    debugGateway.queueMessage(ApiMessage::create<SetPour>(&debugGateway));

    debugGateway.start();

    QTimer::singleShot(5000, [&](){
       debugGateway.start();

       QTimer::singleShot(1000, [&](){
          debugGateway.start();
       });
    });


#else
    QSharedPointer<QQmlApplicationEngine> engine;
    if (!parser.isSet("no-gui")) {
        engine.reset(new QQmlApplicationEngine());
        engine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));
        if (engine->rootObjects().isEmpty())
            return -1;
    }
#endif

    return lazyFarmerApp.exec();
}
