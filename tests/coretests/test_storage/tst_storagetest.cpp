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
    StorageTest() { }

private slots:
    void update_data();
    void update();
};

void StorageTest::update_data()
{
    QTest::addColumn<QVariantList>("storage");
    QTest::addColumn<QList<int>>("indexes");
    QTest::addColumn<QList<int>>("amounts");

    QTest::newRow("Single row")
            << QVariantList({
                   QVariantMap({ { "Id", 1 }, { "Amount", 1 } })
               })
            << QList<int>({ 1 })
            << QList<int>({ 1 });

    QTest::newRow("Multiple ordered rows")
            << QVariantList({
                   QVariantMap({ { "Id", 1 }, { "Amount", 10 } }),
                   QVariantMap({ { "Id", 2 }, { "Amount", 20 } }),
                   QVariantMap({ { "Id", 3 }, { "Amount", 30 } }),
                   QVariantMap({ { "Id", 4 }, { "Amount", 40 } }),
               })
            << QList<int>({  1,  2,  3,  4 })
            << QList<int>({ 10, 20, 30, 40 });

    QTest::newRow("Multiple unordered rows")
            << QVariantList({
                   QVariantMap({ { "Id", 4 }, { "Amount", 40 } }),
                   QVariantMap({ { "Id", 2 }, { "Amount", 20 } }),
                   QVariantMap({ { "Id", 3 }, { "Amount", 30 } }),
                   QVariantMap({ { "Id", 1 }, { "Amount", 10 } }),
               })
            << QList<int>({  1,  2,  3,  4 })
            << QList<int>({ 10, 20, 30, 40 });
}

void StorageTest::update()
{
    QFETCH(QVariantList, storage);
    QFETCH(QList<int>, indexes);
    QFETCH(QList<int>, amounts);

    QScopedPointer<Storage> m_storage (new Storage);

    m_storage->update(storage);
    QVERIFY2 (m_storage->size() == storage.size(), "Storage sizes must be equal");

    int i = 0;
    const auto &products = m_storage->products();
    for (const auto &product : products) {
        QVERIFY2 (product->id() == indexes[i], "Invalid Product index");
        QVERIFY2 (product->amount() == amounts[i], "Invalid Product amount");
        i++;
    }
}

QTEST_APPLESS_MAIN(StorageTest)

#include "tst_storagetest.moc"
