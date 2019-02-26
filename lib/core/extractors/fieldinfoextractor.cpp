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

#include "fieldinfoextractor.h"

#include <QtCore/QDateTime>

#include <QDebug>

using namespace Core;
using namespace Core::Extractors;
using namespace Core::Data;

FieldInfoExtractor::FieldInfoExtractor(qint64 timestamp, GlobalGameData *data)
    : DatablockExtractor()
    , m_gamedata(data)
{
    m_timestamp = timestamp == 0 ? QDateTime::currentMSecsSinceEpoch()/1000
                                 : timestamp;
}

QVariantList FieldInfoExtractor::filterFields(const QVariantList &fieldsInfo) const
{
    QMap<int, QVariant> placeholdList;
    std::for_each(fieldsInfo.cbegin(), fieldsInfo.cend(), [&placeholdList](const QVariant& fieldInfo) {
        int fieldNo = fieldInfo.toMap()["FieldId"].toInt();
        placeholdList.insert(fieldNo, fieldInfo);
    });

    QMutableMapIterator<int, QVariant> iter (placeholdList);
    QList<int> fieldsToRemove;
    while(iter.hasNext()) {
        const auto value = iter.next();
        auto fieldInfo = value->toMap();
        int fieldId = fieldInfo["FieldId"].toInt();
        int plantId = fieldInfo["Id"].toInt();

        if (fieldsToRemove.contains(fieldId)) {
            fieldsToRemove.removeOne(fieldId);
            fieldInfo["Id"] = -1;
            iter.setValue(fieldInfo);
        } else {
            fieldsToRemove << ProductDetails::neighbours(fieldId, m_gamedata->productSize(plantId));
        }

    }

    return placeholdList.values();
}

void FieldInfoExtractor::extractSpecificData()
{
    QVariantList fieldsInfo;

    for (const auto field : m_datablock) {
        // Field should be an object
        if (!field.isObject()) {
            break;
        }

        // Field info should contain "inhalt" field
        const auto info = field.toObject();
        if (!info.contains("inhalt")) {
            return;
        }

        // Calculate remaining time
        int storedRemaining = info["zeit"].toString().toInt();
        qint64 remaining = storedRemaining - ((storedRemaining == 0) ? 0 : m_timestamp);

        bool isWater = info["iswater"].toBool();
        int storedWateredTime = info["wasser"].toString().toInt();
        bool hasWateredRecord = storedWateredTime > 0 && isWater;

        // Build FieldInfo object
        fieldsInfo.append(QVariantMap({
            { "Id", info["inhalt"].toString().toInt() },
            { "FieldId", info["teil_nr"].toString().toInt() },
            { "Remaining", QString::number(qMax(qint64(-1), remaining)).toInt() },
            { "IsWatered", QString::number(hasWateredRecord).toInt() }
        }));

    }

    m_data.insert("FieldsInfo", m_gamedata ? filterFields(fieldsInfo) : fieldsInfo);
}
