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
    void update_data();
    void update();

private:
    friend class Storage;
    QScopedPointer<Storage> m_storage;
};

void StorageTest::update_data()
{
    QTest::addColumn<QVariantList>("storage");
    QTest::addColumn<QList<quint32>>("indexes");
    QTest::addColumn<QList<quint32>>("amounts");

    QTest::addRow("Single row")
            << QVariantList({
                   QVariantMap({ { "Id", 1 }, { "Amount", 1 } })
               })
            << QList<quint32>({ 1 })
            << QList<quint32>({ 1 });

    QTest::addRow("Multiple ordered rows")
            << QVariantList({
                   QVariantMap({ { "Id", 1 }, { "Amount", 1 } }),
                   QVariantMap({ { "Id", 2 }, { "Amount", 2 } }),
                   QVariantMap({ { "Id", 3 }, { "Amount", 3 } }),
                   QVariantMap({ { "Id", 4 }, { "Amount", 4 } }),
               })
            << QList<quint32>({ 1, 2, 3, 4 })
            << QList<quint32>({ 1, 2, 3, 4 });

    QTest::addRow("Multiple unordered rows")
            << QVariantList({
                   QVariantMap({ { "Id", 4 }, { "Amount", 4 } }),
                   QVariantMap({ { "Id", 2 }, { "Amount", 2 } }),
                   QVariantMap({ { "Id", 3 }, { "Amount", 3 } }),
                   QVariantMap({ { "Id", 1 }, { "Amount", 1 } }),
               })
            << QList<quint32>({ 1, 2, 3, 4 })
            << QList<quint32>({ 1, 2, 3, 4 });
}

void StorageTest::update()
{
    QFETCH(QVariantList, storage);
    QFETCH(QList<quint32>, indexes);
    QFETCH(QList<quint32>, amounts);

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
