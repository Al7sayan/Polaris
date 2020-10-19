#include "main_window.h"
#include "program.h"
#include "ui_renderer.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
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
                                m_pAboutWindow->m_bIsOpen = !m_pAboutWindow->m_bIsOpen;
                            }
                            ImGui::EndMenu();
                        }
                    }
                }
                void MainWindow::Update()
                {
                    m_bIsOpen = gpRenderer->m_bLockFortInput;
                }
            }
        }
    }
}