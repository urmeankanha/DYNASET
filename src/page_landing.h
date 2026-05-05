#pragma once
#include "imgui.h"
#include "app.h"
#include "ui_theme.h"
#include <cmath>
#include <cstring>
#include <cstdio>

namespace Pages {

// Draw an animated decorative background grid/dots
static void DrawBgDecoration(ImVec2 win_pos, ImVec2 win_size, float t) {
    auto* dl = ImGui::GetBackgroundDrawList();
    ImU32 dot_col = IM_COL32(255, 84, 73, 22);

    float spacing = 38.0f;
    for (float x = 0; x < win_size.x + spacing; x += spacing) {
        for (float y = 0; y < win_size.y + spacing; y += spacing) {
            float pulse = 0.5f + 0.5f * sinf(t * 0.6f + x * 0.04f + y * 0.04f);
            float r = 1.2f + pulse * 0.8f;
            dl->AddCircleFilled(
                ImVec2(win_pos.x + x, win_pos.y + y),
                r, dot_col
            );
        }
    }

    // Large radial glow top-right
    ImVec2 gc = ImVec2(win_pos.x + win_size.x * 0.82f, win_pos.y - 80.0f);
    for (int i = 5; i >= 0; i--) {
        float gi = (float)i / 5.0f;
        float pr = 260.0f * (1.0f - gi * 0.6f);
        float ga = 0.06f * (1.0f - gi) * (0.85f + 0.15f * sinf(t * 0.4f));
        dl->AddCircleFilled(gc, pr, IM_COL32(255, 84, 73, (int)(ga * 255)));
    }
}

// Single calculator card
static bool DrawCalcCard(const CalculatorInfo& info, int idx, int& hovered,
                         ImVec2 card_size, float anim_t) {
    bool clicked = false;
    ImGui::PushID(idx);

    ImVec2 cp = ImGui::GetCursorPos();
    ImVec2 cs = ImGui::GetCursorScreenPos();
    bool is_hov = (hovered == idx);

    // Animated border glow on hover
    float hov_blend = is_hov ? 1.0f : 0.0f;
    ImVec4 border_col = ImVec4(
        Theme::BORDER[0] + hov_blend * (Theme::ACCENT[0] - Theme::BORDER[0]) * 0.7f,
        Theme::BORDER[1] + hov_blend * (Theme::ACCENT[1] - Theme::BORDER[1]) * 0.7f,
        Theme::BORDER[2] + hov_blend * (Theme::ACCENT[2] - Theme::BORDER[2]) * 0.7f,
        1.0f
    );

    ImGui::PushStyleColor(ImGuiCol_ChildBg,
        is_hov ? UI::C(Theme::BG_HOVER) : UI::C(Theme::BG_CARD));
    ImGui::PushStyleColor(ImGuiCol_Border, border_col);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 14.0f);

    // Slight Y offset on hover for lift effect
    if (is_hov) {
        ImGui::SetCursorPos(ImVec2(cp.x, cp.y - 3.0f));
        cs.y -= 3.0f;
    }

    ImGui::BeginChild(info.id.c_str(), card_size, true,
                      ImGuiWindowFlags_NoScrollbar);

    // NEW badge
    if (info.is_new) {
        ImGui::SetCursorPos(ImVec2(card_size.x - 58, 16));
        UI::Badge("NEW", Theme::ACCENT);
        ImGui::SetCursorPos(ImVec2(16, 16));
    }

    // Tag badge top-left
    ImGui::SetCursorPos(ImVec2(18, 18));
    UI::Badge(info.tag.c_str(), Theme::ACCENT);

    // Big icon area
    float icon_y = 52.0f;
    ImGui::SetCursorPos(ImVec2(18, icon_y));
    ImGui::PushStyleColor(ImGuiCol_Text,
        is_hov ? UI::C(Theme::ACCENT) : ImVec4(0.7f, 0.7f, 0.72f, 1.0f));

    // Draw a custom physics-ish icon via drawlist
    ImVec2 icon_screen = ImGui::GetCursorScreenPos();
    auto* dl = ImGui::GetWindowDrawList();

    if (info.id == "spring_damping") {
        // Draw a little spring icon
        ImVec2 base = ImVec2(icon_screen.x + 4, icon_screen.y + 12);
        ImU32 ic = is_hov
            ? IM_COL32(255, 84, 73, 255)
            : IM_COL32(160, 155, 150, 220);

        // Vertical line top
        dl->AddLine(ImVec2(base.x + 14, base.y - 10),
                    ImVec2(base.x + 14, base.y), ic, 2.0f);
        // Spring coils
        int coils = 5;
        for (int i = 0; i < coils; i++) {
            float y0 = base.y + i * 7.0f;
            float y1 = base.y + (i + 1) * 7.0f;
            float xL = base.x + 4,  xR = base.x + 24;
            dl->AddLine(ImVec2(i % 2 == 0 ? xL : xR, y0),
                        ImVec2(i % 2 == 0 ? xR : xL, y1), ic, 2.0f);
        }
        // Vertical line bottom
        dl->AddLine(ImVec2(base.x + 14, base.y + coils * 7.0f),
                    ImVec2(base.x + 14, base.y + coils * 7.0f + 10), ic, 2.0f);
        // Mass block
        float my = base.y + coils * 7.0f + 10;
        dl->AddRectFilled(ImVec2(base.x + 4, my),
                          ImVec2(base.x + 24, my + 14), ic, 3.0f);
    }

    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(40, 90)); // space for icon (spring is ~80px tall)

    // Title
    ImGui::SetCursorPos(ImVec2(18, icon_y + 96));
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT));
    ImGui::TextUnformatted(info.name.c_str());
    ImGui::PopStyleColor();

    // Description
    ImGui::SetCursorPos(ImVec2(18, icon_y + 116));
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT_DIM));
    ImGui::PushTextWrapPos(card_size.x - 18);
    ImGui::TextUnformatted(info.description.c_str());
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();

    // Open button at bottom
    float btn_y = card_size.y - 52.0f;
    ImGui::SetCursorPos(ImVec2(18, btn_y));
    if (is_hov) {
        if (UI::AccentButton("Open Calculator", ImVec2(card_size.x - 36, 34))) {
            clicked = true;
        }
    } else {
        UI::GhostButton("Open Calculator", ImVec2(card_size.x - 36, 34));
    }

    // Detect hover via invisible overlay
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::InvisibleButton("##hover", card_size);
    if (ImGui::IsItemHovered()) {
        hovered = idx;
        if (ImGui::IsMouseDoubleClicked(0)) clicked = true;
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
    ImGui::PopID();
    return clicked;
}

