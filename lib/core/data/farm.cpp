#include "farm.h"

#include <QtDebug>

using namespace Core;
using namespace Core::Data;

QSharedPointer<Building> Farm::buildingAt(int farm, int position)
{
    for (auto building : m_buildings) {
        if (building->farmId == farm && building->position == position) {
            return building;
        }
    }

    return QSharedPointer<Building>(nullptr);
}

void Farm::update(const QVariantList &farmInfo)
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
