#pragma once

#include <QObject>

class AbstractField : public QObject
{
    Q_OBJECT

public:
    explicit AbstractField(QObject *parent = nullptr);

};
