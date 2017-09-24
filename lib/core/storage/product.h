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

    ~Product() { qDebug() << Q_FUNC_INFO << m_id; }

    inline Product operator =(const Product &another)
        { m_amount = another.amount(); return *this; }

    inline bool operator ==(const Product &another)
        { return m_id == another.id(); }

    inline bool operator <(const Product &another)
        { qDebug() << Q_FUNC_INFO; return m_id < another.id(); }

    inline quint32 id() const { return m_id; }
    inline void setId(const quint32 &value) { m_id = value; }
    inline quint32 amount() const { return m_amount; }
    inline void setAmount(const quint32 &value) { m_amount = value; }

private:
    quint32 m_id;
    quint32 m_amount;
};
