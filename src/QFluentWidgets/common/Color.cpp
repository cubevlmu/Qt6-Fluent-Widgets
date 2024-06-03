#include "Color.hpp"

namespace fluent {
    
    QColor FluentThemeColor::color(Value value){
        switch (value) {
        case YELLOW_GOLD: return QColor("#FFB900");
        case GOLD: return QColor("#FF8C00");
        case ORANGE_BRIGHT: return QColor("#F7630C");
        case ORANGE_DARK: return QColor("#CA5010");
        case RUST: return QColor("#DA3B01");
        case PALE_RUST: return QColor("#EF6950");
        case BRICK_RED: return QColor("#D13438");
        case MOD_RED: return QColor("#FF4343");
        case PALE_RED: return QColor("#E74856");
        case RED: return QColor("#E81123");
        case ROSE_BRIGHT: return QColor("#EA005E");
        case ROSE: return QColor("#C30052");
        case PLUM_LIGHT: return QColor("#E3008C");
        case PLUM: return QColor("#BF0077");
        case ORCHID_LIGHT: return QColor("#BF0077");
        case ORCHID: return QColor("#9A0089");
        case DEFAULT_BLUE: return QColor("#0078D7");
        case NAVY_BLUE: return QColor("#0063B1");
        case PURPLE_SHADOW: return QColor("#8E8CD8");
        case PURPLE_SHADOW_DARK: return QColor("#6B69D6");
        case IRIS_PASTEL: return QColor("#8764B8");
        case IRIS_SPRING: return QColor("#744DA9");
        case VIOLET_RED_LIGHT: return QColor("#B146C2");
        case VIOLET_RED: return QColor("#881798");
        case COOL_BLUE_BRIGHT: return QColor("#0099BC");
        case COOL_BLUR: return QColor("#2D7D9A");
        case SEAFOAM: return QColor("#00B7C3");
        case SEAFOAM_TEAL: return QColor("#038387");
        case MINT_LIGHT: return QColor("#00B294");
        case MINT_DARK: return QColor("#018574");
        case TURF_GREEN: return QColor("#00CC6A");
        case SPORT_GREEN: return QColor("#10893E");
        case GRAY: return QColor("#7A7574");
        case GRAY_BROWN: return QColor("#5D5A58");
        case STEAL_BLUE: return QColor("#68768A");
        case METAL_BLUE: return QColor("#515C6B");
        case PALE_MOSS: return QColor("#567C73");
        case MOSS: return QColor("#486860");
        case MEADOW_GREEN: return QColor("#498205");
        case GREEN: return QColor("#107C10");
        case OVERCAST: return QColor("#767676");
        case STORM: return QColor("#4C4A48");
        case BLUE_GRAY: return QColor("#69797E");
        case GRAY_DARK: return QColor("#4A5459");
        case LIDDY_GREEN: return QColor("#647C64");
        case SAGE: return QColor("#525E54");
        case CAMOUFLAGE_DESERT: return QColor("#847545");
        case CAMOUFLAGE: return QColor("#7E735F");
        default: return QColor();
        }
    }
}