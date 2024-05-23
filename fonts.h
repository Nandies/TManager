#pragma once
#include "imgui.h"
#include "imgui_internal.h"

void LoadCustomFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("./fonts/GaruteRegular.ttf", 16.0f); // Update path to the font file
    io.Fonts->AddFontDefault();
}
