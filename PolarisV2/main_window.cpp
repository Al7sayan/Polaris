#include "main_window.h"
#include "program.h"

namespace polaris
{
    MainWindow::MainWindow()
    {
        m_pAboutWindow = new AboutWindow;
    }

    void MainWindow::Draw()
    {
        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("Player"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    gpProgram->~Program();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                {
                    m_pAboutWindow->m_bIsOpen = true;
                }
                ImGui::EndMenu();
            }
        }
    }
}