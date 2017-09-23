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
        QVERIFY2(extractor->parseInfo(std::move(siteFile.readAll())),
                 QString("PlayerInfoExtractor should extract information from %1").arg(filename).toLatin1().constData());
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
    QTest::addColumn<int>("size");
    QTest::addColumn<QVariantList>("ids");
    QTest::addColumn<QVariantList>("amount");

    QTest::addRow("Small File") << m_smallExtractor.data()
                                   // Storage size
                                << 1
                                   // Storage ids
                                << QVariantList({ 17 })
                                   // Storage amount
                                << QVariantList({ 6 });

    QTest::addRow("Big File") << m_bigExtractor.data()
                                 // Storage size
                              << 19
                                 // Storage ids
                              << QVariantList({   1,   4,   7,  18,  20,  26,  93, 104, 107, 108,
                                                109, 110, 111, 159, 172, 173, 174, 189, 202})
                                 // Storage amount
                              << QVariantList({1196, 157,  25, 789, 177,  22,   3,  50,  10, 8773,
                                               1490,  81, 253,   1, 212, 828, 896,   1,  14});
}

void PlayerInfoExtractorTest::extractStorageInfo()
{
    QFETCH(PlayerInfoExtractor*, extractor);
    QFETCH(int, size);
    QFETCH(QVariantList, ids);
    QFETCH(QVariantList, amount);

    const auto &storageInfo = extractor->storageInfo();
    QVERIFY2(storageInfo.size() == size, "Invalid storage length");

    int i = 0;
    for (const auto &info : storageInfo) {
        const auto &product = info.toMap();
        if (product["Id"] != ids[i]) {
            continue;
        }

        QVERIFY2(product["Id"] == ids[i], "Invalid product Id");
        QVERIFY2(product["Amount"] == amount[i], "Invalid product amount");

        i++;
    }
}

void PlayerInfoExtractorTest::extractFarmsInfo_data()
{
    QTest::addColumn<PlayerInfoExtractor*>("extractor");
    QTest::addColumn<int>("farms");
    QTest::addColumn<QVariantMap>("buildingids");
    QTest::addColumn<QVariantMap>("levels");
    QTest::addColumn<QVariantMap>("animals");
    QTest::addColumn<QVariantMap>("remaining");

    QTest::addRow("Small File") << m_smallExtractor.data()
                                   // amount of farms
                                << 1
                                   // building ids
                                << QVariantMap({{"1", QList<QVariant>({1, 0, 0, 0, 0, 0})}})
                                   // levels
                                << QVariantMap({{"1", QList<QVariant>({1, 0, 0, 0, 0, 0})}})
                                   // number of animals
                                << QVariantMap({{"1", QList<QVariant>({0, 0, 0, 0, 0, 0})}})
                                   // remaining time
                                << QVariantMap({{"1", QList<QVariant>({0, 0, 0, 0, 0, 0})}});


    QTest::addRow("Big File") << m_bigExtractor.data()
                                 // amount of farms
                              << 2
                                 // building ids
                              << QVariantMap({{"1", QList<QVariant>({1, 8,        8, 12, 0, 12})},
                                              {"2", QList<QVariant>({8, 0,        8,  0, 8,  8})}})
                                 // levels
                              << QVariantMap({{"1", QList<QVariant>({1, 1,        1,  2, 0,  2})},
                                              {"2", QList<QVariant>({1, 0,        1,  0, 1,  1})}})
                                 // number of animals
                              << QVariantMap({{"1", QList<QVariant>({0, 0,        0,  3, 0,  3})},
                                              {"2", QList<QVariant>({0, 0,        0,  0, 0,  0})}})
                                 // remaining time;
                              << QVariantMap({{"1", QList<QVariant>({0, 31309697, 0,  0, 0,  0})},
                                              {"2", QList<QVariant>({0, 0,        0,  0, 0,  0})}});
}

void PlayerInfoExtractorTest::extractFarmsInfo()
{
    QFETCH(PlayerInfoExtractor*, extractor);
    QFETCH(int, farms);
    QFETCH(QVariantMap, buildingids);
    QFETCH(QVariantMap, levels);
    QFETCH(QVariantMap, animals);
    QFETCH(QVariantMap, remaining);

    const auto &farmInfo = extractor->farmsInfo();

    auto getVariantMapValue = [&] (const QVariantMap &source, const QString &farmId, const QString &buildingId) {
        return source[farmId].toList()[buildingId.toInt() - 1].toInt();
    };

    // Compare farms availability amount
    QVERIFY2(farmInfo.size() == farms, "Wrong amount of farms");

    for (const auto &farmKey : farmInfo.keys()) {
        const auto &buildings = farmInfo[farmKey].toMap();

        for (const auto &buildingKey : buildings.keys()) {
            const auto &buildingInfo = buildings[buildingKey].toMap();

            int id = getVariantMapValue(buildingids, farmKey, buildingKey);
            int level = getVariantMapValue(levels, farmKey, buildingKey);
            int animalsCount = getVariantMapValue(animals, farmKey, buildingKey);
            int remainingTime = getVariantMapValue(remaining, farmKey, buildingKey);

            QVERIFY2(buildingInfo["Id"] == id, "Building id doesn't match");
            QVERIFY2(buildingInfo["Level"] == level, "Building level doesn't  match");
            QVERIFY2(buildingInfo["Animals"] == animalsCount, "Animals count doesn't match");
            QVERIFY2(buildingInfo["Remaining"] == remainingTime, "Remaining time doesn't match");
        }
    }
}

QTEST_APPLESS_MAIN(PlayerInfoExtractorTest)
#include "tst_playerinfoextractor.moc"