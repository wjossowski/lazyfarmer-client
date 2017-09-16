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
    explicit Extractor(const QString &content,
                       const QString &domain = QString());

    Extractor(const QString &content,
              const QMap<QString, QVariant> &filters,
              const QString &domain = QString());

    virtual ~Extractor();

    inline const QJsonObject &results() const { return m_results; }

    inline QString domain() const { return m_domain; }
    inline void setDomain(const QString &domain) { m_domain = domain; }

#if DEBUG_MODE
    inline const QStringList &regexMatches() const { return m_regexMatches; }
#endif

private:
    QJsonValue moveNameFromArray(QJsonObject &&object);

private:
    QMap<QString, QVariant> m_filters;
    QString m_domain;
    QJsonObject m_results;

#if DEBUG_MODE
    QStringList m_regexMatches;
#endif
};


