#include "extractor.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QFile>
#include <QtDebug>

Extractor::Extractor(const QString &content, const QString &domain)
    : Extractor(content, {
        { "forestry", "var produkt_name_forestry = (?<forestry>.*);" },
        { "products", "var produkt_name = (?<products>.*);" },
        { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" }
    }, domain)
{

}

Extractor::Extractor(const QString &content,
                     const QMap<QString, QVariant> &filters,
                     const QString &domain)
    : m_filters(filters)
    , m_domain(domain)
{
    auto extractObject = [this] (const QJsonDocument &document) -> QJsonValue {
        if (document.isArray()) {
            QJsonValue container(document.array().first());
            return container.isObject() ? moveNameFromArray(container.toObject()) : QJsonValue::Null;
        } else if (document.isObject()) {
            return document.object();
        } else {
            return QJsonValue::Null;
        }
    };

    auto extractJson = [&] (const QString &match, const QString &pattern) {
        const QRegularExpression regex(pattern);
        const QString captured = regex.match(content).captured(match).replace(QRegularExpression("(,)(?!.*\\\\)"), "");
        const auto value = extractObject(QJsonDocument::fromJson(captured.toUtf8()));

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
}

Extractor::~Extractor()
{
    if (m_domain.isEmpty())
        return;

    QFile file(QString("labels_%1.json").arg(m_domain));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(m_results).toJson(QJsonDocument::Indented));
    }
}


QJsonValue Extractor::moveNameFromArray(QJsonObject &&object) {
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
