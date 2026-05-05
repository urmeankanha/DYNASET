#pragma once
#include "imgui.h"
#include "app.h"
#include "ui_theme.h"
#include "spring_calc.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

namespace Pages {

// ── Mini step-response graph ──────────────────────────────────────────────────
static void DrawStepGraph(const SpringResult& r, ImVec2 size) {
    if (!r.valid) return;

    auto curve = SpringCalc::StepResponse(r, 180);
    if (curve.empty()) return;

    ImVec2 origin = ImGui::GetCursorScreenPos();
    auto*  dl     = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##graph", size);

    float pad = 36.0f;
    ImVec2 plot_min = ImVec2(origin.x + pad, origin.y + 12);
    ImVec2 plot_max = ImVec2(origin.x + size.x - 12, origin.y + size.y - 28);
    float  pw = plot_max.x - plot_min.x;
    float  ph = plot_max.y - plot_min.y;

    // Background
    dl->AddRectFilled(origin, ImVec2(origin.x + size.x, origin.y + size.y),
        IM_COL32(13, 13, 15, 255), 10.0f);
    dl->AddRect(origin, ImVec2(origin.x + size.x, origin.y + size.y),
        IM_COL32(50, 49, 56, 255), 10.0f);

    // Y axis grid lines and labels
    float y_vals[] = {0.0f, 0.5f, 1.0f, 1.25f};
    for (float yv : y_vals) {
        float sy = plot_max.y - yv * ph * 0.8f; // 0..1.25 mapped to 0..ph
        if (sy < plot_min.y - 4 || sy > plot_max.y + 4) continue;
        dl->AddLine(ImVec2(plot_min.x, sy), ImVec2(plot_max.x, sy),
            IM_COL32(55, 54, 60, 200), 1.0f);
        char buf[12]; snprintf(buf, sizeof(buf), "%.2f", yv);
        dl->AddText(ImVec2(origin.x + 2, sy - 7), IM_COL32(90,88,96,255), buf);
    }

    // Steady-state reference line at y=1
    float ss_y = plot_max.y - 1.0f * ph * 0.8f;
    dl->AddLine(ImVec2(plot_min.x, ss_y), ImVec2(plot_max.x, ss_y),
        IM_COL32(255, 84, 73, 60), 1.0f);

    // Curve: find y range
    float y_max = 0.0f;
    for (auto& p : curve) y_max = std::max(y_max, p.y);
    y_max = std::max(y_max, 1.1f);
    float t_max = curve.back().t;

    // Smoothed polyline
    std::vector<ImVec2> pts;
    pts.reserve(curve.size());
    for (auto& p : curve) {
        float sx = plot_min.x + (p.t / t_max) * pw;
        float sy = plot_max.y - (p.y / y_max) * ph * 0.8f;
        pts.push_back(ImVec2(sx, sy));
    }

    // Shadow/glow
    for (int g = 3; g >= 1; g--) {
        for (int i = 1; i < (int)pts.size(); i++) {
            dl->AddLine(pts[i-1], pts[i],
                IM_COL32(255, 84, 73, 18 / g), (float)(g * 3));
        }
    }
    // Main line
    for (int i = 1; i < (int)pts.size(); i++) {
        dl->AddLine(pts[i-1], pts[i], IM_COL32(255, 84, 73, 230), 2.0f);
    }

    // Axes labels
    dl->AddText(ImVec2(plot_min.x, plot_max.y + 6),
        IM_COL32(90,88,96,255), "0");
    char tbuf[24]; snprintf(tbuf, sizeof(tbuf), "t = %.2fs", t_max);
    dl->AddText(ImVec2(plot_max.x - 60, plot_max.y + 6),
        IM_COL32(90,88,96,255), tbuf);
    dl->AddText(ImVec2(plot_min.x - 4, plot_min.y - 14),
        IM_COL32(90,88,96,255), "x(t)");
}

// ── Main Spring Damping Page ──────────────────────────────────────────────────
inline void DrawSpringDamping(AppState& app) {
    SpringState& ss = app.spring;
    ImGuiIO& io     = ImGui::GetIO();
    ImVec2 ws       = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ws);
    ImGui::Begin("##spring", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove);

    float pad = std::max(32.0f, (ws.x - 960.0f) * 0.5f);

