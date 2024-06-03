#include "QFluentWidgets/common/StyleSheet.hpp"

#include <QString>
#include "StyleSheet.hpp"

#include "QFluentWidgets/utils/os.hpp"

namespace fluent {


    FluentStyleSheet::FluentStyleSheet(Type type) 
        : m_type(type) 
    {}


    QString FluentStyleSheet::path(Theme::Mode theme) const {
      // Assuming qconfig.theme is a global variable or a singleton
        if (theme == Theme::Mode::Auto) {
          theme = fluent::os::isDarkTheme() ? Theme::Mode::Dark : Theme::Mode::Light;
        }

        static const std::unordered_map<Type, std::string> typeToString{
            {Type::MENU, "menu"},
            {Type::LABEL, "label"},
            {Type::PIVOT, "pivot"},
            {Type::BUTTON, "button"},
            {Type::DIALOG, "dialog"},
            {Type::SLIDER, "slider"},
            {Type::INFO_BAR, "info_bar"},
            {Type::SPIN_BOX, "spin_box"},
            {Type::TAB_VIEW, "tab_view"},
            {Type::TOOL_TIP, "tool_tip"},
            {Type::CHECK_BOX, "check_box"},
            {Type::COMBO_BOX, "combo_box"},
            {Type::FLIP_VIEW, "flip_view"},
            {Type::LINE_EDIT, "line_edit"},
            {Type::LIST_VIEW, "list_view"},
            {Type::TREE_VIEW, "tree_view"},
            {Type::INFO_BADGE, "info_badge"},
            {Type::PIPS_PAGER, "pips_pager"},
            {Type::TABLE_VIEW, "table_view"},
            {Type::CARD_WIDGET, "card_widget"},
            {Type::TIME_PICKER, "time_picker"},
            {Type::COLOR_DIALOG, "color_dialog"},
            {Type::MEDIA_PLAYER, "media_player"},
            {Type::SETTING_CARD, "setting_card"},
            {Type::TEACHING_TIP, "teaching_tip"},
            {Type::FLUENT_WINDOW, "fluent_window"},
            {Type::SWITCH_BUTTON, "switch_button"},
            {Type::MESSAGE_DIALOG, "message_dialog"},
            {Type::STATE_TOOL_TIP, "state_tool_tip"},
            {Type::CALENDAR_PICKER, "calendar_picker"},
            {Type::FOLDER_LIST_DIALOG, "folder_list_dialog"},
            {Type::SETTING_CARD_GROUP, "setting_card_group"},
            {Type::EXPAND_SETTING_CARD, "expand_setting_card"},
            {Type::NAVIGATION_INTERFACE, "navigation_interface"}
        };

        static const std::unordered_map<Theme::Mode, std::string> themeToString{
            {Theme::Mode::Auto, "auto"},
            {Theme::Mode::Light, "light"},
            {Theme::Mode::Dark, "dark"}
        };

        std::string path = ":/qfluentwidgets/qss/" + themeToString.at(theme) + "/" + typeToString.at(m_type) + ".qss";
        return QString::fromStdString(path);
    }
}
