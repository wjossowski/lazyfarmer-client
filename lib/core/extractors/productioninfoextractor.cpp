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

#include "productioninfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonArray>

using namespace Core;
using namespace Core::Extractors;

void ProductionInfoExtractor::extractSpecificData()
{
    const QJsonArray in = m_datablock.value("in").toArray();
    const QJsonArray need = m_datablock.value("need").toArray();
    const QJsonArray out = m_datablock.value("out").toArray();

    // Check lenghts:
    if (in.size() != need.size() ||
        in.size() != out.size()) {
        return;
    }

    // Extract current production
    const QJsonValue currentProductionId = m_datablock.value("pid");
    if (currentProductionId.isString()) {
        m_data.insert("CurrentProduction", currentProductionId.toString().toInt());
    }

    QVariantList availableProductions;
    for (int i = 0, max = in.size(); i < max; i++) {

        int inputId = in.at(i).toInt();
        int inputNeeded = need.at(i).toInt();

        const QJsonObject availableOutput = out.at(i).toObject();
        if (availableOutput.isEmpty()){
            continue;
        }

        int outputId = availableOutput.value("1").toInt();
        int remaining = availableOutput.value("2").toInt();

        availableProductions.append(QVariantMap({
            { "In", inputId },
            { "Need", inputNeeded },
            { "Out", outputId },
            { "Remaining", remaining },
        }));
    }

    m_data.insert("ProductionInfo", availableProductions);
}
