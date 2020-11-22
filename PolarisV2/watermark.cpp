#include "watermark.h"
#include "imgui_text_extension.h"
#include "build.h"

#include <string>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                Watermark::Watermark()
                {
                    m_bIsOpen = true;
                }

                void Watermark::Draw()
                {
                    ImGuiIO& io = ImGui::GetIO();

                    ImGui::SetNextWindowBgAlpha(0.45f); // Make window background invisible.

                    ImVec2 window_pos = ImVec2((CORNER & 1) ? io.DisplaySize.x - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER, (CORNER & 2) ? io.DisplaySize.y - DISTANCE_TO_CORNER : DISTANCE_TO_CORNER);
                    ImVec2 window_pos_pivot = ImVec2((CORNER & 1) ? 1.0f : 0.0f, (CORNER & 2) ? 1.0f : 0.0f);
                    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

                    // (irma) We want this to always render, thus the reinterpret cast.
                    // I don't care if this makes it crash trying to hide the window, cus it's probably
                    // a skid trying to hide it, anyway, so it's a nice anti-skid measure, I suppose.
                    ImGui::Begin("Watermark", reinterpret_cast<bool*>(true), WINDOW_FLAGS);
                    {
                        ImGui::Header2("Polaris");
                        ImGui::End();
                    }
                }
            }
        }
    }
}