#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

class QJsonObject;

class PlayerInfoExtractor
{

public:
    explicit PlayerInfoExtractor() { }

    bool parseInfo(const QByteArray &info);

    const QString &errorString() const { return m_error; }

    const QVariantMap &basicInfo() const { return m_basicInfo; }
    const QVariantMap &storageInfo() const { return m_storageInfo; }
    const QVariantMap &farmsInfo() const { return m_farmsInfo; }

private:
    void parseBasicInfo(const QJsonObject &basicInfo);
    void parseStorageInfo(const QJsonObject &storageInfo);
    void parseFarmsInfo(const QJsonObject &farmsInfo);

private:
    QVariantMap m_basicInfo;
    QVariantMap m_storageInfo;
    QVariantMap m_farmsInfo;

    QString m_error;
};
