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
    const QVariantMap &results() { return m_results; }
    void save();

private:
    QVariantMap extractNameFromObject(QJsonObject &&object) const;
    QVariantMap extractObject(QJsonDocument &&document) const;

private:
    QVariantMap m_filters;
    QVariantMap m_results;

    QString m_domain;

#if DEBUG_MODE
    QStringList m_regexMatches;
#endif
};


