#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>

class Extractor
{
    Q_DISABLE_COPY(Extractor)

public:
    explicit Extractor(const QString &content, const QString &domain);
    virtual ~Extractor();

private:
    QString m_domain;
    QMap<QString, QVariant> m_filters;
    QJsonObject m_results;
};


