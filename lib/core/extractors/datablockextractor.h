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

#include <QtCore/QVariant>
#include <QtCore/QJsonObject>

namespace Core {

    namespace Extractors {

        class DatablockExtractor
        {

        public:
            explicit DatablockExtractor() = default;

            const QVariantMap &result() { return m_data; }
            void extract(const QByteArray &content);

        private:
            bool fetchDatablock(const QJsonDocument &document);
            virtual void extractSpecificData() = 0;

        protected:
            QVariantMap m_data;
            QJsonObject m_datablock;
        };

    }

}
