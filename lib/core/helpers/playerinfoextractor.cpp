#include "playerinfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

#include <QtDebug>

struct PlayerParseException {
    enum Type {
        EmptyResponseObject,
        EmptyBasicInfo,
        EmptyStorageInfo,
        EmptyFarmsInfo,
        TypeParsingError
    } type;

    QString toString() const
    {
        switch (type) {
        case EmptyResponseObject: return QObject::tr("Empty respose object");
        case EmptyBasicInfo: return QObject::tr("Empty basic info");
        case EmptyStorageInfo: return QObject::tr("Empty storage info");
        case EmptyFarmsInfo: return QObject::tr("Empty farms info");
        case TypeParsingError: return QObject::tr("Invalid parsing type");
        default: return QObject::tr("Unknown error");
        }
    }

    PlayerParseException(Type exceptionType) : type(exceptionType) {}
};

void assertNotEmptyObject (const QJsonObject &object, PlayerParseException::Type error = PlayerParseException::TypeParsingError) {
    if (object.isEmpty())
        throw PlayerParseException(error);
}

bool PlayerInfoExtractor::parseInfo(const QByteArray &info)
{
    try {
        QJsonObject updateObject = QJsonDocument::fromJson(info).object()["updateblock"].toObject();

        if (!updateObject.isEmpty()) {
            parseBasicInfo(updateObject["menue"].toObject());
            parseStorageInfo(updateObject["stock"].toObject()["stock"].toObject());
            parseFarmsInfo(updateObject["farms"].toObject()["farms"].toObject());
        } else {
            throw PlayerParseException(PlayerParseException::EmptyResponseObject);
        }

    } catch (PlayerParseException &ex) {
        m_error = ex.toString();
        qWarning() << m_error;
        return false;
    }

    return true;
}

int countPercentage(const QJsonValue &exp, const QJsonValue &levelExp, const QJsonValue &nextLevelExp)
{
    if (!exp.isString() || !levelExp.isDouble() || !nextLevelExp.isDouble())
        throw PlayerParseException(PlayerParseException::TypeParsingError);

    double expPoints = exp.toString().toDouble();
    double levelExpPoints = levelExp.toDouble();
    double nextLevelExpPoints = nextLevelExp.toDouble();

    return qRound(100.0 * ((expPoints - levelExpPoints) / (nextLevelExpPoints - levelExpPoints)));
}

void printJson(const QJsonObject &obj) {
    qDebug() << QJsonDocument(obj).toJson(QJsonDocument::Indented);
}

void PlayerInfoExtractor::parseBasicInfo(const QJsonObject &basicInfo)
{
    assertNotEmptyObject(basicInfo, PlayerParseException::EmptyBasicInfo);

    m_basicInfo["Level"] = basicInfo["levelnum"].toString().toInt();
    m_basicInfo["LevelDescription"] = basicInfo["level"].toString();
    m_basicInfo["LevelPercentage"] = countPercentage(basicInfo["points"], basicInfo["levelcurrentlimit"], basicInfo["levellimit"]);
}

void PlayerInfoExtractor::parseStorageInfo(const QJsonObject &storageInfo)
{
    assertNotEmptyObject(storageInfo, PlayerParseException::EmptyStorageInfo);

//    printJson(storageInfo);
}

QVariantMap extractBuildingInfo(const QJsonObject &buildingObject)
{
    assertNotEmptyObject(buildingObject);

    auto remaining = [] (const QJsonValue &production) {
        if (!production.isArray())
            return 0;

        int max = 0;
        foreach (const auto &process, production.toArray()) {
            max = qMax(max, process.toObject()["remain"].toInt());
        }
        return max;
    };

    QVariantMap building;
    building["Id"] = buildingObject["buildingid"].toString().toInt();
    building["Name"] = buildingObject["name"].toString();
    building["Level"] = buildingObject["level"].toString().toInt();
    building["Animals"] = buildingObject["animals"].toString();
    building["Remaining"] = remaining(buildingObject["production"]);

    return building;
}

QVariantMap extractFarmInfo(const QJsonObject &farmObject)
{
    assertNotEmptyObject(farmObject);

    QVariantMap buildings;
    foreach (const auto &key, farmObject.keys()) {
        buildings[key] = extractBuildingInfo(farmObject[key].toObject());
    }
    return buildings;
}

void PlayerInfoExtractor::parseFarmsInfo(const QJsonObject &farmsInfo)
{
    assertNotEmptyObject(farmsInfo);

    foreach (const auto &key, farmsInfo.keys()) {
        m_farmsInfo[key] = extractFarmInfo(farmsInfo[key].toObject());
    }
}
