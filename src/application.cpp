/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2018 Wojciech Ossowski <w.j.ossowski@gmail.com>.
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

#include "application.h"

#include "core/configreader.h"

#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QTimer>

#include <QtDebug>

using namespace Core;
using namespace Model;

Application::Application(int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    setAttribute(Qt::AA_EnableHighDpiScaling);

    setApplicationVersion(CURRENT_VERSION);
    setApplicationName(APPLICATION_NAME);
    setOrganizationName(COMPANY_NAME);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    QTimer::singleShot(1000, [&] () {
        emit pushToStack("CH U J", QVariantMap { {"Foo", "Bar"} });
    });
}

void Application::initializeCommandLineInterface(QCommandLineParser &parser)
{
    parser.setApplicationDescription(tr("Lazy farmer - My Free Farm bot"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        { { "l", "login" },
          tr("Specifies login on startup."),
          tr("User's login.") },
        { { "p", "password" },
          tr("Specifies password on startup."),
          tr("User's password.") },
        { { "d", "domain" },
          tr("Specifies game's domain on startup."),
          tr("Domain (myfreefarm.de / wolnifarmerzy.pl).") },
        { { "s", "server" },
          tr("Specifies server number on startup."),
          tr("Server number.") },
        { { "c", "config" },
          tr("Specifies config file on startup."),
          tr("Task manager's configuration file.") },
        { { "n", "no-gui" },
          tr("Disables UI Mode.") }
    });

    parser.process(*this);
}

void Application::initializeStaticGameData()
{
#ifdef DEBUG_MODE
    QDir assetsDirectory(ASSETS_DIRECTORY);
#else
    QDir assetsDirectory(qApp->applicationDirPath());
#endif
    QFile buildingConfig(assetsDirectory.absoluteFilePath("config.json"));
    if (buildingConfig.open(QIODevice::ReadOnly)) {
        const auto json = buildingConfig.readAll();
        if (!ConfigReader::instance().loadConfig(json)) {
            throw std::ios_base::failure(tr("Unable to read building-config.json").toStdString());
        }
    } else {
        throw std::ios_base::failure(tr("Unable to load building-config.json").toStdString());
    }
}

void Application::showOverviewPage(const QVariant &playerVariant)
{
    if (!playerVariant.isValid()) {
        return;
    }

    Core::Player *p = playerVariant.value<Core::Player*>();
    QVariant value = QVariant::fromValue(p->buildingModel().data());

    emit pushToStack("qrc:/qml/Views/FarmView.qml", value);
}
