#pragma once

namespace fluent { 
    namespace os {
        bool isDarkTheme();
        
        typedef struct {
            float r;
            float g;
            float b;
            float a;
        } v_color;

        v_color getPrimaryColor();
    } 
}