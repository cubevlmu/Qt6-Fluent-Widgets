#pragma once

#include <QColor>
#include <QString>
#include <QMetaEnum>

namespace fluent {

    class FluentThemeColor {
        Q_GADGET
    public:
        enum Value {
            YELLOW_GOLD,
            GOLD,
            ORANGE_BRIGHT,
            ORANGE_DARK,
            RUST,
            PALE_RUST,
            BRICK_RED,
            MOD_RED,
            PALE_RED,
            RED,
            ROSE_BRIGHT,
            ROSE,
            PLUM_LIGHT,
            PLUM,
            ORCHID_LIGHT,
            ORCHID,
            DEFAULT_BLUE,
            NAVY_BLUE,
            PURPLE_SHADOW,
            PURPLE_SHADOW_DARK,
            IRIS_PASTEL,
            IRIS_SPRING,
            VIOLET_RED_LIGHT,
            VIOLET_RED,
            COOL_BLUE_BRIGHT,
            COOL_BLUR,
            SEAFOAM,
            SEAFOAM_TEAL,
            MINT_LIGHT,
            MINT_DARK,
            TURF_GREEN,
            SPORT_GREEN,
            GRAY,
            GRAY_BROWN,
            STEAL_BLUE,
            METAL_BLUE,
            PALE_MOSS,
            MOSS,
            MEADOW_GREEN,
            GREEN,
            OVERCAST,
            STORM,
            BLUE_GRAY,
            GRAY_DARK,
            LIDDY_GREEN,
            SAGE,
            CAMOUFLAGE_DESERT,
            CAMOUFLAGE
        };
        Q_ENUM(Value);

        static QColor color(Value value);
    };
}