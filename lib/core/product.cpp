#include "product.h"

Product::Product(quint32 id,
                 quint32 amount,
                 std::chrono::milliseconds &&duration,
                 QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_amount(amount)
    , m_duration(duration)
{

}

