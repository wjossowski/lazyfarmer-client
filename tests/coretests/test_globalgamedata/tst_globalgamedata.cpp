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

#include "core/globalgamedata.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtTest>

using namespace Core;

class GlobalGameDataTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

//    void
};

void GlobalGameDataTest::initTestCase()
{
    QFile fieldsJson (QString("%1/assets/globalgamedata.json").arg(TEST_PWD));
    if (!fieldsJson.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open globalgamedata.json file");
    }

    GlobalGameData::registerGameData("domain", QJsonDocument::fromJson(fieldsJson.readAll()).toVariant());
}


QTEST_APPLESS_MAIN(GlobalGameDataTest)

#include "tst_globalgamedata.moc"
