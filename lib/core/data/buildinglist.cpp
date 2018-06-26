#include "buildinglist.h"

#include <QtDebug>

using namespace Core;
using namespace Core::Data;

BuildingList::BuildingList(Player *parent)
    : IPlayerData(parent)
{

}

QSharedPointer<Building> BuildingList::buildingAt(int farm, int position)
{
    for (auto building : m_buildings) {
        if (building->farmId() == farm && building->position() == position) {
            return building;
        }
    }

    return QSharedPointer<Building>(nullptr);
}

void BuildingList::update(const QVariant &info)
{
    const QVariantList farmInfo = info.toList();

    for (const auto &buildingInfo : farmInfo) {
        const QVariantMap &buildingInfoMap = buildingInfo.toMap();

        int farm = buildingInfoMap["FarmId"].toInt();
        int position = buildingInfoMap["Position"].toInt();

        auto building = buildingAt(farm, position);
        if (building.isNull()) {
            building.reset(new Building(m_owner));
            m_buildings.append(building);
        }

        building->update(buildingInfoMap);

        if (building->isFarm()) {
            // TODO: Handle getting farm info here
        }

    }
}

QString BuildingList::toString() const
{
    QStringList buildingContents;

    for (const auto &building : m_buildings) {
        buildingContents.append(QString("(%1)").arg(building->toString()));
    }

    return QString("Buildings:(%1)").arg(buildingContents.join(", "));
}
