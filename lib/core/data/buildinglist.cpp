#include "buildinglist.h"

#include <QtDebug>
#include <algorithm>

using namespace Core;
using namespace Core::Data;

BuildingList::BuildingList(Player *parent)
    : IPlayerData(parent)
{

}

Building::Ptr BuildingList::buildingAt(int farm, int position)
{
    const auto buildingIterator = std::find_if(m_buildings.cbegin(), m_buildings.cend(), [=] (const auto &ptr){
        return ptr->farmId() == farm && ptr->position() == position;
    });

    if (buildingIterator == m_buildings.cend()) {
        auto building = Building::Ptr(new Building(m_owner));
        connect(&*building, &Building::buildingChanged, [=] () {
            emit buildingChanged(m_buildings.indexOf(building));
        });

        m_buildings.append(building);

        return building;
    } else {
        return *buildingIterator;
    }
}

Building::Ptr BuildingList::buildingAt(int index)
{
    if (m_buildings.count() < index) {
        return Building::Ptr(new Building(m_owner));
    } else {
        return m_buildings.at(index);
    }
}

void BuildingList::update(const QVariant &info)
{
    const QVariantList farmInfo = info.toList();

    for (const auto &buildingInfo : farmInfo) {
        const QVariantMap &buildingInfoMap = buildingInfo.toMap();

        int farm = buildingInfoMap["FarmId"].toInt();
        int position = buildingInfoMap["Position"].toInt();

        auto building = buildingAt(farm, position);
        building->update(buildingInfoMap);
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

void BuildingList::updateBuilding(int farm, int position, const QVariant &info)
{
    const auto building = buildingAt(farm, position);
    building->updateBuildingData(info);
}
