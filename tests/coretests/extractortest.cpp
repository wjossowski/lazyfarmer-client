#include "extractortest.h"

#include "extractor.h"

#include <QtTest/QTest>
#include <QtCore/QRegularExpression>

void ExtractorTest::initTestCase()
{
    QFile siteFile (QString("%1/assets/content.html").arg(TEST_PWD));
    if (!siteFile.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open content.html file");
    }
    m_siteContent = QString::fromUtf8(siteFile.readAll());
}

void ExtractorTest::extractProducts()
{
    Extractor extractor(m_siteContent, { { "products", "var produkt_name = (?<products>.*);" } });
    const auto &output = extractor.results();
    QVERIFY2 (!output.isEmpty(), "Product object shouldn't be empty");
    QVERIFY2 (output["products"].isObject(), "Products object should be saved");
}

void ExtractorTest::extractForestry()
{
    Extractor extractor(m_siteContent, { { "forestry", "var produkt_name_forestry = (?<forestry>.*);" } });
    const auto &output = extractor.results();
    QVERIFY2 (!output.isEmpty(), "Forestry object shouldn't be empty");
    QVERIFY2 (output["forestry"].isObject(), "Forestry object should be saved");
}

void ExtractorTest::extractBuildings()
{
    Extractor extractor(m_siteContent, { { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" } });
    const auto &output = extractor.results();
    QVERIFY2 (!output.isEmpty(), "Buildings object shouldn't be empty");
    QVERIFY2 (output["buildings"].isObject(), "Buildings object should be saved");
}

void ExtractorTest::cleanupTestCase()
{
    Extractor extractor(m_siteContent);
    QFile file(QString("%1/labels.json").arg(TEST_OUT_PWD));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(extractor.results()).toJson(QJsonDocument::Indented));
    } else {
        QFAIL("File should be opened");
    }
}
