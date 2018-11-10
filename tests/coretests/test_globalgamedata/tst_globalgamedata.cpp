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
    void productDataTest();
    void buildingDataTest();
    void forestryDataTest();
    void emptyDataTest();

private:
    QVariant m_data;
};

void GlobalGameDataTest::initTestCase()
{
    QFile fieldsJson (QString("%1/assets/globalgamedata.json").arg(TEST_PWD));
    if (!fieldsJson.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open globalgamedata.json file");
    }

    m_data = QJsonDocument::fromJson(fieldsJson.readAll()).toVariant();
    GlobalGameData::registerGameData("domain", m_data);
}

void GlobalGameDataTest::productDataTest()
{
    const auto domainData = GlobalGameData::gameData("domain");
    const auto productInfo = domainData->productInfo(2);

    QVERIFY(productInfo.name == "Kukurydza");
    QVERIFY(productInfo.size == 4);
    QVERIFY(productInfo.price == 110);
    QVERIFY(productInfo.time == 2700);
}

void GlobalGameDataTest::buildingDataTest()
{
    const auto domainData = GlobalGameData::gameData("domain");
    const auto buildingInfo = domainData->buildingInfo(2);

    QVERIFY(buildingInfo.name == "Kurnik");
}

void GlobalGameDataTest::forestryDataTest()
{
    const auto domainData = GlobalGameData::gameData("domain");
    const auto forestryInfo = domainData->forestryInfo(2);

    QVERIFY(forestryInfo.name == "Brzoza");
}

void GlobalGameDataTest::emptyDataTest()
{
    const auto undefinedData = GlobalGameData::gameData("nothinsk");

    QVERIFY(undefinedData->productInfo(2).name == "Unknown");
    QVERIFY(undefinedData->buildingInfo(11).name == "Unknown");
    QVERIFY(undefinedData->forestryInfo(21).name == "Unknown");
}

QTEST_APPLESS_MAIN(GlobalGameDataTest)

#include "tst_globalgamedata.moc"
