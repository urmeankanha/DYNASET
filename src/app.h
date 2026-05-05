#pragma once
#include <string>
#include <vector>
#include <functional>
#include <cmath>

// ─── Color Palette (Gumroad-inspired, dark editorial) ────────────────────────
// Deep charcoal bg, warm cream text, hot coral accent, subtle amber glow
namespace Theme {
    // Main palette
    constexpr float BG[4]           = { 0.082f, 0.082f, 0.090f, 1.00f }; // #151517
    constexpr float BG_CARD[4]      = { 0.112f, 0.112f, 0.122f, 1.00f }; // #1C1C1F
    constexpr float BG_INPUT[4]     = { 0.075f, 0.075f, 0.082f, 1.00f }; // #131315
    constexpr float BG_HOVER[4]     = { 0.155f, 0.155f, 0.168f, 1.00f }; // #282A2A
    constexpr float ACCENT[4]       = { 1.000f, 0.329f, 0.286f, 1.00f }; // #FF5449 coral
    constexpr float ACCENT_DIM[4]   = { 1.000f, 0.329f, 0.286f, 0.18f };
    constexpr float ACCENT_HOVER[4] = { 1.000f, 0.420f, 0.380f, 1.00f };
    constexpr float TEXT[4]         = { 0.945f, 0.933f, 0.914f, 1.00f }; // #F1EEE9
    constexpr float TEXT_DIM[4]     = { 0.530f, 0.520f, 0.510f, 1.00f };
    constexpr float TEXT_HINT[4]    = { 0.340f, 0.335f, 0.325f, 1.00f };
    constexpr float BORDER[4]       = { 0.200f, 0.198f, 0.210f, 1.00f };
    constexpr float BORDER_ACTIVE[4]= { 1.000f, 0.329f, 0.286f, 0.60f };
    constexpr float SUCCESS[4]      = { 0.278f, 0.875f, 0.584f, 1.00f }; // mint green
    constexpr float WARNING[4]      = { 1.000f, 0.780f, 0.300f, 1.00f }; // amber
    constexpr float OVERLAY[4]      = { 0.000f, 0.000f, 0.000f, 0.65f };
}

// ─── Calculator Descriptor ───────────────────────────────────────────────────
struct CalculatorInfo {
    std::string id;
    std::string name;
    std::string description;
    std::string icon;          // unicode symbol used as icon
    std::string tag;           // e.g. "PHYSICS", "AUDIO"
    bool        is_new = false;
};

// ─── App Pages ────────────────────────────────────────────────────────────────
enum class AppPage {
    LANDING,
    SPRING_DAMPING,
    // ADD NEW CALCULATORS HERE
};

// ─── Spring Damping Calculator State ─────────────────────────────────────────
struct SpringResult {
    double damping_ratio   = 0.0;
    double natural_freq    = 0.0;  // rad/s
    double damped_freq     = 0.0;  // rad/s
    double settling_time   = 0.0;  // seconds (2% criterion)
    double overshoot_pct   = 0.0;  // percent
    std::string regime;            // "Underdamped", "Critically Damped", "Overdamped"
    bool valid = false;
};

struct SpringState {
    // Inputs (mode A: mass/spring/damper)
    float mass          = 1.0f;    // kg
    float spring_k      = 50.0f;  // N/m
    float damper_c      = 5.0f;   // N·s/m

    // Inputs (mode B: zeta + wn direct)
    float zeta_direct   = 0.7f;
    float wn_direct     = 10.0f;  // rad/s

    // Inputs (mode C: from overshoot target)
    float target_overshoot = 5.0f; // percent

    int   input_mode    = 0;       // 0=MKC, 1=ζωn, 2=Overshoot
    SpringResult result;
    bool  show_viz      = true;
};

// ─── App State ────────────────────────────────────────────────────────────────
struct AppState {
    AppPage      current_page = AppPage::LANDING;
    SpringState  spring;
    float        anim_t       = 0.0f;  // global animation time
    int          hovered_card = -1;
    float        landing_scroll = 0.0f;
};

// ─── Calculator Registry ─────────────────────────────────────────────────────
inline std::vector<CalculatorInfo> GetCalculatorRegistry() {
    return {
        {
            "spring_damping",
            "Spring & Damping",
            "Compute damping ratio, natural frequency,\nsettling time & overshoot for spring systems.",
            "\xef\x81\xa9", // gear-ish, fallback to text below
            "PHYSICS",
            false
        },
        // ── ADD NEW CALCULATORS BELOW THIS LINE ──────────────────────────────
        // {
        //   "audio_envelope",
        //   "Audio Envelope",
        //   "ADSR envelope time constants\nfor sound design.",
        //   "♪", "AUDIO", true
        // },
    };
}
