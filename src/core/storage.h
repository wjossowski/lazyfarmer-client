#pragma once

#include <QtCore/QObject>

class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(QObject *parent = nullptr);

signals:

public slots:
};
