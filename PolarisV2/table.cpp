#include "table.h"

namespace polaris
{
    void Table::Update()
    {
        if (m_lPlates.size() > 0)
            m_lPlates.front()->Update();
    }

    void Table::PushPlate(Plate* plate)
    {
        // NOTE: If there's no plates yet, not having this check will cause a crash.
        if (m_lPlates.size() > 0)
            m_lPlates.front()->OnDisabled();

        m_lPlates.push_front(plate);
        plate->OnEnabled();
    }
    void Table::PopPlate()
    {
        if (m_lPlates.size() > 0)
        {
            m_lPlates.front()->OnDisabled();
            m_lPlates.pop_front();

            if (m_lPlates.size() > 0)
                m_lPlates.front()->OnEnabled();
        }
    }
    void Table::Pop()
    {
        m_lPlates.clear();
        delete this;
    }
}