#pragma once
#include "imgui.h"
#include "app.h"

namespace UI {

inline ImVec4 C(const float c[4]) {
    return ImVec4(c[0], c[1], c[2], c[3]);
}

// Call once at startup
inline void ApplyTheme() {
    ImGuiStyle& s = ImGui::GetStyle();

    // Rounding — Gumroad uses soft-medium rounding
    s.WindowRounding    = 12.0f;
    s.ChildRounding     = 10.0f;
    s.FrameRounding     = 8.0f;
    s.PopupRounding     = 8.0f;
    s.ScrollbarRounding = 6.0f;
    s.GrabRounding      = 6.0f;
    s.TabRounding       = 8.0f;
    s.WindowBorderSize  = 0.0f;
    s.ChildBorderSize   = 1.0f;
    s.FrameBorderSize   = 1.0f;
    s.PopupBorderSize   = 1.0f;

    // Padding
    s.WindowPadding     = ImVec2(24, 24);
    s.FramePadding      = ImVec2(14, 10);
    s.ItemSpacing       = ImVec2(12, 10);
    s.ItemInnerSpacing  = ImVec2(8, 6);
    s.IndentSpacing     = 20.0f;
    s.ScrollbarSize     = 10.0f;
    s.GrabMinSize       = 10.0f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg]             = C(Theme::BG);
    c[ImGuiCol_ChildBg]              = C(Theme::BG_CARD);
    c[ImGuiCol_PopupBg]              = C(Theme::BG_CARD);
    c[ImGuiCol_Border]               = C(Theme::BORDER);
    c[ImGuiCol_BorderShadow]         = ImVec4(0,0,0,0);
    c[ImGuiCol_FrameBg]              = C(Theme::BG_INPUT);
    c[ImGuiCol_FrameBgHovered]       = C(Theme::BG_HOVER);
    c[ImGuiCol_FrameBgActive]        = C(Theme::BG_HOVER);
    c[ImGuiCol_TitleBg]              = C(Theme::BG);
    c[ImGuiCol_TitleBgActive]        = C(Theme::BG);
    c[ImGuiCol_TitleBgCollapsed]     = C(Theme::BG);
    c[ImGuiCol_ScrollbarBg]          = C(Theme::BG);
    c[ImGuiCol_ScrollbarGrab]        = C(Theme::BORDER);
    c[ImGuiCol_ScrollbarGrabHovered] = C(Theme::TEXT_HINT);
    c[ImGuiCol_ScrollbarGrabActive]  = C(Theme::ACCENT);
    c[ImGuiCol_CheckMark]            = C(Theme::ACCENT);
    c[ImGuiCol_SliderGrab]           = C(Theme::ACCENT);
    c[ImGuiCol_SliderGrabActive]     = C(Theme::ACCENT_HOVER);
    c[ImGuiCol_Button]               = C(Theme::BG_CARD);
    c[ImGuiCol_ButtonHovered]        = C(Theme::BG_HOVER);
    c[ImGuiCol_ButtonActive]         = C(Theme::ACCENT_DIM);
    c[ImGuiCol_Header]               = C(Theme::ACCENT_DIM);
    c[ImGuiCol_HeaderHovered]        = C(Theme::BG_HOVER);
    c[ImGuiCol_HeaderActive]         = C(Theme::ACCENT_DIM);
    c[ImGuiCol_Tab]                  = C(Theme::BG_CARD);
    c[ImGuiCol_TabHovered]           = C(Theme::BG_HOVER);
    c[ImGuiCol_TabActive]            = C(Theme::ACCENT_DIM);
    c[ImGuiCol_TabUnfocused]         = C(Theme::BG_CARD);
    c[ImGuiCol_TabUnfocusedActive]   = C(Theme::BG_HOVER);
    c[ImGuiCol_Separator]            = C(Theme::BORDER);
    c[ImGuiCol_SeparatorHovered]     = C(Theme::ACCENT);
    c[ImGuiCol_SeparatorActive]      = C(Theme::ACCENT);
    c[ImGuiCol_ResizeGrip]           = C(Theme::BORDER);
    c[ImGuiCol_ResizeGripHovered]    = C(Theme::ACCENT);
    c[ImGuiCol_ResizeGripActive]     = C(Theme::ACCENT);
    c[ImGuiCol_PlotLines]            = C(Theme::ACCENT);
    c[ImGuiCol_PlotLinesHovered]     = C(Theme::ACCENT_HOVER);
    c[ImGuiCol_PlotHistogram]        = C(Theme::ACCENT);
    c[ImGuiCol_PlotHistogramHovered] = C(Theme::ACCENT_HOVER);
    c[ImGuiCol_TableHeaderBg]        = C(Theme::BG_CARD);
    c[ImGuiCol_TableBorderStrong]    = C(Theme::BORDER);
    c[ImGuiCol_TableBorderLight]     = C(Theme::BG_HOVER);
    c[ImGuiCol_TableRowBg]           = ImVec4(0,0,0,0);
    c[ImGuiCol_TableRowBgAlt]        = ImVec4(1,1,1,0.03f);
    c[ImGuiCol_TextSelectedBg]       = C(Theme::ACCENT_DIM);
    c[ImGuiCol_DragDropTarget]       = C(Theme::ACCENT);
    c[ImGuiCol_NavHighlight]         = C(Theme::ACCENT);
    c[ImGuiCol_Text]                 = C(Theme::TEXT);
    c[ImGuiCol_TextDisabled]         = C(Theme::TEXT_HINT);
}

