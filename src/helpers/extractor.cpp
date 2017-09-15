#include "extractor.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QFile>
#include <Qtcore/QSettings>

#include <QtDebug>

Extractor::Extractor(const QString &content, const QString &domain)
    : m_domain(domain)
    , m_filters({
//        { "forestry", "var produkt_name_forestry = (?<forestry>.*);" },
//        { "products", "var produkt_name = (?<products>.*);" },
        { "buildings", "var buildinginfos = eval\\(\\'(?<buildings>.*)\\'\\);" }
    })
{
    QSettings settings;
    settings.beginGroup("Extractor");

    auto extractObject = [] (const QJsonDocument &document) -> QJsonValue {
        if (document.isArray()) {
            QJsonValue container(document.array().first());
            return container.isObject() ? container : QJsonValue::Null;
        } else if (document.isObject()) {
            return document.object();
        } else {
            return QJsonValue::Null;
        }
    };

    auto extractJson = [&] (const QString &match, const QString &pattern) {
        const QRegularExpression regex(pattern);
        const QString captured = regex.match(content).captured(match);
        const auto &value = extractObject(QJsonDocument::fromJson(captured.toUtf8()));

        m_results.insert(match, value);
    };

    QMapIterator<QString, QVariant> filter(m_filters);
    while (filter.hasNext()) {
        filter.next();
        extractJson (filter.key(), filter.value().toString());
    }

    settings.setValue("ExtractorFilters", m_filters);
}

Extractor::~Extractor()
{
    QFile file(QString("labels_%1.json").arg(m_domain));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(m_results).toJson(QJsonDocument::Indented));
    }
}
