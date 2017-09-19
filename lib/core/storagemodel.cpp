#include "storagemodel.h"

StorageModel::StorageModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant StorageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

bool StorageModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex StorageModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex StorageModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int StorageModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int StorageModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant StorageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool StorageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags StorageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool StorageModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool StorageModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool StorageModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool StorageModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}