// ── Main Landing Page ─────────────────────────────────────────────────────────
inline void DrawLanding(AppState& app) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 ws   = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ws);
    ImGui::Begin("##landing", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse);

    DrawBgDecoration(ImGui::GetWindowPos(), ws, app.anim_t);

    // ── Header Bar ───────────────────────────────────────────────────────────
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        auto* dl = ImGui::GetWindowDrawList();
        // Subtle top bar line
        dl->AddRectFilled(p, ImVec2(p.x + ws.x, p.y + 1),
            IM_COL32(255,84,73,60));
    }

    float pad = std::max(40.0f, (ws.x - 900.0f) * 0.5f);
    ImGui::SetCursorPos(ImVec2(pad, 36));

    // Logo / wordmark
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::ACCENT));
    ImGui::Text("DEVTOOLS");
    ImGui::PopStyleColor();
    ImGui::SameLine(0, 6);
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT_HINT));
    ImGui::Text("/ CALCULATOR SUITE");
    ImGui::PopStyleColor();

    // Version tag top-right
    ImGui::SameLine(ws.x - pad - 80);
    UI::TextHint("v0.1.0");

    // ── Hero Section ─────────────────────────────────────────────────────────
    float hero_y = 90.0f;
    ImGui::SetCursorPos(ImVec2(pad, hero_y));

    // Large headline
    float headline_size_mult = ws.x > 1000 ? 1.0f : 0.78f;
    (void)headline_size_mult;

    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT));
    ImGui::SetWindowFontScale(2.4f);
    ImGui::Text("Game Dev");
    ImGui::SetCursorPos(ImVec2(pad, hero_y + 52));
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::ACCENT));
    ImGui::Text("Calculators.");
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPos(ImVec2(pad, hero_y + 112));
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT_DIM));
    ImGui::PushTextWrapPos(pad + 480);
    ImGui::TextUnformatted(
        "Precision tools for game physics, audio, and math.\n"
        "Click a calculator below to get started."
    );
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();

    // Decorative number stat row
    float stat_y = hero_y + 170;
    struct { const char* n; const char* l; } stats[] = {
        {"01", "Calculators"}, {"inf", "Precision"}, {"0ms", "Lag"}
    };
    for (int i = 0; i < 3; i++) {
        ImGui::SetCursorPos(ImVec2(pad + i * 140.0f, stat_y));
        ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::ACCENT));
        ImGui::SetWindowFontScale(1.6f);
        ImGui::TextUnformatted(stats[i].n);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::SetCursorPos(ImVec2(pad + i * 140.0f, stat_y + 30));
        UI::TextHint("%s", stats[i].l);
    }

    // ── Divider ───────────────────────────────────────────────────────────────
    float grid_y = stat_y + 72;
    ImGui::SetCursorPos(ImVec2(pad, grid_y));
    {
        ImVec2 lp = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddLine(
            lp, ImVec2(lp.x + ws.x - pad * 2, lp.y),
            IM_COL32(50,49,56,255), 1.0f);
    }
    ImGui::Dummy(ImVec2(0, 12));
    ImGui::SetCursorPos(ImVec2(pad, grid_y + 16));
    UI::TextHint("ALL CALCULATORS  (%d)", (int)GetCalculatorRegistry().size());
    ImGui::SetCursorPos(ImVec2(pad, grid_y + 36));
    ImGui::Dummy(ImVec2(0, 6));

    // ── Calculator Grid ───────────────────────────────────────────────────────
    auto calcs = GetCalculatorRegistry();
    float card_w = 280.0f, card_h = 340.0f, gap = 20.0f;
    float grid_start_y = grid_y + 52;

    int cols = std::max(1, (int)((ws.x - pad * 2 + gap) / (card_w + gap)));

    for (int i = 0; i < (int)calcs.size(); i++) {
        int col = i % cols;
        int row = i / cols;
        ImGui::SetCursorPos(ImVec2(
            pad + col * (card_w + gap),
            grid_start_y + row * (card_h + gap)
        ));
        if (DrawCalcCard(calcs[i], i, app.hovered_card,
                         ImVec2(card_w, card_h), app.anim_t)) {
            // Navigate to the selected calculator
            if (calcs[i].id == "spring_damping")
                app.current_page = AppPage::SPRING_DAMPING;
        }
    }

    // Reset hover if mouse not near any card
    if (!ImGui::IsAnyItemHovered()) {
        // keep last hover for a frame for animations
    }

    ImGui::End();
}

} // namespace Pages
