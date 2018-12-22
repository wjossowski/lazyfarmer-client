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

#include "core/extractors/productioninfoextractor.h"
#include "core/extractors/feedinfoextractor.h"

#include <QtTest>

using namespace Core;
using namespace Core::Extractors;

class DatablockExtractorTest : public QObject
{
    Q_OBJECT

private slots:
    void productionInfoExtractorTest();

    void feedInfoExtractorTest();

private:
    void parseFromFile (const QString &filename, DatablockExtractor &extractor) const {
        QFile siteFile (QString("%1/assets/%2.json").arg(TEST_PWD).arg(filename));
        if (!siteFile.open(QIODevice::ReadOnly)) {
            QFAIL ("Unable to open farminfo_small.json file");
        }
        extractor.extract(std::move(siteFile.readAll()));
    }
};

void DatablockExtractorTest::productionInfoExtractorTest()
{
    ProductionInfoExtractor extractor;
    parseFromFile("productioninfo", extractor);

    const auto result = extractor.result();
    QVERIFY2(!result.isEmpty(),
             "ProductionInfoExtractor should not extract empty object");

    QVERIFY2(result["CurrentProduction"].toString() == "111",
            "Invalid CurrentProduction");

    QVariantList info = result["ProductionInfo"].toList();
    QVERIFY2(!info.isEmpty(),
             "Production info not exported");

    QVERIFY2(info.length() == 2,
             "Invalid info length");

    const QVariantMap expectedInfo1 = {
        { "In", "10" },
        { "Need", "12" },
        { "Out","27" },
        { "Remaining","120000" }
    };

    const QVariantMap expectedInfo2 = {
        { "In", "110" },
        { "Need", "5" },
        { "Out", "111" },
        { "Remaining", "59040" }
    };


    QVERIFY2(info.first().toMap() == expectedInfo1,
             "First Info doesnt match");

    QVERIFY2(info.last().toMap() == expectedInfo2,
             "Second info doesnt match");
}

void DatablockExtractorTest::feedInfoExtractorTest()
{
    FeedInfoExtractor extractor;
    parseFromFile("feedinfo", extractor);

    const auto result = extractor.result();

    QVERIFY2(!result.isEmpty(),
             "FeedInfoExtractor should not extract empty object");

    QVariantList info = result["FeedInputInfo"].toList();
    QVERIFY2(!info.isEmpty(),
             "Feed input info not exported");

    QVERIFY2(info.length() == 2,
             "Invalid info length");

    const QVariantMap expectedInfo1 = {
        { "In", "108" },
        { "Name", ""},
        { "Remaining", "1200" }
    };

    const QVariantMap expectedInfo2 = {
        { "In", "109" },
        { "Name", ""},
        { "Remaining","600" }
    };

    QVERIFY2(info.first().toMap() == expectedInfo1,
             "First Input Info doesnt match");

    QVERIFY2(info.last().toMap() == expectedInfo2,
             "Second Input info doesnt match");

    QVERIFY2(result["OutputProduct"].toInt() == 110,
             "Invalid Output Product");

    QVERIFY2(result["TotalTime"].toInt() == 129600,
            "Invalid Total Time");

    QVERIFY2(result["TimeLeft"].toInt() == -1,
            "Invalid Time Left");

    QVERIFY2(result["TimeToRefeed"].toInt() == 64800,
            "Invalid Time to Refeed");
}

QTEST_MAIN(DatablockExtractorTest)

#include "tst_datablockextractor.moc"
