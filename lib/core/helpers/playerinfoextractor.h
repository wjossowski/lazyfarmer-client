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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

class QJsonObject;

namespace Extractors {

    class PlayerInfoExtractor
    {
    public:
        bool parseInfo(const QByteArray &info);

        const QString &errorString() const { return m_error; }

        const QVariantMap &basicInfo() const { return m_basicInfo; }
        const QVariantList &storageInfo() const { return m_storageInfo; }
        const QVariantList &farmsInfo() const { return m_farmsInfo; }

    private:
        void parseBasicInfo(const QJsonObject &basicInfo);
        void parseStorageInfo(const QJsonObject &storageInfo);
        void parseFarmsInfo(const QJsonObject &farmsInfo);

    private:
        QVariantMap m_basicInfo;
        QVariantList m_storageInfo;
        QVariantList m_farmsInfo;

        QString m_error;
    };

}

