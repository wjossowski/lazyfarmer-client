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

#include "core/helpers/gameinfoextractor.h"

#include <QtTest>

using namespace Core;
using namespace Core::Extractors;

class GameInfoExtractorTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void baseExtractorTest_data();
    void baseExtractorTest();

    void constantsExtractorTest_data();
    void constantsExtractorTest();

    void extractToFile();

private:

    QVariantMap constructBaseFilter (const QString &key) {
        return {
            { key, GameInfoExtractor::BaseFilters[key] }
        };
    }

    QVariantMap constructConstantsFilter (const QString &key) {
        return {
            { key, GameInfoExtractor::ConstantsFilters[key] }
        };
    }

private:
    QString m_baseContent;
    QString m_constantsContents;

};

void GameInfoExtractorTest::initTestCase()
{
    QHash<QString, QString*> data {
        { "content.html", &m_baseContent },
        { "jsconstants.js", &m_constantsContents },
    };

    QHashIterator<QString, QString*> dataIterator(data);
    while (dataIterator.hasNext()) {
        const auto dataItem = dataIterator.next();
        QString *dataMember = dataIterator.value();

        QFile siteFile (QString("%1/assets/%2").arg(TEST_PWD).arg(dataItem.key()));
        if (!siteFile.open(QIODevice::ReadOnly)) {
            QFAIL ("Unable to open file");
        }

        *dataMember = QString::fromUtf8(siteFile.readAll());
    }
}

void GameInfoExtractorTest::baseExtractorTest_data()
{
    QTest::addColumn<QString>("key");

    QTest::newRow("Products") << "products";
    QTest::newRow("Products Prices") << "products_prices";
    QTest::newRow("Forestry") << "forestry";
    QTest::newRow("Buildings") << "buildings";
}

void GameInfoExtractorTest::baseExtractorTest()
{
    QFETCH(QString, key);

    GameInfoExtractor extractor(constructBaseFilter(key));

    extractor.extract(m_baseContent);
    const auto &output = extractor.results();

    QVERIFY2 (!output.isEmpty(), "Product object shouldn't be empty");
    QVERIFY2 (output[key].isValid(), "Products object should be saved");
}

void GameInfoExtractorTest::constantsExtractorTest_data()
{
    QTest::addColumn<QString>("key");

    QTest::newRow("X Axis size") << "product_x";
    QTest::newRow("Y Axis size") << "product_y";
    QTest::newRow("Product time") << "product_time";
}

void GameInfoExtractorTest::constantsExtractorTest()
{
    QFETCH(QString, key);

    GameInfoExtractor extractor(constructConstantsFilter(key));

    extractor.extract(m_constantsContents);
    const auto &output = extractor.results();

    QVERIFY2 (!output.isEmpty(), "Product object shouldn't be empty");
    QVERIFY2 (output[key].isValid(), "Products object should be saved");
}

void GameInfoExtractorTest::extractToFile()
{
    const auto baseExtractor = GameInfoExtractor::baseExtractor();
    baseExtractor->extract(m_baseContent);

    const auto constantsExtractor = GameInfoExtractor::constantsExtractor();
    constantsExtractor->extract(m_constantsContents);

    QFile file(QString("%1/labels.json").arg(TEST_OUT_PWD));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(QJsonObject({
            { "base", QJsonValue::fromVariant(baseExtractor->results()) },
            { "constants", QJsonValue::fromVariant(constantsExtractor->results())}
        })).toJson(QJsonDocument::Indented));
    } else {
        QFAIL("File should be opened");
    }
}


QTEST_MAIN(GameInfoExtractorTest)

#include "tst_gameinfoextractor.moc"
