#include "bot_category.h"
#include "globals.h"
#include "program.h"
#include "sdk_utils.h"

namespace polaris::ui::window::windows::mainwindow::category::categories
{
    BotCategory::BotCategory()
    {
        m_cCategoryName = "Bots";
        m_pBotCreator = new BotCreator();
    }

    void BotCategory::DrawButtons()
    {
        if (ImGui::MenuItem("Bot Creator"))
        {
            m_pBotCreator->m_bIsOpen = !m_pBotCreator->m_bIsOpen;
        }
    }
}