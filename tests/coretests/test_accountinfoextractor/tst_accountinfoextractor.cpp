#include "core/player.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QFileInfo>
#include <QtTest>

class AccountInfoExtractor : public QObject
{
    Q_OBJECT

public:
    AccountInfoExtractor () : m_player(new Player) { }

private slots:
    void initTestCase();

    void extractBasicInfo();

    void cleanupTestCase();

private:
    QString m_accountInfoContent;
    QScopedPointer<Player> m_player;
};

void AccountInfoExtractor::initTestCase()
{
    QFile siteFile (QString("%1/assets/farminfo_small.json").arg(TEST_PWD));
    if (!siteFile.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open farminfo_small.json file");
    }
    qDebug() << QObject::tr("Reading from") << QFileInfo(siteFile).absoluteFilePath();
    m_accountInfoContent = QString::fromUtf8(siteFile.readAll());
}

void AccountInfoExtractor::extractBasicInfo()
{

}

void AccountInfoExtractor::cleanupTestCase()
{

}

QTEST_APPLESS_MAIN(AccountInfoExtractor)

#include "tst_accountinfoextractor.moc"
