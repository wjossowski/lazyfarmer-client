#pragma once

#include "product.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

class Storage : public QObject
{
    Q_OBJECT

public:
    explicit Storage(QObject *parent = nullptr);

private:
    QList<QSharedPointer<Product> > m_products;
};
