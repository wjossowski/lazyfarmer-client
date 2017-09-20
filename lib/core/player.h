#pragma once

#include "storage.h"

#include <QtCore/QObject>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY (quint32 level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY (QString levelDescription READ levelDescription WRITE setLevelDescription NOTIFY levelDescriptionChanged)

public:
    explicit Player(QObject *parent = nullptr);

    quint32 level() const { return m_level; }
    void setLevel(const quint32 &level);

    QString levelDescription() const { return m_levelDescription; }
    void setLevelDescription(const QString &levelDescription);

signals:
    void levelChanged(int level);
    void levelDescriptionChanged(const QString &levelDescription);

private:
    quint32 m_level;
    QString m_levelDescription;
    Storage m_storage;
};
