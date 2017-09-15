#pragma once

#include <QtCore/QObject>

class ExtractorTest : public QObject
{
    Q_OBJECT

public:
    ExtractorTest () {}

private slots:
    void initTestCase();
    void cleanupTestCase();
    void extractBuildings();
};