    // ── Top Nav Bar ───────────────────────────────────────────────────────────
    ImGui::SetCursorPos(ImVec2(pad, 28));
    if (UI::GhostButton("< Back")) {
        app.current_page = AppPage::LANDING;
    }
    ImGui::SameLine(0, 14);
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT_HINT));
    ImGui::TextUnformatted("DEVTOOLS  /");
    ImGui::PopStyleColor();
    ImGui::SameLine(0, 6);
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT));
    ImGui::TextUnformatted("Spring & Damping");
    ImGui::PopStyleColor();

    // ── Page Title ────────────────────────────────────────────────────────────
    float title_y = 72;
    ImGui::SetCursorPos(ImVec2(pad, title_y));
    ImGui::SetWindowFontScale(1.9f);
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT));
    ImGui::TextUnformatted("Spring Damping");
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPos(ImVec2(pad + 3, title_y + 42));
    UI::TextDim("Compute damping ratio, natural frequency, settling time & step response.");

    // Separator
    ImGui::SetCursorPos(ImVec2(pad, title_y + 68));
    {
        ImVec2 lp = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddLine(lp,
            ImVec2(lp.x + ws.x - pad * 2, lp.y),
            IM_COL32(50,49,56,255), 1.0f);
    }
    ImGui::Dummy(ImVec2(0, 10));

    // ── Two-column layout ─────────────────────────────────────────────────────
    float content_y = title_y + 86;
    float col1_w    = 340.0f;
    float col2_w    = ws.x - pad * 2 - col1_w - 24;
    float col_h     = ws.y - content_y - 28;

    // ── LEFT PANEL: Inputs ────────────────────────────────────────────────────
    ImGui::SetCursorPos(ImVec2(pad, content_y));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::C(Theme::BG_CARD));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 14.0f);
    ImGui::BeginChild("##inputs", ImVec2(col1_w, col_h), true);

    // Mode selector tabs
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
    if (ImGui::BeginTabBar("##modes")) {
        if (ImGui::BeginTabItem("M / K / C")) {
            ss.input_mode = 0; ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("\xce\xb6 / \xcf\x89n")) {
            ss.input_mode = 1; ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Overshoot")) {
            ss.input_mode = 2; ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::PopStyleVar();
    ImGui::Dummy(ImVec2(0, 8));

    bool changed = false;
    float input_w = col1_w - 48;

    if (ss.input_mode == 0) {
        UI::TextHint("MASS  (kg)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##mass", &ss.mass, 0.01f, 0.001f, 1000.0f, "%.3f kg");

        ImGui::Dummy(ImVec2(0,6));
        UI::TextHint("SPRING CONSTANT  (N/m)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##k", &ss.spring_k, 0.5f, 0.001f, 100000.0f, "%.2f N/m");

        ImGui::Dummy(ImVec2(0,6));
        UI::TextHint("DAMPER COEFFICIENT  (N\xc2\xb7s/m)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##c", &ss.damper_c, 0.1f, 0.0f, 100000.0f, "%.3f N\xc2\xb7s/m");

        // Live slider for damper
        ImGui::Dummy(ImVec2(0,4));
        UI::TextHint("QUICK-TUNE DAMPER");
        float cc = 2.0f * sqrtf(ss.spring_k * ss.mass);
        float c_max = cc * 3.0f;
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::SliderFloat("##cslide", &ss.damper_c, 0.0f, c_max > 0 ? c_max : 100.0f);
    }
    else if (ss.input_mode == 1) {
        UI::TextHint("DAMPING RATIO  \xce\xb6");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##zeta", &ss.zeta_direct, 0.005f, 0.0f, 5.0f, "%.4f");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::SliderFloat("##zetaslide", &ss.zeta_direct, 0.0f, 3.0f);

        ImGui::Dummy(ImVec2(0,6));
        UI::TextHint("NATURAL FREQUENCY  \xcf\x89n  (rad/s)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##wn", &ss.wn_direct, 0.1f, 0.001f, 1000.0f, "%.3f rad/s");
    }
    else {
        UI::TextHint("TARGET OVERSHOOT  (%)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##os", &ss.target_overshoot,
            0.1f, 0.0f, 100.0f, "%.2f %%");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::SliderFloat("##osslide", &ss.target_overshoot, 0.0f, 60.0f);

        ImGui::Dummy(ImVec2(0,6));
        UI::TextHint("NATURAL FREQUENCY  \xcf\x89n  (rad/s)");
        ImGui::SetNextItemWidth(input_w);
        changed |= ImGui::DragFloat("##wn2", &ss.wn_direct, 0.1f, 0.001f, 1000.0f, "%.3f rad/s");
    }

    // Recompute on any input change (also always recompute first frame)
    static bool first = true;
    if (changed || first) {
        ss.result = SpringCalc::Compute(ss);
        first = false;
    }

    // ── Regime pill ──────────────────────────────────────────────────────────
    ImGui::Dummy(ImVec2(0, 12));
    UI::HR(0.5f);
    ImGui::Dummy(ImVec2(0, 6));

    if (ss.result.valid) {
        const float* regime_col =
            (ss.result.damping_ratio < 0.99) ? Theme::ACCENT :
            (ss.result.damping_ratio < 1.01) ? Theme::SUCCESS :
                                               Theme::WARNING;
        UI::Badge(ss.result.regime.c_str(), regime_col);
        ImGui::Dummy(ImVec2(0, 4));
        if (ss.result.damping_ratio < 0.99) {
            UI::TextHint("System will oscillate before settling.");
        } else if (ss.result.damping_ratio < 1.01) {
            UI::TextHint("Fastest response without oscillation.");
        } else {
            UI::TextHint("System returns to equilibrium slowly.");
        }
    }

    ImGui::Dummy(ImVec2(0, 8));
    UI::HR(0.4f);
    ImGui::Dummy(ImVec2(0, 6));
    ImGui::PushStyleColor(ImGuiCol_Text, UI::C(Theme::TEXT_HINT));
    ImGui::PushTextWrapPos(col1_w - 16);
    ImGui::TextUnformatted(
        "Drag values to adjust, or use the slider for quick tuning. "
        "Results update in real time."
    );
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    // ── RIGHT PANEL: Results + Graph ──────────────────────────────────────────
    ImGui::SetCursorPos(ImVec2(pad + col1_w + 24, content_y));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::C(Theme::BG_CARD));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 14.0f);
    ImGui::BeginChild("##results", ImVec2(col2_w, col_h), true);

    if (!ss.result.valid) {
        ImGui::SetCursorPos(ImVec2(20, col_h * 0.4f));
        UI::TextDim("Enter valid inputs to see results.");
    } else {
        // ── Output stats row ─────────────────────────────────────────────────
        UI::TextHint("RESULTS");
        ImGui::Dummy(ImVec2(0, 6));

        // Stat boxes
        struct Stat { const char* label; char val[32]; const char* unit; const float* col; };
        char buf_z[32], buf_wn[32], buf_wd[32], buf_st[32], buf_os[32];
        snprintf(buf_z,  sizeof(buf_z),  "%.4f", ss.result.damping_ratio);
        snprintf(buf_wn, sizeof(buf_wn), "%.3f", ss.result.natural_freq);
        snprintf(buf_wd, sizeof(buf_wd), "%.3f", ss.result.damped_freq);
        snprintf(buf_st, sizeof(buf_st), "%.4f", ss.result.settling_time);
        snprintf(buf_os, sizeof(buf_os), "%.2f", ss.result.overshoot_pct);

        // Stat grid — BeginGroup/EndGroup keeps rows from cascading
        float sw = (col2_w - 48 - 2 * 10) / 3.0f;
        sw = std::max(sw, 130.0f);
        float bh = 72.0f;

        auto DrawStatBox = [&](const char* lbl, const char* val, const char* unit,
                                const float* col) {
            ImGui::BeginGroup();
            ImVec2 p  = ImGui::GetCursorScreenPos();
            auto*  dl = ImGui::GetWindowDrawList();
            float bw  = sw;
            dl->AddRectFilled(p, ImVec2(p.x + bw, p.y + bh),
                IM_COL32(13,13,15,255), 8.0f);
            dl->AddRect(p, ImVec2(p.x + bw, p.y + bh),
                IM_COL32(50,49,56,255), 8.0f);
            ImGui::SetCursorScreenPos(ImVec2(p.x + 10, p.y + 8));
            UI::TextHint("%s", lbl);
            ImGui::SetCursorScreenPos(ImVec2(p.x + 10, p.y + 26));
            ImGui::PushStyleColor(ImGuiCol_Text,
                col ? ImVec4(col[0],col[1],col[2],1.0f) : UI::C(Theme::TEXT));
            ImGui::SetWindowFontScale(1.35f);
            ImGui::TextUnformatted(val);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
            ImGui::SetCursorScreenPos(ImVec2(p.x + 10, p.y + 52));
            UI::TextHint("%s", unit);
            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + bh));
            ImGui::Dummy(ImVec2(bw, 0));
            ImGui::EndGroup();
        };

        // Row 1
        DrawStatBox("Damping Ratio z",  buf_z,  "dimensionless", Theme::ACCENT);
        ImGui::SameLine(0, 10);
        DrawStatBox("Natural Freq wn",  buf_wn, "rad/s",         Theme::SUCCESS);
        ImGui::SameLine(0, 10);
        DrawStatBox("Damped Freq wd",   buf_wd, "rad/s",         nullptr);
        ImGui::Dummy(ImVec2(0, 8));
        // Row 2
        DrawStatBox("Settling Time 2%", buf_st, "seconds",       Theme::WARNING);
        ImGui::SameLine(0, 10);
        DrawStatBox("Peak Overshoot",   buf_os, "%",             nullptr);
        ImGui::Dummy(ImVec2(0, 16));

        UI::HR(0.4f);
        ImGui::Dummy(ImVec2(0, 10));

        // ── Step response graph ───────────────────────────────────────────────
        UI::TextHint("STEP RESPONSE");
        ImGui::Dummy(ImVec2(0, 8));

        float graph_w = col2_w - 48;
        float graph_h = col_h - 260;
        graph_h = std::max(graph_h, 180.0f);

        DrawStepGraph(ss.result, ImVec2(graph_w, graph_h));
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::End();
}

} // namespace Pages
