#pragma once

#include <QObject>

class Product : public QObject
{
    Q_OBJECT

public:
    explicit Product(QObject *parent = nullptr);

};
