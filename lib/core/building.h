#pragma once

#include <QtCore/QObject>

class Building : public QObject
{
    Q_OBJECT

public:
    explicit Building(QObject *parent = nullptr);

};
