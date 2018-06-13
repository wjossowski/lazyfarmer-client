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

FieldInfoExtractor::FieldInfoExtractor(quint64 timestamp)
    : DatablockExtractor()
{
    m_timestamp = timestamp == 0 ? QDateTime::currentMSecsSinceEpoch()/1000
                                 : timestamp;
}

void FieldInfoExtractor::extractSpecificData()
{
    QVariantList fieldsInfo;

    for (const auto &field : m_datablock) {
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
        int remaining = storedRemaining - ((storedRemaining == 0) ? 0 : m_timestamp);

        // Build FieldInfo object
        fieldsInfo.append(QVariantMap({
            { "Id", info["inhalt"].toString() },
            { "FieldId", info["teil_nr"].toString() },
            { "Remaining", QString::number(qMax(-1, remaining)) },
            { "IsWater", QString::number(info["iswater"].toBool()) }
        }));

    }

    m_data.insert("FieldsInfo", fieldsInfo);
}
