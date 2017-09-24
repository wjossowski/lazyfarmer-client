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

#include "gameinfoextractor.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtDebug>

static const QVariantMap BaseFilters = {
    { "forestry", "var produkt_name_forestry = (?<forestry>.*);" },
    { "products", "var produkt_name = (?<products>.*);" },
    { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" }
};

GameInfoExtractor::GameInfoExtractor(const QString &domain)
    : GameInfoExtractor(BaseFilters, domain)
{

}

GameInfoExtractor::GameInfoExtractor(const QVariantMap &filters,
                                     const QString &domain)
    : m_filters(std::move(filters))
    , m_domain(domain)
{

}

bool GameInfoExtractor::extract(const QString &content)
{
    auto extractJson = [&] (const QString &match, const QString &pattern) {
        const QRegularExpression regex(pattern);
        const QString captured = regex.match(content).captured(match).replace(QRegularExpression(",\\s*\\}$"), "}");
        auto value = extractObject(QJsonDocument::fromJson(captured.toUtf8())).toObject();

#if DEBUG_MODE
        m_regexMatches << captured;
#endif

        m_results.insert(match, value);
    };

    QMapIterator<QString, QVariant> filter(m_filters);
    while (filter.hasNext()) {
        filter.next();
        extractJson (filter.key(), filter.value().toString());
    }

    return true;
}

void GameInfoExtractor::save()
{
    if (m_domain.isEmpty())
        return;

    QFile file(QString("labels_%1.json").arg(m_domain));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << QObject::tr("Saving to") << QFileInfo(file).absoluteFilePath();
        file.write(QJsonDocument::fromVariant(m_results).toJson(QJsonDocument::Indented));
    }
}

QJsonValue GameInfoExtractor::extractNameFromObject(QJsonObject &&object) const
{
    for (const auto &key : object.keys()) {
        QJsonArray array = object[key].toArray();

        if (array.isEmpty())
            continue;

        for (const auto &item : array) {
            if (!item.isString())
                continue;

            const QString &value = item.toString();
            QRegularExpression re (".(png|gif|jpg|jpeg)$");

            if (re.match(value).hasMatch())
                continue;

            object[key] = value;
            break;
        }
    }

    return QJsonValue(object);
}

QJsonValue GameInfoExtractor::extractObject(QJsonDocument &&document) const
{
    if (document.isArray()) {
        QJsonValue container(document.array().first());
        return container.isObject() ? extractNameFromObject(container.toObject()) : QJsonValue::Null;
    } else if (document.isObject()) {
        return document.object();
    } else {
        return QJsonValue::Null;
    }
}
