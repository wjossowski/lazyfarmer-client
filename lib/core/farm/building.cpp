#include "building.h"

Building::Building(QObject *parent)
    : QObject(parent)
    , m_type(0)
    , m_position(0)
    , m_farmId(0)
    , m_level(0)
    , m_animals(0)
    , m_remaining(0)
{

}

void Building::update(const QVariantMap &buildingData)
{

}
