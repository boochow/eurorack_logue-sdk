#pragma once
/*
 *  File: synth.h
 *
 *  synth unit for braids
 *
 */

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstdio>

#include <algorithm>

#include "userosc.h"   // Note: Include base definitions for osc units

#include "stmlib/utils/dsp.h"
#include "macro_oscillator.h"
//#include "braids/signature_waveshaper.h"
//#include "braids/vco_jitter_source.h"

#define fast_inline __fast_inline

using namespace stmlib;
enum Params {
    Shape,
    ModTarget,
    ModDelay,
    Pitch,
    Resolution,
    SampleRate,
    Param1,
    Param2,
    PARAMCOUNT,
};

const uint16_t bit_reduction_masks[] = {
    0xffff,
    0xfff0,
    0xff00,
    0xf800,
    0xf000,
    0xe000,
    0xc000
};

const uint16_t decimation_factors[] = { 1, 2, 3, 6, 8, 12 };

class Osc {
public:
    Osc(void) {}
    ~Osc(void) {}

    inline void Init(uint32_t platform, uint32_t api) {

        std::memset(&osc_, 0, sizeof(osc_));
        osc_.Init();
//        ws_.Init(0x42636877U); // in the original src, MPU's unique id is used 
//        jitter_source_.Init();

#ifdef LILYVA
        p_[Shape] = 0;
#endif
#ifdef LILYDG
        p_[Shape] = 15;
#endif
#ifdef LILYFM
        p_[Shape] = 21;
#endif
#ifdef LILYRS
        p_[Shape] = 28;
#endif
#ifdef LILYWT
        p_[Shape] = 37;
#endif
#ifdef LILYNZ
        p_[Shape] = 41;
#endif
        osc_.set_shape(static_cast<braids::MacroOscillatorShape>(p_[Shape]));

        lfo_timbre_ = 1;
        lfo_color_ = 0;
    }

    inline void Process(q31_t * out, size_t frames, const user_osc_param_t * const params) {
        q31_t * __restrict out_p = out;
#ifdef KORG_NTS1
        // lfo value is 0..1.0 ; 0x0..0x7fff
        int16_t lfo = params->shape_lfo >> 16;
#else
        // lfo value is -1.0..1.0 ; 0x0000..0xffff
        int16_t lfo = params->shape_lfo >> 17;
#endif
        int16_t lfoz = lfoz_;
        const int16_t lfo_inc = (lfo - lfoz) / frames;
        uint32_t lfo_count = lfo_count_;
        const int bufsize = 24; // size of temp_buffer in macro_oscillator.h

        int16_t buf[bufsize] = {};
        const uint8_t sync[bufsize] = {};
        size_t decimation_factor = decimation_factors[p_[SampleRate]];
        uint16_t bit_mask = bit_reduction_masks[p_[Resolution]];
//        uint16_t signature = p_[Signature] * p_[Signature] * 4095;
        static uint32_t n = 0;
        static int16_t current_sample = 0;
        for(uint32_t p = 0; p < frames; p += bufsize) {
            // *NOTE* the stepsize of this loop is busize(24 samples)
            int16_t lfo_delayed = 1.f / (lfo_count + 1) * lfoz;
            if (lfo_count) {
                lfo_count--;
            }

            // Set timbre and color: parameter value + LFO modulation.
            int32_t timbre = timbre_;
            timbre += lfo_delayed * lfo_timbre_;
            CONSTRAIN(timbre, 0, 32767);

            int32_t color = color_;
            color += lfo_delayed * lfo_color_;
            CONSTRAIN(color, 0, 32767);
            osc_.set_parameters(timbre, color);

            // Set pitch: parameter value + audio_in * (parameter value + LFO).
//            const unit_runtime_osc_context_t *ctxt = static_cast<const unit_runtime_osc_context_t *>(runtime_desc_.hooks.runtime_context);
            int32_t pitch = params->pitch >> 1;
            pitch += p_[Pitch] * 4;

            if (pitch > 16383) {
                pitch = 16383;
            } else if (pitch < 0) {
                pitch = 0;
            }

            osc_.set_pitch(pitch);

            size_t r_size = (bufsize < (frames - p)) ? bufsize : frames - p;
            osc_.Render(sync, buf, r_size);

            
            // Copy to the buffer with sample rate and bit reduction applied.
            for(uint32_t i = 0; i < r_size ; i++, n++, out_p += 1) {
                if ((n % decimation_factor) == 0) {
                    current_sample = buf[i] & bit_mask;
                }
#ifdef KORG_PROLOGUE
                *out_p = f32_to_q31(current_sample / 32768.f);
#else
                *out_p = f32_to_q31(-current_sample / 32768.f);
#endif
            }
            lfoz += lfo_inc;
        }
        lfoz_ = lfoz;
        lfo_count_ = lfo_count;
    }

