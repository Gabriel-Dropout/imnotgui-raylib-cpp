#include "imnotgui.hpp"

#define ATLAS_ATLAS_IMAGE_PATH      "atlas.png"
#define ATLAS_ATLAS_SPRITE_COUNT    19

// Atlas sprites array
static rtpAtlasSprite rtpDescAtlas[19] = {
    { "spr_pfp", 100, 100, 803, 367, 200, 200, 0, false, 0, 0, 200, 200 },
    { "spr_uicheck", 8, 8, 952, 731, 16, 16, 0, false, 0, 0, 16, 16 },
    { "spr_uimouse_0", 0, 0, 792, 731, 40, 40, 0, false, 0, 0, 28, 40 },
    { "spr_uimouse_1", 0, 0, 832, 731, 40, 40, 0, false, 2, 4, 24, 34 },
    { "spr_uimouse_2", 0, 0, 872, 731, 40, 40, 0, false, 2, 4, 24, 34 },
    { "spr_biglogo", 280, 64, 0, 731, 560, 128, 0, false, 23, 14, 517, 110 },
    { "spr_chip64", 32, 32, 560, 731, 64, 64, 0, false, 0, 0, 64, 64 },
    { "spr_diamond", 10, 10, 912, 731, 20, 20, 0, false, 0, 0, 20, 20 },
    { "spr_here", 10, 10, 932, 731, 20, 20, 0, false, 1, 1, 18, 18 },
    { "spr_icons42_0", 21, 21, 708, 731, 42, 42, 0, false, 0, 0, 42, 42 },
    { "spr_icons42_1", 21, 21, 750, 731, 42, 42, 0, false, 0, 0, 42, 42 },
    { "spr_icons42_2", 21, 21, 624, 731, 42, 42, 0, false, 0, 0, 42, 42 },
    { "spr_icons42_3", 21, 21, 666, 731, 42, 42, 0, false, 0, 0, 42, 42 },
    { "spr_logo_0", 128, 128, 291, 367, 256, 256, 0, false, 8, 72, 239, 90 },
    { "spr_logo_1", 128, 128, 547, 367, 256, 256, 0, false, 8, 72, 239, 90 },
    { "spr_loss_0", 147, 183, 0, 0, 294, 367, 0, false, 0, 0, 294, 367 },
    { "spr_loss_1", 147, 183, 0, 367, 291, 364, 0, false, 0, 0, 291, 364 },
    { "spr_loss_2", 147, 183, 588, 0, 289, 364, 0, false, 0, 0, 289, 364 },
    { "spr_loss_3", 147, 183, 294, 0, 294, 367, 0, false, 0, 0, 294, 367 },
};
