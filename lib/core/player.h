#pragma once

#include "storage.h"

#include <QtCore/QObject>

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QObject *parent = nullptr);

    void setBasicInfo(const QVariantMap &basicInfo);

    quint32 level() const { return m_level; }
    QString levelDescription() const { return m_levelDescription; }
    quint32 levelPercentage() const { return m_levelPercentage; }

private:
    int m_level;
    QString m_levelDescription;
    int m_levelPercentage;

    Storage m_storage;
};