    inline void setParameter(uint8_t index, int32_t value) {
        switch(index) {
        case Shape:
#ifdef KORG_NTS1
            // NTS-1's shape parameter value is 1 greater than other platforms
            // due to a bug of NTS-1's firmware
            value--;
#endif
#ifdef LILYVA
            CONSTRAIN(value, 0, 14);
#endif
#ifdef LILYDG
            CONSTRAIN(value, 15, 20);
#endif
#ifdef LILYFM
            CONSTRAIN(value, 21, 27);
#endif
#ifdef LILYRS
            CONSTRAIN(value, 28, 36);
#endif
#ifdef LILYWT
            CONSTRAIN(value, 37, 40);
#endif
#ifdef LILYNZ
            CONSTRAIN(value, 41, 46);
#endif
            break;
        case Param1:
        case Param2:
            CONSTRAIN(value, 0, 1023);
            break;
        case ModTarget:
            CONSTRAIN(value, 0, 2);
            break;
        case ModDelay:
            CONSTRAIN(value, 0, 31);
            break;
        case Pitch:
            CONSTRAIN(value, 0, 63);
            break;
        case Resolution:
            CONSTRAIN(value, 0, 6);
            break;
        case SampleRate:
            CONSTRAIN(value, 0, 5);
            break;
        }
        p_[index] = value;
        switch (index) {
        case Shape:   // 0..47
/*
#ifdef LILYNZ
            // for easter egg
            if (value == 47 && p_[Shape] != 47) {
                if (p_[Param1] == 0 &&
                    p_[Param2] == 1023) {
                    value = 47;
                } else {
                    value = 46;
                }
            }
#endif
*/
            osc_.set_shape(static_cast<braids::MacroOscillatorShape>(value));
            break;
        case Param1:
            // timbre and color must be 0..32767
            timbre_ = value << 5;
            break;
        case Param2:
            color_ = value << 5;
            break;
        case ModTarget:
            switch(value) {
            case 0:
            case 1:
                lfo_color_ = value;
                lfo_timbre_ = 1 - lfo_color_;
                break;
            default:
                lfo_color_ = 0;
                lfo_timbre_ = 0;
            }
            break;
        case ModDelay:
            lfo_delay_ = k_samplerate / 240 * value; 
            break;
        default:
            break;
        }
    }

    inline void NoteOn(uint8_t note, uint8_t velocity) {
        pitch_ = note << 7;
        GateOn(velocity);
    }

    inline void NoteOff(uint8_t note) {
        (void)note;
        GateOff();
    }

    inline void GateOn(uint8_t velocity) {
        gate_ += 1;
        osc_.Strike();
        lfo_count_ = lfo_delay_;
    }

    inline void GateOff() {
        if (gate_ > 0 ) {
            gate_ -= 1;
        }
    }

private:
    int32_t p_[PARAMCOUNT];

    braids::MacroOscillator osc_;
//    braids::SignatureWaveshaper ws_;
//    braids::VcoJitterSource jitter_source_;

    int16_t pitch_;
    int16_t timbre_;
    int16_t color_;
    int16_t gate_;
    int16_t lfoz_;
    uint16_t lfo_timbre_;
    uint16_t lfo_color_;
    uint32_t lfo_delay_;
    uint32_t lfo_count_;

    /* Private Methods. */
};
