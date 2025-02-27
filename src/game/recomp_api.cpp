#include <cmath>

#include "recomp.h"
#include "librecomp/overlays.hpp"
#include "zelda_config.h"
#include "recomp_input.h"
#include "recomp_ui.h"
#include "zelda_render.h"
#include "zelda_sound.h"
#include "librecomp/helpers.hpp"
#include "../patches/input.h"
#include "../patches/graphics.h"
#include "../patches/sound.h"
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"
#include "librecomp/addresses.hpp"

//map compressed rom address -> decompressed rom address, then load the overlay
extern "C" void load_dk64_overlay(uint32_t compressed_rom, int32_t ram_addr, uint32_t size) {
    uint32_t decompressed_rom = 0;
    switch (compressed_rom) {
        case 0x113F0: //global_asm
            decompressed_rom = 0x2000000;
            break;
        case 0xCBE70: //menu
            decompressed_rom = 0x2165D50;
            break;
        case 0xD4B00: //multiplayer
            decompressed_rom = 0x2175C60;
            break;
        case 0xD6B00: //minecart
            decompressed_rom = 0x2178D60;
            break;
        case 0xD9A40: //bonus
            decompressed_rom = 0x217DB70;
            break;
        case 0xDF600: //race
            decompressed_rom = 0x2187A60;
            break;
        case 0xE6780: //critter
            decompressed_rom = 0x2193BC0;
            break;
        case 0xEA0B0: //boss
            decompressed_rom = 0x2199D70;
            break;
        case 0xF41A0: //arcade
            decompressed_rom = 0x21ACB30;
            break;
        case 0xFD2F0: //jetpac
            decompressed_rom = 0x21D3730;
            break;
    }
    if (decompressed_rom != 0) {
        load_overlays(decompressed_rom, ram_addr, size);
    }
}

extern "C" void boot_osPiRawStartDma(uint8_t* rdram, recomp_context* ctx) {
    uint32_t direction = ctx->r4;
    uint32_t device_address = ctx->r5;
    gpr rdram_address = ctx->r6;
    uint32_t size = ctx->r7;

    assert(direction == 0); // Only reads

    printf("boot_osPiRawStartDma: Rom %08X, Ram %08X, Size: %08X\n", device_address, rdram_address, size);

    // Complete the DMA synchronously (the game immediately waits until it's done anyways)
    recomp::do_rom_read(rdram, rdram_address, device_address + recomp::rom_base, size);
}

extern "C" void recomp_update_inputs(uint8_t * rdram, recomp_context * ctx) {
    recomp::poll_inputs();
}

extern "C" void recomp_puts(uint8_t * rdram, recomp_context * ctx) {
    PTR(char) cur_str = _arg<0, PTR(char)>(rdram, ctx);
    u32 length = _arg<1, u32>(rdram, ctx);

    for (u32 i = 0; i < length; i++) {
        fputc(MEM_B(i, (gpr)cur_str), stdout);
    }
}

extern "C" void recomp_exit(uint8_t * rdram, recomp_context * ctx) {
    ultramodern::quit();
}

extern "C" void recomp_get_gyro_deltas(uint8_t * rdram, recomp_context * ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_gyro_deltas(x_out, y_out);
}

extern "C" void recomp_get_mouse_deltas(uint8_t * rdram, recomp_context * ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recomp::get_mouse_deltas(x_out, y_out);
}

extern "C" void recomp_powf(uint8_t * rdram, recomp_context * ctx) {
    float a = _arg<0, float>(rdram, ctx);
    float b = ctx->f14.fl; //_arg<1, float>(rdram, ctx);

    _return(ctx, std::pow(a, b));
}

extern "C" void recomp_get_target_framerate(uint8_t * rdram, recomp_context * ctx) {
    int frame_divisor = _arg<0, u32>(rdram, ctx);

    _return(ctx, ultramodern::get_target_framerate(60 / frame_divisor));
}

extern "C" void recomp_get_window_resolution(uint8_t * rdram, recomp_context * ctx) {
    int width, height;
    recompui::get_window_size(width, height);

    gpr width_out = _arg<0, PTR(u32)>(rdram, ctx);
    gpr height_out = _arg<1, PTR(u32)>(rdram, ctx);

    MEM_W(0, width_out) = (u32)width;
    MEM_W(0, height_out) = (u32)height;
}

extern "C" void recomp_get_target_aspect_ratio(uint8_t * rdram, recomp_context * ctx) {
    ultramodern::renderer::GraphicsConfig graphics_config = ultramodern::renderer::get_graphics_config();
    float original = _arg<0, float>(rdram, ctx);
    int width, height;
    recompui::get_window_size(width, height);

    switch (graphics_config.ar_option) {
    case ultramodern::renderer::AspectRatio::Original:
    default:
        _return(ctx, original);
        return;
    case ultramodern::renderer::AspectRatio::Expand:
        _return(ctx, std::max(static_cast<float>(width) / height, original));
        return;
    }
}

