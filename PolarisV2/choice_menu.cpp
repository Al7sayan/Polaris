#include "choice_menu.h"
#include "frontend_plate.h"
#include "program.h"

namespace polaris::ui::window::windows
{
    void ChoiceMenu::Draw()
    {
        ImGui::Begin("Choice Menu", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        {
            if (ImGui::Button("Create"))
            {
                m_bIsOpen = false;
                tables::plates::FrontendPlate* frontendPlate = new tables::plates::FrontendPlate;;
                frontendPlate->m_bCreativeMode = true;

                gpProgram->m_pMainTable->PushPlate(frontendPlate);
            }
            if (ImGui::Button("Play"))
            {
                m_bIsOpen = false;
                gpProgram->m_pMainTable->PushPlate(new tables::plates::FrontendPlate);
            }
            ImGui::End();
        }
    }
}