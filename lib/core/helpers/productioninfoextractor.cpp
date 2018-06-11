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

using namespace Extractors;

void ProductionInfoExtractor::extract(const QByteArray &content)
{
    if (content.isEmpty()){
        return;
    }

    // Create JSON document
    const auto document = QJsonDocument::fromJson(content);
    const auto data = document.object();

    if (data.isEmpty()) {
        return;
    }

    if (!data.contains("datablock")) {
        return;
    }

    // Get result
    const QJsonArray datablock = data.value("datablock").toArray();

    // Check if result is correct;
    if (datablock.size() < 2 ||
        datablock.first().toInt() != 1) {
        return;
    }

    // Get Production Info Object
    const auto productionJsonData = datablock.at(1).toObject();
    if (productionJsonData.isEmpty()) {
        return;
    }

    const QJsonArray in = productionJsonData.value("in").toArray();
    const QJsonArray need = productionJsonData.value("need").toArray();
    const QJsonArray out = productionJsonData.value("out").toArray();

    // Check lenghts:
    if (in.size() != need.size() ||
        in.size() != out.size()) {
        return;
    }

    // Extract current production
    const QJsonValue currentProductionId = productionJsonData.value("pid");
    if (currentProductionId.isString()) {
        m_productionData.insert("CurrentProduction", currentProductionId.toString());
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
            { "In", QString::number(inputId) },
            { "Need", QString::number(inputNeeded) },
            { "Out", QString::number(outputId) },
            { "Remaining", QString::number(remaining) },
        }));
    }

    m_productionData.insert("ProductionInfo", availableProductions);

}
