#ifdef __WIN32__

#include "OS.hpp"

#include <windows.h>

namespace fluent::os {

    bool isDarkTheme() {
        DWORD value;
        DWORD valueSize = sizeof(value);

        HKEY hKey;
        LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey);

        if (result != ERROR_SUCCESS) {
            return false;
        }

        result = RegQueryValueEx(hKey, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &valueSize);

        RegCloseKey(hKey);

        if (result != ERROR_SUCCESS) {
            return false;
        }

        return value == 0;
    }


    v_color getPrimaryColor()
    { 
        DWORD highlightColor = GetSysColor(COLOR_HIGHLIGHT);

        int red = GetRValue(highlightColor);
        int green = GetGValue(highlightColor);
        int blue = GetBValue(highlightColor);

        return v_color { .r = red, .g = green, .b = blue };
    }

}

#endif