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
                    m_pWorldInspector = new WorldInspector;
                    m_pWatermark = new Watermark;

                    m_pAboutWindow->m_bIsOpen = false;
                    m_pWorldInspector->m_bIsOpen = false;
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