#include "playerfarm.h"

#include <QtDebug>

using namespace Farm;

QSharedPointer<Building> PlayerFarm::buildingAt(int farm, int position)
{
    for (auto building : m_buildings) {
        if (building->farmId == farm && building->position == position) {
            return building;
        }
    }

    return QSharedPointer<Building>(nullptr);
}

void PlayerFarm::update(const QVariantList &farmInfo)
{
    for (const auto &buildingInfo : farmInfo) {
        const QVariantMap &buildingInfoMap = buildingInfo.toMap();

        int farm = buildingInfoMap["FarmId"].toInt();
        int position = buildingInfoMap["Position"].toInt();

        auto building = buildingAt(farm, position);
        if (building.isNull()) {
            building.reset(new Building);
            m_buildings.append(building);
        }

        building->update(buildingInfoMap);
    }
}
