#pragma once

#include "storage.h"

#include <QtCore/QObject>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);

private:
    int m_level;

    Storage m_storage;
};
