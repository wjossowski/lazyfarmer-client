#pragma once

#include <QtCore/QObject>
#include <chrono>

class Product : public QObject
{
    Q_OBJECT

public:
    explicit Product(quint32 id,
                     quint32 amount,
                     std::chrono::milliseconds &&duration,
                     QObject *parent = nullptr);

    inline quint32 id() const { return m_id; }
    inline void setId(const quint32 id) { m_id = id; }
    inline quint32 amount() const { return m_amount; }
    inline void setAmount(const quint32 amount) { m_amount = amount; }
    inline std::chrono::milliseconds duration() const { return m_duration; }
    inline void setDuration(const std::chrono::duration<quint32, std::giga> &duration) { m_duration = duration; }

private:
    quint32 m_id;
    quint32 m_amount;
    std::chrono::milliseconds m_duration;
};
