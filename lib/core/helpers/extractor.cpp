#include "extractor.h"

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

Extractor::Extractor(const QString &content, const QString &domain)
    : Extractor(content, BaseFilters, domain)
{

}

Extractor::Extractor(const QString &content,
                     const QVariantMap &filters,
                     const QString &domain)
    : m_filters(std::move(filters))
    , m_domain(domain)
{
    extract(content);
}

Extractor::~Extractor()
{
    if (m_domain.isEmpty())
        return;

    QFile file(QString("labels_%1.json").arg(m_domain));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << QObject::tr("Saving to") << QFileInfo(file).absoluteFilePath();
        file.write(QJsonDocument(m_results).toJson(QJsonDocument::Indented));
    }
}

void Extractor::extract(const QString &content)
{
    auto extractNameFromObject = [] (QJsonObject &&object) -> QJsonValue {
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
    };

    auto addNamePrefix = [&] (QJsonObject &object) {
        for (const auto &key : object.keys()) {
            object[key] = QJsonObject({
               { "name", object.value(key) },
               { "duration", -1 }
            });
        }
    };

    auto extractObject = [&] (const QJsonDocument &&document) -> QJsonValue {
        if (document.isArray()) {
            QJsonValue container(document.array().first());
            return container.isObject() ? extractNameFromObject(container.toObject()) : QJsonValue::Null;
        } else if (document.isObject()) {
            return document.object();
        } else {
            return QJsonValue::Null;
        }
    };

    auto extractJson = [&] (const QString &match, const QString &pattern) {
        const QRegularExpression regex(pattern);
        const QString captured = regex.match(content).captured(match).replace(QRegularExpression(",\\s*\\}$"), "}");
        auto value = extractObject(QJsonDocument::fromJson(captured.toUtf8())).toObject();

#if DEBUG_MODE
        m_regexMatches << captured;
#endif

        addNamePrefix(value);
        m_results.insert(match, value);
    };

    QMapIterator<QString, QVariant> filter(m_filters);
    while (filter.hasNext()) {
        filter.next();
        extractJson (filter.key(), filter.value().toString());
    }
}
