#pragma once

#include <QIconEngine>
#include <QPainter>
#include <QRect>
#include <QSvgRenderer>
#include <QFile>
#include <QDomDocument>
#include <QIcon>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QMap>

#include "Config.hpp"

namespace fluent {

    QString getIconColor(Theme::Mode theme = Theme::Mode::Auto, bool reverse = false);
    void drawSvgIcon(const QString& icon, QPainter* painter, const QRect& rect);
    QString writeSvg(const QString& iconPath, const QList<int>& indexes = {}, const QMap<QString, QString>& attributes = {});
    void drawIcon(QIcon& icon, QPainter* painter, QRectF rect, QIcon::State state = QIcon::Off);

    class FluentIconBase
    {
    public:
        virtual QString path(Theme::Mode theme = Theme::Mode::Auto) const = 0;

        QIcon icon(Theme::Mode theme = Theme::Mode::Auto, const QColor& color = QColor()) const;
        QIcon qicon(bool reverse = false) const;

        void render(QPainter* painter, const QRect& rect, Theme::Mode theme = Theme::Mode::Auto, const QList<int>& indexes = {}, const QMap<QString, QString>& attributes = {}) const;
    };


    class FluentIconEngine : public QIconEngine
    {
    public:
        FluentIconEngine(QIcon icon, bool reverse = false);
        FluentIconEngine(FluentIconBase* icon, bool reverse = false);

        void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;

        QIconEngine* clone() const override;
        QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

    private:
        QIcon icon;
        FluentIconBase* ficon;
        bool isThemeReversed;

        bool isDarkTheme() const;
    };


    class SvgIconEngine : public QIconEngine
    {
    public:
        SvgIconEngine(const QString& svg);

        void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) override;
        QIconEngine* clone() const override;
        QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override;

    private:
        QString svg;
    };


    class FluentIcon : public FluentIconBase {
    public:
        enum IconType {
            Up, Add, Bus, Car, Cut, IOT, Pin, Tag, VPN, Cafe, Chat, Copy, Code, Down, Edit, Flag, Font, Game, Help, Hide, Home, Info, Leaf, Link, Mail, Menu, Mute, More, Move, Play, Save, Send, Sync, Unit, View, Wifi, Zoom, Album, Brush, Broom, Close, Cloud, Embed, Globe, Heart, Label, Media, Movie, Music, Robot, Pause, Paste, Photo, Phone, Print, Share, Tiles, Unpin, Video, Train, AddTo, Accept, Camera, Cancel, Delete, Folder, Filter, Market, Scroll, Layout, GitHub, Update, Remove, Return, People, QRCode, Ringer, Rotate, Search, Volume, Frigid, SaveAs, ZoomIn, Connect, History, Setting, Palette, Message, FitPage, ZoomOut, Airplane, Asterisk, Calories, Calendar, Feedback, Library, Minimize, CheckBox, Document, Language, Download, Question, Speakers, DateTime, FontSize, HomeFill, PageLeft, SaveCopy, SendFill, SkipBack, SpeedOff, Alignment, Bluetooth, Completed, Constract, Headphone, Megaphone, Projector, Education, LeftArrow, EraseTool, PageRight, PlaySolid, BookShelf, Highlight, FolderAdd, PauseBold, PencilInk, PieSingle, QuickNote, SpeedHigh, StopWatch, ZipFolder, Basketball, Brightness, Dictionary, Microphone, ArrowDown, FullScreen, MixVolumes, RemoveFrom, RightArrow, QuietHours, Fingerprint, Application, Certificate, Transparent, ImageExport, SpeedMedium, LibraryFill, MusicFolder, PowerButton, SkipForward, CareUpSolid, AcceptMedium, CancelMedium, ChevronRight, ClippingTool, SearchMirror, ShoppingCart, FontIncrease, BackToWindow, CommandPrompt, CloudDownload, DictionaryAdd, CareDownSolid, CareLeftSolid, ClearSelection, DeveloperTools, BackgroundColor, CareRightSolid, ChevronDownMed, ChevronRightMed, EmojiTabSymbols, ExpressiveInputEntry, Nil
        };

        FluentIcon(IconType type);

        QString path(Theme::Mode theme = Theme::Mode::Auto) const override;
        IconType getType() { return type; }
        bool isNull() { return type == Nil; }

    private:
        IconType type = IconType::Nil;

        static QString iconTypeToString(IconType type);
    };

}