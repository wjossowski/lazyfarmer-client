#include <QtTest>

#include "core/helpers/gameinfoextractor.h"

class GameInfoExtractorTest : public QObject
{
    Q_OBJECT

public:
    GameInfoExtractorTest () {}

private slots:
    void initTestCase();

    void extractProducts();
    void extractForestry();
    void extractBuildings();

    void extractToFile();

private:
    QString m_siteContent;
};

void GameInfoExtractorTest::initTestCase()
{
    QFile siteFile (QString("%1/assets/content.html").arg(TEST_PWD));
    if (!siteFile.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open content.html file");
    }
    m_siteContent = QString::fromUtf8(siteFile.readAll());
}

void GameInfoExtractorTest::extractProducts()
{
    GameInfoExtractor extractor({ { "products", "var produkt_name = (?<products>.*);" } });

    QVERIFY2 (extractor.extract(m_siteContent), "GameInfoExtractor should extract");
    const auto &output = extractor.results();

    QVERIFY2 (!output.isEmpty(), "Product object shouldn't be empty");
    QVERIFY2 (output["products"].isObject(), "Products object should be saved");
}

void GameInfoExtractorTest::extractForestry()
{
    GameInfoExtractor extractor({ { "forestry", "var produkt_name_forestry = (?<forestry>.*);" } });

    QVERIFY2 (extractor.extract(m_siteContent), "GameInfoExtractor should extract");
    const auto &output = extractor.results();

    QVERIFY2 (!output.isEmpty(), "Forestry object shouldn't be empty");
    QVERIFY2 (output["forestry"].isObject(), "Forestry object should be saved");
}

void GameInfoExtractorTest::extractBuildings()
{
    GameInfoExtractor extractor({ { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" } });

    QVERIFY2 (extractor.extract(m_siteContent), "GameInfoExtractor should extract");
    const auto &output = extractor.results();

    QVERIFY2 (!output.isEmpty(), "Buildings object shouldn't be empty");
    QVERIFY2 (output["buildings"].isObject(), "Buildings object should be saved");
}

void GameInfoExtractorTest::extractToFile()
{
    GameInfoExtractor extractor({
        { "forestry", "var produkt_name_forestry = (?<forestry>.*);" },
        { "products", "var produkt_name = (?<products>.*);" },
        { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" }
    });

    QVERIFY2 (extractor.extract(m_siteContent), "GameInfoExtractor should extract");

    QFile file(QString("%1/labels.json").arg(TEST_OUT_PWD));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(extractor.results()).toJson(QJsonDocument::Indented));
    } else {
        QFAIL("File should be opened");
    }
}

QTEST_MAIN(GameInfoExtractorTest)

#include "tst_gameinfoextractor.moc"