// ── Helpers ───────────────────────────────────────────────────────────────────

// Coral accent button
inline bool AccentButton(const char* label, ImVec2 size = {0, 0}) {
    ImGui::PushStyleColor(ImGuiCol_Button,        C(Theme::ACCENT));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, C(Theme::ACCENT_HOVER));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  C(Theme::ACCENT));
    ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(0.05f,0.05f,0.05f,1.0f));
    bool r = ImGui::Button(label, size);
    ImGui::PopStyleColor(4);
    return r;
}

// Ghost / outline button
inline bool GhostButton(const char* label, ImVec2 size = {0, 0}) {
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, C(Theme::BG_HOVER));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  C(Theme::BG_HOVER));
    ImGui::PushStyleColor(ImGuiCol_Border,        C(Theme::BORDER));
    bool r = ImGui::Button(label, size);
    ImGui::PopStyleColor(4);
    return r;
}

// Dim text helper
inline void TextDim(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::PushStyleColor(ImGuiCol_Text, C(Theme::TEXT_DIM));
    ImGui::TextV(fmt, args);
    ImGui::PopStyleColor();
    va_end(args);
}

// Hint text
inline void TextHint(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::PushStyleColor(ImGuiCol_Text, C(Theme::TEXT_HINT));
    ImGui::TextV(fmt, args);
    ImGui::PopStyleColor();
    va_end(args);
}

// Accent-colored text
inline void TextAccent(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::PushStyleColor(ImGuiCol_Text, C(Theme::ACCENT));
    ImGui::TextV(fmt, args);
    ImGui::PopStyleColor();
    va_end(args);
}

// Horizontal rule
inline void HR(float alpha = 1.0f) {
    ImGui::PushStyleColor(ImGuiCol_Separator,
        ImVec4(Theme::BORDER[0], Theme::BORDER[1], Theme::BORDER[2], alpha));
    ImGui::Separator();
    ImGui::PopStyleColor();
}

// Badge tag label
inline void Badge(const char* text, const float col[4]) {
    ImVec2 p  = ImGui::GetCursorScreenPos();
    ImVec2 ts = ImGui::CalcTextSize(text);
    float  px = 8.0f, py = 3.0f;
    ImVec2 bmin = p;
    ImVec2 bmax = ImVec2(p.x + ts.x + px * 2, p.y + ts.y + py * 2);
    ImGui::GetWindowDrawList()->AddRectFilled(bmin, bmax,
        ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0.15f)), 4.0f);
    ImGui::GetWindowDrawList()->AddRect(bmin, bmax,
        ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0.40f)), 4.0f);
    ImGui::SetCursorScreenPos(ImVec2(p.x + px, p.y + py));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(col[0], col[1], col[2], 1.0f));
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    ImGui::SetCursorScreenPos(ImVec2(p.x, bmax.y + 4));
}

// Stat result card
inline void StatCard(const char* label, const char* value, const char* unit,
                     const float color[4] = nullptr) {
    float w = 160.0f, h = 74.0f;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, C(Theme::BG));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
    ImGui::BeginChild(label, ImVec2(w, h), true);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
    TextHint("%s", label);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.Size > 1
                    ? ImGui::GetIO().Fonts->Fonts[1] : nullptr);
    if (color) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color[0],color[1],color[2],1));
    ImGui::Text("%s", value);
    if (color) ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
    TextDim(" %s", unit);
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

} // namespace UI
