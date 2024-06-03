#pragma once

#include <QObject>
#include <QColor>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>
#include <QJsonArray>
#include <QStringList>
#include <QException>
#include <QMetaEnum>
#include <QMetaObject>
#include <QMetaProperty>
#include <QCoreApplication>
#include <qtmetamacros.h>

namespace fluent {


#define ALERT "\n📢 Tips: QFluentWidgets. Ported by cubevlmu. Original written by zhiyiyo\n"

    class Theme 
    {
        Q_GADGET
    public:
        enum class Mode {
            Light,
            Dark,
            Auto
        };
        Q_ENUM(Mode);
    };


    class ConfigValidator : public QObject {
        friend class RangeConfigItem;
        Q_OBJECT

    public:
        virtual bool validate(const QVariant& value) const { return true; }
        virtual QVariant correct(const QVariant& value) const { return value; }
    };


    class RangeValidator : public ConfigValidator {
        Q_OBJECT

    public:
        RangeValidator(double min, double max);

        bool validate(const QVariant& value) const override;
        QVariant correct(const QVariant& value) const override;

        double min, max;
    };


    class OptionsValidator : public ConfigValidator {
        Q_OBJECT
    public:
        OptionsValidator(const QStringList& options);

        bool validate(const QVariant& value) const override;
        QVariant correct(const QVariant& value) const override;

        QStringList options;
    };


    class BoolValidator : public OptionsValidator {
        Q_OBJECT
    public:
        BoolValidator();
    };


    class FolderValidator : public ConfigValidator {
        Q_OBJECT
    public:
        bool validate(const QVariant& value) const override;
        QVariant correct(const QVariant& value) const override;
    };


    class FolderListValidator : public ConfigValidator {
        Q_OBJECT
    public:
        bool validate(const QVariant& value) const override;
        QVariant correct(const QVariant& value) const override;
    };


    class ColorValidator : public ConfigValidator {
        Q_OBJECT
    public:
        ColorValidator(const QColor& defaultColor);

        bool validate(const QVariant& value) const override;
        QVariant correct(const QVariant& value) const override;

    private:
        QColor defaultColor;
    };


    class ConfigSerializer : public QObject {
        Q_OBJECT
    public:
        virtual QVariant serialize(const QVariant& value) const;
        virtual QVariant deserialize(const QVariant& value) const;
    };


    class EnumSerializer : public ConfigSerializer {
        Q_OBJECT
    public:
        EnumSerializer(QMetaEnum enumMeta);

        QVariant serialize(const QVariant& value) const override;
        QVariant deserialize(const QVariant& value) const override;

    private:
        QMetaEnum enumMeta;
    };


    class ColorSerializer : public ConfigSerializer {
        Q_OBJECT
    public:
        QVariant serialize(const QVariant& value) const override;
        QVariant deserialize(const QVariant& value) const override;
    };


    class ConfigItem : public QObject {
        friend class RangeConfigItem;

        Q_OBJECT;
        Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged);

    public:
        ConfigItem(const QString& group, const QString& name, const QVariant& defaultValue, ConfigValidator* validator = nullptr, ConfigSerializer* serializer = nullptr, bool restart = false);

        QVariant value() const { return _value; }
        void setValue(const QVariant& v);

        QString key() const;

        QVariant serialize() const;
        void deserializeFrom(const QVariant& value);

    signals:
        void valueChanged(const QVariant& value);

    public:
        QString group;
        QString name;
        QVariant _value;
        ConfigValidator* validator;
        ConfigSerializer* serializer;
        bool restart;
        QVariant defaultValue;
    };


    class RangeConfigItem : public ConfigItem {
        Q_OBJECT
    public:
        RangeConfigItem(const QString& group, const QString& name, const QVariant& defaultValue, RangeValidator* validator, bool restart = false);

        QString toString() const;
        QVariant range() const;
    };


    class OptionsConfigItem : public ConfigItem {
        Q_OBJECT
    public:
        OptionsConfigItem(const QString& group, const QString& name, const QVariant& defaultValue, OptionsValidator* validator, ConfigSerializer* serializer = nullptr, bool restart = false);

        QString toString() const;
        QStringList options() const;
    };


    class ColorConfigItem : public ConfigItem {
        Q_OBJECT
    public:
        ColorConfigItem(const QString& group, const QString& name, const QColor& defaultColor, bool restart = false);
        QString toString() const;
    };


    class QConfig : public QObject {
        Q_OBJECT

    public:
        OptionsConfigItem* themeMode;
        ColorConfigItem* themeColor;

        QConfig(QObject* parent = nullptr);

        QVariant get(ConfigItem* item) const;
        void set(ConfigItem* item, const QVariant& value, bool save = true, bool copy = true);

        QJsonObject toJson() const;

        void loadConfig();
        void saveConfig() const;

        Theme getTheme();

    signals:
        void appRestartSig();
        void themeChanged(Theme::Mode theme);
        void themeChangedFinished();
        void themeColorChanged(QColor color);

    private:
        QHash<QString, ConfigItem*> configItems;
        QFile file;
    };


    Q_GLOBAL_STATIC(QConfig, qconfig);


    bool isDarkTheme();
    Theme::Mode theme();

}