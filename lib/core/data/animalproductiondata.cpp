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

#include "animalproductiondata.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>

using namespace Core;
using namespace Core::Data;

AnimalProductionData::AnimalProductionData(Player *parent)
    : BuildingData (parent)
{

}

void AnimalProductionData::foo()
{
    qDebug() << "Fooooooo kurwy jebane";
}

void AnimalProductionData::update(const QVariant &info)
{
    IPlayerData::update(info);
}

QString AnimalProductionData::toString() const
{
    return QString ("FooBarAnimalProduction");
}

QVariant AnimalProductionData::toVariant()
{
    return QVariant::fromValue<AnimalProductionData*>(this);
}
