#include "accountinfoextractortest.h"

#include <QtTest/QTest>
#include <QtCore/QRegularExpression>

void AccountInfoExtractorTest::initTestCase()
{
    QFile siteFile (QString("%1/assets/farminfo_small.json").arg(TEST_PWD));
    if (!siteFile.open(QIODevice::ReadOnly)) {
        QFAIL ("Unable to open farminfo_small.json file");
    }
    m_accountInfoContent = QString::fromUtf8(siteFile.readAll());
}

void AccountInfoExtractorTest::extractBasicInfo()
{

}

void AccountInfoExtractorTest::cleanupTestCase()
{

}
