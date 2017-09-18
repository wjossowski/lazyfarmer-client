#pragma once

#include "storage.h"

#include <QtCore/QObject>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);

private:
    quint32 m_level;
    QString m_levelDescription;

    Storage m_storage;
};
