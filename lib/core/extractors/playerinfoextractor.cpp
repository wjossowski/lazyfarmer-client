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

#include "playerinfoextractor.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

#include <QtDebug>

using namespace Core;
using namespace Core::Extractors;

#ifdef DEBUG_MODE
void printJson(const QJsonObject &obj) {
    qDebug() << QJsonDocument(obj).toJson(QJsonDocument::Indented);
}
#endif

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
        case TypeParsingError: return QObject::tr("Invalid puarsing type");
        default: return QObject::tr("Unknown error");
        }
    }

    PlayerParseException(Type exceptionType) : type(exceptionType) { }
};

void assertNotEmptyObject (const QJsonObject &object,
                           PlayerParseException::Type error = PlayerParseException::TypeParsingError)
{
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

void PlayerInfoExtractor::parseBasicInfo(const QJsonObject &basicInfo)
{
    assertNotEmptyObject(basicInfo, PlayerParseException::EmptyBasicInfo);

    m_basicInfo.clear();

    m_basicInfo["Level"] = basicInfo["levelnum"].toString().toInt();
    m_basicInfo["LevelDescription"] = basicInfo["level"].toString();
    m_basicInfo["LevelPercentage"] = countPercentage(basicInfo["points"], basicInfo["levelcurrentlimit"], basicInfo["levellimit"]);
    m_basicInfo["Money"] = basicInfo["bar"].toString().toDouble();

    m_basicInfo["Tutorial"] = basicInfo["tutorial"].toString().toInt();
}

QVariantMap extractProduct(const QJsonObject &product)
{
    QVariantMap productMap;
    productMap["Id"] = product["pid"].toString().toInt();
    productMap["Amount"] = product["amount"].toString().toInt();
    return productMap;
}

QVariantList extractProductsFromRack(const QJsonObject &rack)
{
    QVariantList list;
    // extract products
    for (const auto &product : rack) {
        list.append(extractProduct(product.toObject()));
    }
    // sort rack
    std::sort(list.begin(), list.end(), [] (const QVariant &lhs, const QVariant &rhs) {
        return lhs.toMap()["Id"].toInt() < rhs.toMap()["Id"].toInt();
    });

    return list;
}

void PlayerInfoExtractor::parseStorageInfo(const QJsonObject &storageInfo)
{
    assertNotEmptyObject(storageInfo, PlayerParseException::EmptyStorageInfo);

    m_storageInfo.clear();

    for (const auto &node : storageInfo) {
        if (node.isObject()) {
            for (const auto &rack : node.toObject()) {
                m_storageInfo.append(extractProductsFromRack(rack.toObject()));
            }
        } else {
            // Todo
        }
    }
}

QVariantMap extractBuildingInfo(int type, const QJsonObject &buildingObject)
{
    assertNotEmptyObject(buildingObject);

    auto remaining = [] (const QJsonValue &production) {
        if (!production.isArray())
            return 0;

        int max = -1;
        foreach (const auto &process, production.toArray()) {
            max = qMax(max, process.toObject()["remain"].toInt());
        }
        return max;
    };

    QVariantMap building;
    building["Type"] = type;
    building["FarmId"] = buildingObject["farm"].toString().toInt();
    building["Position"] = buildingObject["position"].toString().toInt();
    building["Level"] = buildingObject["level"].toString().toInt();
    building["Animals"] = buildingObject["animals"].toString().toInt();
    building["Remaining"] = remaining(buildingObject["production"]);

    return building;
}

void extractFarmInfo(const QJsonObject &farmObject, QVariantList &farmsInfo)
{
    assertNotEmptyObject(farmObject);

    for (const auto &building : farmObject) {
        const auto &buildingObject = building.toObject();
        int type = buildingObject["buildingid"].toString().toInt();
        if (type == 0)
            continue;

        farmsInfo.append(extractBuildingInfo(type, buildingObject));
    }
}

void PlayerInfoExtractor::parseFarmsInfo(const QJsonObject &farmsInfo)
{
    assertNotEmptyObject(farmsInfo);

    m_farmsInfo.clear();

    for (const auto &value : farmsInfo) {
        extractFarmInfo(value.toObject(), m_farmsInfo);
    }
}
