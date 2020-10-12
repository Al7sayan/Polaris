#include "main_window.h"

namespace polaris
{
    void MainWindow::Draw()
    {
		ImGui::Begin("Polaris", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar);
		
		ImGuiIO& io = ImGui::GetIO();
		ImFontAtlas* atlas = io.Fonts;

		ImFont* headerFont = atlas->Fonts[2];
		ImFont* headerntFont = atlas->Fonts[3];
		ImFont* smallFont = atlas->Fonts[1];
		ImGui::PushFont(headerFont);
		ImGui::Text("Polaris");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.3f), "v2");
		ImGui::PopFont();

		ImGui::PushFont(smallFont);
		ImGui::Text("They said to think back, but I think we went a bit too far.");
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(1, 10));

		ImGui::PushFont(headerntFont);
		ImGui::Text("Creator");
		ImGui::PopFont();
		ImGui::Text("Cyuubi");

		ImGui::Dummy(ImVec2(1, 5));

		ImGui::PushFont(headerntFont);
		ImGui::Text("Developer");
		ImGui::PopFont();
		ImGui::Text("irma");

		ImGui::Dummy(ImVec2(1, 5));

		ImGui::PushFont(headerntFont);
		ImGui::Text("Contributor");
		ImGui::PopFont();
		ImGui::Text("kanner");
		ImGui::Text("pivotman319");
		ImGui::Text("wiktorwiktor12");

		ImGui::Dummy(ImVec2(1, 5));

		ImGui::PushFont(headerntFont);
		ImGui::Text("Special Thanks");
		ImGui::PopFont();
		ImGui::Text("BattleDash");
		ImGui::Text("Crush");
		ImGui::Text("Darkblade");
		ImGui::Text("Chase");
		ImGui::Text("Samicc");
		ImGui::Text("Slushia");

		ImGui::End();
    }
}