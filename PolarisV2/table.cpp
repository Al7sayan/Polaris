#include "table.h"

namespace polaris
{
    void Table::Update()
    {
        for (auto i = m_lPlates.begin(); i != m_lPlates.end(); ++i)
            (*i)->Update();
    }

    void Table::PushPlate(Plate* plate)
    {
        // NOTE: If there's no plates yet, not having this check will cause a crash.
        if(m_lPlates.front() != nullptr)
            m_lPlates.front()->OnDisabled();

        m_lPlates.push_front(plate);
        plate->OnEnabled();
    }
    void Table::PopPlate()
    {
        // NOTE: If there's no plates yet, not having this check will cause a crash.
        if (m_lPlates.front() != nullptr)
            m_lPlates.front()->OnDisabled();

        m_lPlates.pop_front();
        m_lPlates.front()->OnEnabled();
    }
    void Table::Pop()
    {
        m_lPlates.clear();
        delete this;
    }
}