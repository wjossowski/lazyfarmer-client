#include "mainwindow.h"

#include "logindialog.h"
#include "core/apigateway.h"

#include <ios>

#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QMutex>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QCommandLineParser>

#include <QtWidgets/QApplication>

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
    QApplication lazyFarmerApp(argc, argv);
    lazyFarmerApp.setApplicationVersion(CURRENT_VERSION);
    lazyFarmerApp.setApplicationName(APPLICATION_NAME);
    lazyFarmerApp.setOrganizationName(COMPANY_NAME);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::translate("main", "Lazy farmer - My Free Farm bot"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        { { "l", "login" },
          QApplication::translate("main", "Specifies login on startup."),
          QApplication::translate("main", "User's login.") },
        { { "p", "password" },
          QApplication::translate("main", "Specifies password on startup."),
          QApplication::translate("main", "User's password.") },
        { { "d", "domain" },
          QApplication::translate("main", "Specifies game's domain on startup."),
          QApplication::translate("main", "Domain (myfreefarm.de / wolnifarmerzy.pl).") },
        { { "s", "server" },
          QApplication::translate("main", "Specifies server number on startup."),
          QApplication::translate("main", "Server number.") },
        { { "c", "config" },
          QApplication::translate("main", "Specifies config file on startup."),
          QApplication::translate("main", "Task manager's configuration file.") },
        { { "n", "no-gui" },
          QApplication::translate("main", "Disables UI Mode.") }
    });
    parser.process(lazyFarmerApp);

    try {
        QDir applicationDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (!applicationDir.mkpath("."))
            throw std::ios_base::failure(QApplication::translate("main", "Unable to create path to application data.").toStdString());

        const QString fileName = QDateTime::currentDateTime().toString("yyyy_MM_dd-HH_mm.log");
        debugFile.setFileName(applicationDir.absoluteFilePath(fileName));

        debugStream.setDevice(&debugFile);
        if (!debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
            throw std::ios_base::failure(QApplication::translate("main", "Unable to open logging file.").toStdString());

        qSetMessagePattern("[%{time HH:mm:ss.zzz}] %{type}: %{message} (%{function}, %{file}, %{line})");
        qInstallMessageHandler(handleMessage);
    } catch (std::exception &e) {
        qCritical() << e.what();
    }

    QScopedPointer<MainWindow> mainWindow;
    if (!parser.isSet("no-gui")) {
        mainWindow.reset(new MainWindow);
        mainWindow->show();
    }

    return lazyFarmerApp.exec();
}
