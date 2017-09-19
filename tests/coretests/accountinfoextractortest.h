#pragma once

#include <QtCore/QObject>

class AccountInfoExtractorTest : public QObject
{
    Q_OBJECT

public:
    AccountInfoExtractorTest () { }

private slots:
    void initTestCase();

    void extractBasicInfo();

    void cleanupTestCase();

private:
    QString m_accountInfoContent;

};
