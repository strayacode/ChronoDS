#include <core/hw/gpu/gpu.h>
#include <core/hw/gpu/gpu_2d.h>

void GPU2D::ComposeScanline(u16 line) {
    for (int i = 0; i < 256; i++) {
        ComposePixel(line, i);
    }
}

void GPU2D::ComposePixel(u16 line, u16 x) {
    // use an enabled variable with each bit corresponding to which layers are enabled
    u8 enabled = DISPCNT >> 8;

    u8 win0_x1 = WINH[0] >> 8;
    u8 win0_x2 = WINH[0] & 0xFF;
    u8 win0_y1 = WINV[0] >> 8;
    u8 win0_y2 = WINV[0] & 0xFF;
    
    u8 win1_x1 = WINH[1] >> 8;
    u8 win1_x2 = WINH[1] & 0xFF;
    u8 win1_y1 = WINV[1] >> 8;
    u8 win1_y2 = WINV[1] & 0xFF;

    // then at least 1 of the windows is enabled so handle accordingly
    if ((DISPCNT >> 13) & 0x7) {
        // TODO: add object window when we doing object rendering
        if ((DISPCNT & (1 << 13)) && x > win0_x1 && x <= win0_x2 && line > win0_y1 && line <= win0_y2) {
            // pixel is in win0
            enabled &= (WININ & 0xF);
        } else if ((DISPCNT & (1 << 14)) && x > win1_x1 && x <= win1_x2 && line > win1_y1 && line <= win1_y2) {
            // pixel is in win1
            enabled &= ((WININ >> 8) & 0xF);
        } else {
            // pixel is not in any window
            enabled &= (WINOUT & 0xF);  
        }
    }

    // only update the framebuffer if any layers are enabled
    if (enabled) {
        u16 pixel;

        u8 priority = 3;
        u8 priority_bg_index = 3;
        for (int i = 3; i >= 0; i--) {
            // only check priority if the bg layer is enabled as indicated by the variable enabled, which has taken into account window logic
            if ((enabled & (1 << i)) && (bg_layers[i][(256 * line) + x] != 0x8000)) {
                if ((BGCNT[i] & 0x3) <= priority) {
                    priority_bg_index = i;
                    priority = (BGCNT[i] & 0x3);
                }
            }
        }


        pixel = bg_layers[priority_bg_index][(256 * line) + x];
        // printf("%04x\n", obj_layer[(256 * line) + x]);
        if ((DISPCNT & (1 << 12)) && (obj_layer[(256 * line) + x].colour != 0x8000)) {
            // we should draw an obj pixel onto the screen
            pixel = obj_layer[(256 * line) + x].colour;
        }

        // finally store the pixel from the correct bg layer
        framebuffer[(256 * line) + x] = Convert15To24(pixel);
    }

}