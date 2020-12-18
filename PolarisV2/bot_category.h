#ifndef CREATIVE_CATEGORY_H
#define CREATIVE_CATEGORY_H

#include "category.h"
#include "bot_creator.h"

namespace polaris::ui::window::windows::mainwindow::category::categories
{
    class BotCategory : public Category
    {
    public:
        BotCreator* m_pBotCreator;

        BotCategory();

        void DrawButtons() override;
    };
}

#endif // !CREATIVE_CATEGORY_H