extern "C" void recomp_get_targeting_mode(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<int>(zelda64::get_targeting_mode()));
}

extern "C" void recomp_get_bgm_volume(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, zelda64::get_bgm_volume() / 100.0f);
}

extern "C" void recomp_get_low_health_beeps_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<u32>(zelda64::get_low_health_beeps_enabled()));
}

extern "C" void recomp_time_us(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<u32>(std::chrono::duration_cast<std::chrono::microseconds>(ultramodern::time_since_start()).count()));
}

extern "C" void recomp_get_autosave_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<s32>(zelda64::get_autosave_mode() == zelda64::AutosaveMode::On));
}

extern "C" void recomp_load_overlays(uint8_t * rdram, recomp_context * ctx) {
    u32 rom = _arg<0, u32>(rdram, ctx);
    PTR(void) ram = _arg<1, PTR(void)>(rdram, ctx);
    u32 size = _arg<2, u32>(rdram, ctx);

    load_overlays(rom, ram, size);
}

extern "C" void recomp_high_precision_fb_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<s32>(zelda64::renderer::RT64HighPrecisionFBEnabled()));
}

extern "C" void recomp_get_resolution_scale(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, ultramodern::get_resolution_scale());
}

extern "C" void recomp_get_inverted_axes(uint8_t * rdram, recomp_context * ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_get_analog_inverted_axes(uint8_t * rdram, recomp_context * ctx) {
    s32* x_out = _arg<0, s32*>(rdram, ctx);
    s32* y_out = _arg<1, s32*>(rdram, ctx);

    zelda64::CameraInvertMode mode = zelda64::get_analog_camera_invert_mode();

    *x_out = (mode == zelda64::CameraInvertMode::InvertX || mode == zelda64::CameraInvertMode::InvertBoth);
    *y_out = (mode == zelda64::CameraInvertMode::InvertY || mode == zelda64::CameraInvertMode::InvertBoth);
}

extern "C" void recomp_get_analog_cam_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return<s32>(ctx, zelda64::get_analog_cam_mode() == zelda64::AnalogCamMode::On);
}

extern "C" void recomp_get_camera_inputs(uint8_t * rdram, recomp_context * ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    // TODO expose this in the menu
    constexpr float radial_deadzone = 0.05f;

    float x, y;

    recomp::get_right_analog(&x, &y);

    float magnitude = sqrtf(x * x + y * y);

    if (magnitude < radial_deadzone) {
        *x_out = 0.0f;
        *y_out = 0.0f;
    }
    else {
        float x_normalized = x / magnitude;
        float y_normalized = y / magnitude;

        *x_out = x_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
        *y_out = y_normalized * ((magnitude - radial_deadzone) / (1 - radial_deadzone));
    }
}

extern "C" void recomp_set_right_analog_suppressed(uint8_t * rdram, recomp_context * ctx) {
    s32 suppressed = _arg<0, s32>(rdram, ctx);

    recomp::set_right_analog_suppressed(suppressed);
}

constexpr uint32_t k1_to_phys(uint32_t addr) {
    return addr & 0x1FFFFFFF;
}

extern "C" void __ll_to_f_recomp(uint8_t * rdram, recomp_context * ctx) {
    int64_t a = (ctx->r4 << 32) | ((ctx->r5 << 0) & 0xFFFFFFFFu);
    float ret = (float)a;

    ctx->f0.fl = ret;
}

extern "C" void __f_to_ull_recomp(uint8_t * rdram, recomp_context * ctx) {
    uint64_t ret = (uint64_t)ctx->f12.fl;
    ctx->r2 = (int32_t)(ret >> 32);
    ctx->r3 = (int32_t)(ret >> 0);
}

extern "C" void __ull_rshift_recomp(uint8_t * rdram, recomp_context * ctx) {
    uint64_t a = (ctx->r4 << 32) | ((ctx->r5 << 0) & 0xFFFFFFFFu);
    uint64_t b = (ctx->r6 << 32) | ((ctx->r7 << 0) & 0xFFFFFFFFu);
    uint64_t result = a >> b;
    
    ctx->r2 = (int32_t)(result >> 32);
    ctx->r3 = (int32_t)(result >> 0);
}

extern "C" void __f_to_ll_recomp(uint8_t * rdram, recomp_context * ctx) {
    int64_t ret = (int64_t)ctx->f12.fl;
    ctx->r2 = (int32_t)(ret >> 32);
    ctx->r3 = (int32_t)(ret >> 0);
}

extern "C" void osPiReadIo_recomp(RDRAM_ARG recomp_context * ctx) {
    uint32_t devAddr = recomp::rom_base | ctx->r4;
    gpr dramAddr = ctx->r5;
    uint32_t physical_addr = k1_to_phys(devAddr);

    if (physical_addr > recomp::rom_base) {
        // cart rom
        recomp::do_rom_pio(PASS_RDRAM dramAddr, physical_addr);
    } else {
        // sram
        assert(false && "SRAM ReadIo unimplemented");
    }

    ctx->r2 = 0;
}