#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>

class GameInfoExtractor
{
    Q_DISABLE_COPY(GameInfoExtractor)

public:
    explicit GameInfoExtractor(const QString &domain = QString());

    GameInfoExtractor(const QVariantMap &filters,
                      const QString &domain = QString());

    inline QString domain() const { return m_domain; }
    inline void setDomain(const QString &domain) { m_domain = domain; }

#if DEBUG_MODE
    inline const QStringList &regexMatches() const { return m_regexMatches; }
#endif

    bool extract(const QString &content);
    const QJsonObject &results() { return m_results; }
    void save();

private:
    QJsonValue extractNameFromObject(QJsonObject &&object) const;
    QJsonValue extractObject(QJsonDocument &&document) const;

private:
    QVariantMap m_filters;
    QString m_domain;
    QJsonObject m_results;

#if DEBUG_MODE
    QStringList m_regexMatches;
#endif
};


