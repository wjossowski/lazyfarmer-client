/**
 ** This file is part of the LazyFarmer project.
 ** Copyright 2017 Wojciech Ossowski <w.j.ossowski@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once

#include <QtCore/QtGlobal>
#include <QtCore/QVariantMap>
#include <QtDebug>

class Product
{
public:
    explicit Product(const QVariantMap &info)
        : m_id(info["Id"].toInt())
        , m_amount(info["Amount"].toInt()) { }

    Product (const Product &another) : m_id(another.id()) , m_amount(another.amount()) { }

    inline int id() const { return m_id; }
    inline void setId(const int value) { m_id = value; }
    inline int amount() const { return m_amount; }
    inline void setAmount(const int value) { m_amount = value; }

    friend bool operator <(const QSharedPointer<Product> &lhs,
                           const QSharedPointer<Product> &rhs)
        { return lhs->id() < rhs->id(); }

private:
    int m_id;
    int m_amount;
};
