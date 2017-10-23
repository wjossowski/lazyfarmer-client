#include "building.h"

Building::Building()
    : m_type(0)
    , m_farmId(0)
    , m_position(0)
    , m_level(0)
    , m_animals(0)
    , m_remaining(0)
{

}

void Building::update(const QVariantMap &buildingInfo)
{
    m_type = buildingInfo["Type"].toInt();
    m_farmId = buildingInfo["FarmId"].toInt();
    m_position = buildingInfo["Position"].toInt();
    m_level = buildingInfo["Level"].toInt();
    m_animals = buildingInfo["Animals"].toInt();
    m_remaining = buildingInfo["Remaining"].toInt();
}
