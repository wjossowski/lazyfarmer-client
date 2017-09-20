#pragma once

#include <QtCore/QObject>

class ExtractorTest : public QObject
{
    Q_OBJECT

public:
    ExtractorTest () {}

private slots:
    void initTestCase();

    void extractProducts();
    void extractForestry();
    void extractBuildings();

    void cleanupTestCase();

private:
    QString m_siteContent;
};
