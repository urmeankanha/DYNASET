#pragma once
#include "app.h"
#include <cmath>
#include <algorithm>

namespace SpringCalc {

static SpringResult Compute(const SpringState& s) {
    SpringResult r;

    double zeta = 0, wn = 0;

    if (s.input_mode == 0) {
        // Mode A: mass / spring constant / damper
        if (s.mass <= 0 || s.spring_k <= 0 || s.damper_c < 0) return r;
        wn   = std::sqrt((double)s.spring_k / (double)s.mass);
        double cc = 2.0 * std::sqrt((double)s.spring_k * (double)s.mass); // critical damping
        zeta = (double)s.damper_c / cc;
    } else if (s.input_mode == 1) {
        // Mode B: direct ζ and ωn
        zeta = (double)s.zeta_direct;
        wn   = (double)s.wn_direct;
        if (wn <= 0 || zeta < 0) return r;
    } else {
        // Mode C: from overshoot target → zeta
        double os = (double)s.target_overshoot / 100.0;
        if (os <= 0) { zeta = 1.0; }
        else if (os >= 1) { zeta = 0.001; }
        else {
            double ln_os = std::log(os);
            zeta = -ln_os / std::sqrt(M_PI * M_PI + ln_os * ln_os);
        }
        wn = (double)s.wn_direct; // reuse wn field
        if (wn <= 0) return r;
    }

    r.damping_ratio = zeta;
    r.natural_freq  = wn;

    if (zeta < 1.0) {
        r.regime      = "Underdamped";
        r.damped_freq = wn * std::sqrt(1.0 - zeta * zeta);
        // overshoot: e^(-pi*zeta/sqrt(1-zeta^2)) * 100
        r.overshoot_pct = std::exp(-M_PI * zeta / std::sqrt(1.0 - zeta * zeta)) * 100.0;
        // settling time (2% band): ~4 / (zeta * wn)
        r.settling_time = 4.0 / (zeta * wn);
    } else if (zeta == 1.0) {
        r.regime        = "Critically Damped";
        r.damped_freq   = 0.0;
        r.overshoot_pct = 0.0;
        r.settling_time = 4.0 / wn; // approx
    } else {
        r.regime        = "Overdamped";
        r.damped_freq   = 0.0;
        r.overshoot_pct = 0.0;
        // dominant pole: zeta*wn - wn*sqrt(zeta^2-1)
        double dominant = zeta * wn - wn * std::sqrt(zeta * zeta - 1.0);
        r.settling_time = 4.0 / dominant;
    }

    r.valid = true;
    return r;
}

// Generate step response curve points (normalized, t in seconds)
struct CurvePoint { float t, y; };

static std::vector<CurvePoint> StepResponse(const SpringResult& r, int points = 200) {
    std::vector<CurvePoint> curve;
    if (!r.valid || r.natural_freq <= 0) return curve;

    double wn   = r.natural_freq;
    double zeta = r.damping_ratio;
    double T    = std::max(r.settling_time * 1.4, 0.5 / wn);

    for (int i = 0; i < points; i++) {
        double t = T * i / (points - 1);
        double y;
        if (zeta < 1.0 - 1e-6) {
            double wd = r.damped_freq;
            y = 1.0 - std::exp(-zeta * wn * t) *
                (std::cos(wd * t) + (zeta / std::sqrt(1 - zeta * zeta)) * std::sin(wd * t));
        } else if (zeta < 1.0 + 1e-6) {
            y = 1.0 - std::exp(-wn * t) * (1.0 + wn * t);
        } else {
            double r1 = -zeta * wn + wn * std::sqrt(zeta * zeta - 1);
            double r2 = -zeta * wn - wn * std::sqrt(zeta * zeta - 1);
            y = 1.0 + (r2 / (r1 - r2)) * std::exp(r1 * t) - (r1 / (r1 - r2)) * std::exp(r2 * t);
        }
        curve.push_back({ (float)t, (float)y });
    }
    return curve;
}

} // namespace SpringCalc
