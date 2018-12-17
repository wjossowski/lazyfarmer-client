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

#include "resourceimageprovider.h"

#include "core/configreader.h"

using namespace Core;

ResourceImageProvider::ResourceImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap ResourceImageProvider::requestPixmap(const QString &id,
                                             QSize *size,
                                             const QSize &requestedSize)
{
    const QStringList path = id.split('/');

    const QString resourceType = path[0];
    const int resourceId = path[1].toInt();

    QPixmap resource = ConfigReader::instance()
            .pixmapAt(resourceType, resourceId);

    if (requestedSize.isValid()) {
        resource = resource.scaled(requestedSize, Qt::KeepAspectRatio);
    }

    *size = QSize(resource.width(), resource.height());

    return resource;
}
