#ifdef __MACH__

#import "OS.hpp"

#import "Cocoa/Cocoa.h"

namespace fluent::os {

    bool isDarkTheme() {
        @autoreleasepool {
            NSString *osxMode = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
            return [osxMode isEqualToString:@"Dark"];
        }
    }

    v_color getPrimaryColor() {
        @autoreleasepool {
            NSColor *highlightColor = [NSColor selectedContentBackgroundColor];
            
            CGFloat red, green, blue, alpha;
            [[highlightColor colorUsingColorSpace:[NSColorSpace deviceRGBColorSpace]] getRed:&red green:&green blue:&blue alpha:&alpha];

            v_color color {
                .r = (float)red,
                .g = (float)green,
                .b = (float)blue,
                .a = (float)alpha
            };

            return color;
        }
    }
}

#endif