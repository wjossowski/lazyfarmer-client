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

#include <QtTest>

#include "core/storage/storage.h"

class StorageTest : public QObject
{
    Q_OBJECT

public:
    StorageTest() : m_storage(new Storage) { }

private slots:
    void insertOne();
    void insertExisting();

private:
    friend class Storage;
    QScopedPointer<Storage> m_storage;
};

void StorageTest::insertOne()
{
    m_storage->update({
        QVariantMap({ {"Id", 1}, { "Amount", 12 } })
    });

    QVERIFY2(m_storage->size() == 1, "Product should be appended to empty storage");
    const QSharedPointer<Product> &last = m_storage->products().last();
    QVERIFY2(last->id() == 1, "Invalid Id");
    QVERIFY2(last->amount() == 12, "Amount should be updated to latest value");
}

void StorageTest::insertExisting()
{
    m_storage->update({
        QVariantMap({ {"Id", 1}, { "Amount", 21 } })
    });

    QVERIFY2(m_storage->size() == 1, "Product with the same ID should be updated");
    const QSharedPointer<Product> &last = m_storage->products().last();
    QVERIFY2(last->id() == 1, "Invalid Id");
    QVERIFY2(last->amount() == 21, "Amount should be updated to latest value");
}

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
