#include "core/helpers/playerinfoextractor.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QFileInfo>
#include <QtTest>

class PlayerInfoExtractorTest : public QObject
{
    Q_OBJECT

public:
    PlayerInfoExtractorTest () : m_extractor(new PlayerInfoExtractor) { }

private slots:
    void initTestCase();

    void extract();

    void extractBasicInfo();
    void extractStorageInfo();
    void extractFarmsInfo();

private:
    QByteArray m_accountInfoContent;
    QScopedPointer<PlayerInfoExtractor> m_extractor;
};

void PlayerInfoExtractorTest::initTestCase()
{
    QFile siteFile (QString("%1/assets/farminfo_big.json").arg(TEST_PWD));
    if (!siteFile.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open farminfo_small.json file");
    }
    qDebug() << QObject::tr("Reading from") << QFileInfo(siteFile).absoluteFilePath();
    m_accountInfoContent = std::move(siteFile.readAll());

}

void PlayerInfoExtractorTest::extract()
{
    QVERIFY2(m_extractor->parseInfo(m_accountInfoContent), "PlayerInfoExtractor should extract information");
}

void PlayerInfoExtractorTest::extractBasicInfo()
{
    const auto &basicInfo = m_extractor->basicInfo();
    QVERIFY2(basicInfo["Level"] == 23, "Level not parsed correctly");
    QVERIFY2(basicInfo["LevelDescription"] == "Kurzy treser", "Level description not parsed correctly");
    QVERIFY2(basicInfo["LevelPercentage"] == 84, "Invalid level percentage");
}

void PlayerInfoExtractorTest::extractStorageInfo()
{
    QFAIL("Not Implemented yet!");
}

void PlayerInfoExtractorTest::extractFarmsInfo()
{
    QFAIL("Not Implemented yet!");
}

QTEST_APPLESS_MAIN(PlayerInfoExtractorTest)

#include "tst_playerinfoextractor.moc"
