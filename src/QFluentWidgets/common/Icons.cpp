#include "Icons.hpp"

#include "QFluentWidgets/utils/os.hpp"

#include <QIcon>

namespace fluent {

    QString getIconColor(Theme::Mode theme, bool reverse) {
        QString lc = reverse ? "white" : "black";
        QString dc = reverse ? "black" : "white";

        if (theme == Theme::Mode::Auto) {
            return os::isDarkTheme() ? dc : lc;
        }
        else {
            return (theme == Theme::Mode::Dark) ? dc : lc;
        }
    }


    void drawSvgIcon(
        const QString& icon,
        QPainter* painter,
        const QRect& rect
    ) {
        QSvgRenderer renderer(icon.toUtf8());
        renderer.render(painter, QRectF(rect));
    }


    QString writeSvg(
        const QString& iconPath,
        const QList<int>& indexes,
        const QMap<QString, QString>& attributes
    ) {
        if (!iconPath.toLower().endsWith(".svg")) {
            return "";
        }

        QFile file(iconPath);
        if (!file.open(QFile::ReadOnly)) {
            return "";
        }

        QDomDocument dom;
        if (!dom.setContent(&file)) {
            file.close();
            return "";
        }
        file.close();

        QDomNodeList pathNodes = dom.elementsByTagName("path");
        QList<int> indices = indexes.isEmpty() ? QList<int>(pathNodes.count(), 0) : indexes;
        for (int i = 0; i < indices.size(); ++i) {
            QDomElement element = pathNodes.at(indices.at(i)).toElement();
            for (auto it = attributes.begin(); it != attributes.end(); ++it) {
                element.setAttribute(it.key(), it.value());
            }
        }

        return dom.toString();
    }


    void drawIcon(
        QIcon& icon, 
        QPainter* painter, 
        QRectF rect,
        QIcon::State state
    ) {
        icon.paint(painter, QRectF(rect).toRect(), Qt::AlignCenter, QIcon::Normal, state);
    }


    QIcon FluentIconBase::icon(Theme::Mode theme, const QColor& color) const {
        QString iconPath = path(theme);
        if (!iconPath.endsWith(".svg") || !color.isValid()) {
            return QIcon(iconPath);
        }

        QString colorStr = color.name();
        return QIcon(new SvgIconEngine(writeSvg(iconPath, {}, { {"fill", colorStr} })));
    }


    QIcon FluentIconBase::qicon(bool reverse) const {
        return QIcon(new FluentIconEngine(icon(), reverse));
    }


    void FluentIconBase::render(
        QPainter* painter,
        const QRect& rect,
        Theme::Mode theme,
        const QList<int>& indexes,
        const QMap<QString, QString>& attributes
    ) const {
        QString iconPath = path(theme);
        if (iconPath.endsWith(".svg")) {
            if (!attributes.isEmpty()) {
                iconPath = writeSvg(iconPath, indexes, attributes).toUtf8();
            }
            drawSvgIcon(iconPath, painter, rect);
        }
        else {
            QIcon icon(iconPath);
            painter->drawPixmap(rect, icon.pixmap(rect.size()));
        }
    }



    FluentIconEngine::FluentIconEngine(QIcon icon, bool reverse)
        : icon(icon), isThemeReversed(reverse), ficon(nullptr)
    {}


    FluentIconEngine::FluentIconEngine(FluentIconBase* icon, bool reverse)
        : ficon(icon), isThemeReversed(reverse)
    {}


    void FluentIconEngine::paint(
        QPainter* painter,
        const QRect& rect,
        QIcon::Mode mode,
        QIcon::State state
    ) {
        painter->save();

        if (mode == QIcon::Disabled) {
            painter->setOpacity(0.5);
        }
        else if (mode == QIcon::Selected) {
            painter->setOpacity(0.7);
        }

        QIcon finalIcon = icon;

        Theme::Mode theme = isThemeReversed ? (isDarkTheme() ? Theme::Mode::Light : Theme::Mode::Dark) : Theme::Mode::Auto;

        // 如果icon是 FluentIconBase 的实例，调用icon方法
        //TODO test it
        if (ficon != nullptr) {
            finalIcon = ficon->icon(theme);
        }
        // if (auto fluentIcon = dynamic_cast<FluentIconBase*>(icon); fluentIcon != nullptr) {
        //     finalIcon = fluentIcon->icon(theme);
        // }

        QRect adjustedRect = rect;
        if (rect.x() == 19) {
            adjustedRect.adjust(-1, 0, 0, 0);
        }

        finalIcon.paint(painter, adjustedRect, Qt::AlignCenter, mode, state);
        painter->restore();
    }


