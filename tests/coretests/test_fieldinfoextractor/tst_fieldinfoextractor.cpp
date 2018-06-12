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

#include "core/extractors/fieldinfoextractor.h"

#include <QtTest>

using namespace Core;
using namespace Core::Extractors;

class FieldInfoExtractorTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void extractorTest();

private:
    QString m_siteContent;

};

void FieldInfoExtractorTest::initTestCase()
{
    QFile fieldsJson (QString("%1/assets/fields.json").arg(TEST_PWD));
    if (!fieldsJson.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open fields.json file");
    }

    m_siteContent = QString::fromUtf8(fieldsJson.readAll());
}

void FieldInfoExtractorTest::extractorTest()
{
    // Initialize with proper timestamp
    FieldInfoExtractor extractor(1528067870);
    extractor.extract(m_siteContent.toLatin1());

    const auto result = extractor.result();

    QVERIFY2(!result.isEmpty(), "Should extract data");

    const auto fieldsInfo = result["FieldsInfo"].toList();
    QVERIFY(!fieldsInfo.isEmpty());
    QVERIFY(fieldsInfo.size() == 120);

    for (const auto &info : fieldsInfo) {
        const auto infoData = info.toMap();
        int id = infoData["Id"].toString().toInt();
        int fieldId = infoData["FieldId"].toString().toInt();
        int remaining = infoData["Remaining"].toString().toInt();
        int isWater = infoData["IsWater"].toString().toInt();

        QVERIFY2(id != 0, "Id should be correct");
        QVERIFY2(fieldId > 0 && fieldId <= 120, "FieldId has to be correct");
        QVERIFY2(remaining >= -1, "Remaining time has to be correct");
        QVERIFY2(isWater == 1 || isWater == 0, "IsWater has to be correct");
    }
}

QTEST_APPLESS_MAIN(FieldInfoExtractorTest)

#include "tst_fieldinfoextractor.moc"
