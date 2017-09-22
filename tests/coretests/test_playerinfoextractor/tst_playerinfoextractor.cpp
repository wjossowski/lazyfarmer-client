#include "core/helpers/playerinfoextractor.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QFileInfo>
#include <QtTest>

Q_DECLARE_METATYPE(PlayerInfoExtractor*)

class PlayerInfoExtractorTest : public QObject
{
    Q_OBJECT

public:
    PlayerInfoExtractorTest ()
        : m_bigExtractor(new PlayerInfoExtractor)
        , m_smallExtractor(new PlayerInfoExtractor)
    { }

private slots:
    void initTestCase();

    void extractBasicInfo_data();
    void extractBasicInfo();

    void extractStorageInfo_data();
    void extractStorageInfo();

    void extractFarmsInfo_data();
    void extractFarmsInfo();

private:
    QSharedPointer<PlayerInfoExtractor> m_bigExtractor;
    QSharedPointer<PlayerInfoExtractor> m_smallExtractor;
};

void PlayerInfoExtractorTest::initTestCase()
{
    const auto parseFromFile = [&] (const QString &filename, const QSharedPointer<PlayerInfoExtractor> &extractor) {
        QFile siteFile (QString("%1/assets/%2.json").arg(TEST_PWD).arg(filename));
        if (!siteFile.open(QIODevice::ReadOnly)) {
            QFAIL ("Unable to open farminfo_small.json file");
        }
        qDebug() << QObject::tr("Reading from") << QFileInfo(siteFile).absoluteFilePath();
        QVERIFY2(extractor->parseInfo(std::move(siteFile.readAll())), QString("PlayerInfoExtractor should extract information from %1").arg(filename).toLatin1().constData());
    };

    parseFromFile("farminfo_big", m_bigExtractor);
    parseFromFile("farminfo_small", m_smallExtractor);
}

void PlayerInfoExtractorTest::extractBasicInfo_data()
{
    QTest::addColumn<PlayerInfoExtractor*>("extractor");
    QTest::addColumn<int>("level");
    QTest::addColumn<QString>("levelDescription");
    QTest::addColumn<int>("levelPercentage");

    QTest::addRow("Small File") << m_smallExtractor.data() << 1 << "Knecht" << 13;
    QTest::addRow("Big File") << m_bigExtractor.data() << 23 << "Kurzy treser" << 84;
}

void PlayerInfoExtractorTest::extractBasicInfo()
{
    QFETCH(PlayerInfoExtractor*, extractor);
    QFETCH(int, level);
    QFETCH(QString, levelDescription);
    QFETCH(int, levelPercentage);

    const auto &basicInfo = extractor->basicInfo();
    QVERIFY2(basicInfo["Level"] == level, "Level not parsed correctly");
    QVERIFY2(basicInfo["LevelDescription"] == levelDescription, "Level description not parsed correctly");
    QVERIFY2(basicInfo["LevelPercentage"] == levelPercentage, "Invalid level percentage");
}

void PlayerInfoExtractorTest::extractStorageInfo_data()
{
    QTest::addColumn<PlayerInfoExtractor*>("extractor");
    QTest::addColumn<int>("farms");
    QTest::addColumn<QList<int>>("buildingids");
    QTest::addColumn<QList<int>>("buildingnames");

    QTest::addRow("Small File");
    QTest::addRow("Big File");
}

void PlayerInfoExtractorTest::extractStorageInfo()
{
    QFAIL("Not Implemented yet!");
}

void PlayerInfoExtractorTest::extractFarmsInfo_data()
{
    QTest::addColumn<PlayerInfoExtractor*>("extractor");
    QTest::addColumn<int>("farms");
    QTest::addColumn<QList<int>>("buildingids");
    QTest::addColumn<QList<int>>("buildingnames");

    QTest::addRow("Small File");
    QTest::addRow("Big File");
}

void PlayerInfoExtractorTest::extractFarmsInfo()
{
    QFAIL("Not Implemented yet!");
}

QTEST_APPLESS_MAIN(PlayerInfoExtractorTest)
#include "tst_playerinfoextractor.moc"
