#include "main_window.h"
#include "program.h"
#include "sdk_utils.h"
#include "globals.h"
#include "ui_renderer.h"
#include "changelog_window.h"

namespace polaris::ui::window::windows
{
    MainWindow::MainWindow()
    {
        m_pAboutWindow = new AboutWindow;
        m_pWorldInspector = new WorldInspector;
        m_pWatermark = new Watermark;
        //new ChangelogWindow;

        m_bIsUE4ConsoleEnabled = false;
        m_pAboutWindow->m_bIsOpen = false;
        m_pWorldInspector->m_bIsOpen = false;
    }

    void MainWindow::Draw()
    {
        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("Player"))
            {
                if (!globals::gpLocalPlayer->ViewportClient->ViewportConsole || !m_bIsUE4ConsoleEnabled)
                {
                    if (ImGui::MenuItem("Enable Console"))
                    {
                        utilities::SDKUtils::ToggleUnrealConsole(true);
                        m_bIsUE4ConsoleEnabled = true;
                    }
                }
                else if (globals::gpLocalPlayer->ViewportClient->ViewportConsole || m_bIsUE4ConsoleEnabled)
                {
                    if (ImGui::MenuItem("Disable Console"))
                    {
                        utilities::SDKUtils::ToggleUnrealConsole(false);
                        m_bIsUE4ConsoleEnabled = false;
                    }
                }

                if (ImGui::MenuItem("Exit"))
                {
                    gpProgram->~Program();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("World Inspector"))
                {
                    m_pWorldInspector->m_bIsOpen = !m_pWorldInspector->m_bIsOpen;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About"))
                {
                    m_pAboutWindow->m_bIsOpen = !m_pAboutWindow->m_bIsOpen;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
    void MainWindow::Update()
    {
        m_bIsOpen = gpRenderer->m_bLockFortInput;
    }
}