    QIconEngine* FluentIconEngine::clone() const {
        return new FluentIconEngine(*this);
    }


    QPixmap FluentIconEngine::pixmap(
        const QSize& size,
        QIcon::Mode mode,
        QIcon::State state
    ) {
        QImage image(size, QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);

        QPainter painter(&pixmap);
        QRect rect(0, 0, size.width(), size.height());
        this->paint(&painter, rect, mode, state);
        return pixmap;
    }


    bool FluentIconEngine::isDarkTheme() const {
        // 替换为实际的主题检查逻辑
        return os::isDarkTheme();
    }



    SvgIconEngine::SvgIconEngine(const QString& svg)
        : svg(svg)
    {}


    void SvgIconEngine::paint(
        QPainter* painter,
        const QRect& rect,
        QIcon::Mode mode,
        QIcon::State state
    ) {
        drawSvgIcon(svg, painter, rect);
    }


    QIconEngine* SvgIconEngine::clone() const {
        return new SvgIconEngine(*this);
    }


    QPixmap SvgIconEngine::pixmap(
        const QSize& size,
        QIcon::Mode mode,
        QIcon::State state
    ) {
        QImage image(size, QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPixmap pixmap = QPixmap::fromImage(image, Qt::NoFormatConversion);

        QPainter painter(&pixmap);
        QRect rect(0, 0, size.width(), size.height());
        this->paint(&painter, rect, mode, state);
        return pixmap;
    }



    FluentIcon::FluentIcon(IconType type)
        : type(type)
    {}


    QString FluentIcon::path(Theme::Mode theme) const {
        return QString(":/qfluentwidgets/images/icons/%1_%2.svg").arg(iconTypeToString(type)).arg(getIconColor(theme));
    }


    QString FluentIcon::iconTypeToString(IconType type) {
        static QMap<IconType, QString> iconTypeMap = {
            {Up, "Up"}, {Add, "Add"}, {Bus, "Bus"}, {Car, "Car"},
            {Cut, "Cut"}, {IOT, "IOT"}, {Pin, "Pin"}, {Tag, "Tag"},
            {VPN, "VPN"}, {Cafe, "Cafe"}, {Chat, "Chat"}, {Copy, "Copy"},
            {Code, "Code"}, {Down, "Down"}, {Edit, "Edit"}, {Flag, "Flag"},
            {Font, "Font"}, {Game, "Game"}, {Help, "Help"}, {Hide, "Hide"},
            {Home, "Home"}, {Info, "Info"}, {Leaf, "Leaf"}, {Link, "Link"},
            {Mail, "Mail"}, {Menu, "Menu"}, {Mute, "Mute"}, {More, "More"},
            {Move, "Move"}, {Play, "Play"}, {Save, "Save"}, {Send, "Send"},
            {Sync, "Sync"}, {Unit, "Unit"}, {View, "View"}, {Wifi, "Wifi"},
            {Zoom, "Zoom"}, {Album, "Album"}, {Brush, "Brush"}, {Broom, "Broom"},
            {Close, "Close"}, {Cloud, "Cloud"}, {Embed, "Embed"}, {Globe, "Globe"},
            {Heart, "Heart"}, {Label, "Label"}, {Media, "Media"}, {Movie, "Movie"},
            {Music, "Music"}, {Robot, "Robot"}, {Pause, "Pause"}, {Paste, "Paste"},
            {Photo, "Photo"}, {Phone, "Phone"}, {Print, "Print"}, {Share, "Share"},
            {Tiles, "Tiles"}, {Unpin, "Unpin"}, {Video, "Video"}, {Train, "Train"},
            {AddTo, "AddTo"}, {Accept, "Accept"}, {Camera, "Camera"}, {Cancel, "Cancel"},
            {Delete, "Delete"}, {Folder, "Folder"}, {Filter, "Filter"}, {Market, "Market"},
            {Scroll, "Scroll"}, {Layout, "Layout"}, {GitHub, "GitHub"}, {Update, "Update"},
            {Remove, "Remove"}, {Return, "Return"}, {People, "People"}, {QRCode, "QRCode"},
            {Ringer, "Ringer"}, {Rotate, "Rotate"}, {Search, "Search"}, {Volume, "Volume"},
            {Frigid, "Frigid"}, {SaveAs, "SaveAs"}, {ZoomIn, "ZoomIn"}, {Connect, "Connect"},
            {History, "History"}, {Setting, "Setting"}, {Palette, "Palette"}, {Message, "Message"},
            {FitPage, "FitPage"}, {ZoomOut, "ZoomOut"}, {Airplane, "Airplane"}, {Asterisk, "Asterisk"},
            {Calories, "Calories"}, {Calendar, "Calendar"}, {Feedback, "Feedback"}, {Library, "Library"},
            {Minimize, "Minimize"}, {CheckBox, "CheckBox"}, {Document, "Document"}, {Language, "Language"},
            {Download, "Download"}, {Question, "Question"}, {Speakers, "Speakers"}, {DateTime, "DateTime"},
            {FontSize, "FontSize"}, {HomeFill, "HomeFill"}, {PageLeft, "PageLeft"}, {SaveCopy, "SaveCopy"},
            {SendFill, "SendFill"}, {SkipBack, "SkipBack"}, {SpeedOff, "SpeedOff"}, {Alignment, "Alignment"},
            {Bluetooth, "Bluetooth"}, {Completed, "Completed"}, {Constract, "Constract"}, {Headphone, "Headphone"},
            {Megaphone, "Megaphone"}, {Projector, "Projector"}, {Education, "Education"}, {LeftArrow, "LeftArrow"},
            {EraseTool, "EraseTool"}, {PageRight, "PageRight"}, {PlaySolid, "PlaySolid"}, {BookShelf, "BookShelf"},
            {Highlight, "Highlight"}, {FolderAdd, "FolderAdd"}, {PauseBold, "PauseBold"}, {PencilInk, "PencilInk"},
            {PieSingle, "PieSingle"}, {QuickNote, "QuickNote"}, {SpeedHigh, "SpeedHigh"}, {StopWatch, "StopWatch"},
            {ZipFolder, "ZipFolder"}, {Basketball, "Basketball"}, {Brightness, "Brightness"}, {Dictionary, "Dictionary"},
            {Microphone, "Microphone"}, {ArrowDown, "ArrowDown"}, {FullScreen, "FullScreen"}, {MixVolumes, "MixVolumes"},
            {RemoveFrom, "RemoveFrom"}, {RightArrow, "RightArrow"}, {QuietHours, "QuietHours"}, {Fingerprint, "Fingerprint"},
            {Application, "Application"}, {Certificate, "Certificate"}, {Transparent, "Transparent"}, {ImageExport, "ImageExport"},
            {SpeedMedium, "SpeedMedium"}, {LibraryFill, "LibraryFill"}, {MusicFolder, "MusicFolder"}, {PowerButton, "PowerButton"},
            {SkipForward, "SkipForward"}, {CareUpSolid, "CareUpSolid"}, {AcceptMedium, "AcceptMedium"}, {CancelMedium, "CancelMedium"},
            {ChevronRight, "ChevronRight"}, {ClippingTool, "ClippingTool"}, {SearchMirror, "SearchMirror"}, {ShoppingCart, "ShoppingCart"},
            {FontIncrease, "FontIncrease"}, {BackToWindow, "BackToWindow"}, {CommandPrompt, "CommandPrompt"}, {CloudDownload, "CloudDownload"},
            {DictionaryAdd, "DictionaryAdd"}, {CareDownSolid, "CareDownSolid"}, {CareLeftSolid, "CareLeftSolid"}, {ClearSelection, "ClearSelection"},
            {DeveloperTools, "DeveloperTools"}, {BackgroundColor, "BackgroundColor"}, {CareRightSolid, "CareRightSolid"}, {ChevronDownMed, "ChevronDownMed"},
            {ChevronRightMed, "ChevronRightMed"}, {EmojiTabSymbols, "EmojiTabSymbols"}, {ExpressiveInputEntry, "ExpressiveInputEntry"}
        };

        return iconTypeMap.value(type);
    }

} // namespace fluent