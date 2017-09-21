#include "playerinfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
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

bool PlayerInfoExtractor::parseInfo(const QByteArray &info)
{
    try {
        QJsonObject updateObject = QJsonDocument::fromJson(info).object()["updateblock"].toObject();

        if (!updateObject.isEmpty()) {
            parseBasicInfo(updateObject["menue"].toObject());
            parseStorageInfo(updateObject["stock"].toObject());
            parseFarmsInfo(updateObject["farms"].toObject());
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

void PlayerInfoExtractor::parseBasicInfo(const QJsonObject &basicInfo)
{
    if (basicInfo.isEmpty())
        throw PlayerParseException(PlayerParseException::EmptyBasicInfo);

    m_basicInfo["Level"] = [] (const QJsonValue &lvl) { return lvl.isString() ? lvl.toString().toInt() : lvl.toInt() ; } (basicInfo["levelnum"]);
    m_basicInfo["LevelDescription"] = basicInfo["level"].toString();
    m_basicInfo["LevelPercentage"] = countPercentage(basicInfo["points"], basicInfo["levelcurrentlimit"], basicInfo["levellimit"]);
}

void PlayerInfoExtractor::parseStorageInfo(const QJsonObject &storageInfo)
{
    if (storageInfo.isEmpty())
        throw PlayerParseException(PlayerParseException::EmptyStorageInfo);
}

void PlayerInfoExtractor::parseFarmsInfo(const QJsonObject &farmsInfo)
{
    if (farmsInfo.isEmpty())
        throw PlayerParseException(PlayerParseException::EmptyFarmsInfo);
}
