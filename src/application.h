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

#pragma once

#include "core/configreader.h"
#include "core/playerfactorymodel.h"

#include <QtGui/QGuiApplication>
#include <QtCore/QCommandLineParser>

class Application : public QGuiApplication
{
    Q_OBJECT

public:
    enum class Screens {
        AccountsViewScreen = 1,
        FarmOverviewScreen,
        FieldScreen,
        AnimalsProductionScreen,
        ResourceProductionScreen,

        TaskQueueScreen,

        Unknown
    };
    Q_ENUM(Screens)

    explicit Application(int &argc, char **argv);
    ~Application() override = default;

    void initializeCommandLineInterface(QCommandLineParser &parser);
    void initializeStaticGameData();

    Core::ConfigReader &reader() { return Core::ConfigReader::instance(); }
    Model::PlayerFactoryModel &playerFactory() { return m_playerFactory; }

    Q_INVOKABLE void requestOverviewScreen(int playerId);
    Q_INVOKABLE void requestBuildingInfoScreen(Model::BuildingModel *buildingModel, int buildingId);

signals:
    void pushToStack(const QString &qml, const QVariant &data) const;

private:
    void showScreen(Screens screenToShow, const QVariant &data) const;

private:
    Model::PlayerFactoryModel m_playerFactory;

};
