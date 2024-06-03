#include "Config.hpp"

#include "QFluentWidgets/utils/OS.hpp"

#include <QRgb>

namespace fluent {

    RangeValidator::RangeValidator(double min, double max)
        : min(min), max(max)
    {}


    bool RangeValidator::validate(const QVariant& value) const {
        double val = value.toDouble();
        return val >= min && val <= max;
    }


    QVariant RangeValidator::correct(const QVariant& value) const {
        double val = value.toDouble();
        return qBound(min, val, max);
    }



    OptionsValidator::OptionsValidator(const QStringList& options) : options(options) {
        if (options.isEmpty()) {
            throw std::invalid_argument("The `options` can't be empty.");
        }
    }


    bool OptionsValidator::validate(const QVariant& value) const {
        return options.contains(value.toString());
    }


    QVariant OptionsValidator::correct(const QVariant& value) const {
        return validate(value) ? value : options.first();
    }



    BoolValidator::BoolValidator() : OptionsValidator({ "true", "false" }) {}



    bool FolderValidator::validate(const QVariant& value) const {
        QFileInfo dir(value.toString());
        return dir.exists() && dir.isDir();
    }


    QVariant FolderValidator::correct(const QVariant& value) const {
        QDir dir(value.toString());
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        return dir.absolutePath();
    }



    bool FolderListValidator::validate(const QVariant& value) const {
        QStringList list = value.toStringList();
        for (const QString& path : list) {
            QFileInfo dir(path);
            if (!dir.exists() || !dir.isDir()) {
                return false;
            }
        }
        return true;
    }


    QVariant FolderListValidator::correct(const QVariant& value) const {
        QStringList corrected;
        QStringList list = value.toStringList();
        for (const QString& path : list) {
            QDir dir(path);
            if (dir.exists()) {
                corrected << dir.absolutePath();
            }
        }
        return corrected;
    }



    ColorValidator::ColorValidator(const QColor& defaultColor)
        : defaultColor(defaultColor)
    {}


    bool ColorValidator::validate(const QVariant& value) const {
        QColor color(value.toString());
        return color.isValid();
    }


    QVariant ColorValidator::correct(const QVariant& value) const {
        return validate(value) ? value : defaultColor;
    }



    QVariant ConfigSerializer::serialize(const QVariant& value) const { return value; }
    QVariant ConfigSerializer::deserialize(const QVariant& value) const { return value; }



    EnumSerializer::EnumSerializer(QMetaEnum enumMeta)
        : enumMeta(enumMeta)
    {}


    QVariant EnumSerializer::serialize(const QVariant& value) const {
        return enumMeta.valueToKey(value.toInt());
    }


    QVariant EnumSerializer::deserialize(const QVariant& value) const {
        return enumMeta.keyToValue(value.toString().toLatin1());
    }



    QVariant ColorSerializer::serialize(const QVariant& value) const {
        return value.value<QColor>().name(QColor::HexArgb);
    }


    QVariant ColorSerializer::deserialize(const QVariant& value) const {
        return QColor(value.toString());
    }



    ConfigItem::ConfigItem(
        const QString& group,
        const QString& name,
        const QVariant& defaultValue,
        ConfigValidator* validator,
        ConfigSerializer* serializer,
        bool restart
    ) : group(group),
        name(name),
        validator(validator ? validator : new ConfigValidator()),
        serializer(serializer ? serializer : new ConfigSerializer()),
        restart(restart),
        defaultValue(defaultValue) {
        setValue(defaultValue);
    }


    void ConfigItem::setValue(const QVariant& v) {
        QVariant correctedValue = validator->correct(v);
        if (_value != correctedValue) {
            _value = correctedValue;
            emit valueChanged(_value);
        }
    }


    QString ConfigItem::key() const {
        return group + (name.isEmpty() ? "" : "." + name);
    }


    QVariant ConfigItem::serialize() const {
        return serializer->serialize(_value);
    }


    void ConfigItem::deserializeFrom(const QVariant& value) {
        setValue(serializer->deserialize(value));
    }



    QConfig::QConfig(QObject* parent) : QObject(parent) {
        
        // Fetch theme mode from system
        themeMode = new OptionsConfigItem("QFluentWidgets", "ThemeMode", QVariant::fromValue(os::isDarkTheme() ? Theme::Mode::Dark : Theme::Mode::Light),
            new OptionsValidator({ "Light", "Dark", "Auto" }), new EnumSerializer(QMetaEnum::fromType<Theme::Mode>()));
        
        
        // Fetch primary color from system
        auto vcc = os::getPrimaryColor();
        QColor color;
        color.setRgbF(vcc.r, vcc.g, vcc.b, vcc.a);

        themeColor = new ColorConfigItem("QFluentWidgets", "ThemeColor", color);

        configItems.insert(themeMode->key(), themeMode);
        configItems.insert(themeColor->key(), themeColor);


        file.setFileName(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.json");
        loadConfig();
    }


    QVariant QConfig::get(ConfigItem* item) const {
        return item->value();
    }


    void QConfig::set(
        ConfigItem* item,
        const QVariant& value,
        bool save,
        bool copy
    ) {
        if (item->value() == value) return;
        item->setValue(copy ? value : QVariant::fromValue(value));
        if (save) saveConfig();
        if (item->property("restart").toBool()) emit appRestartSig();
    }


    QJsonObject QConfig::toJson() const {
        QJsonObject json;
        for (const auto& item : configItems) {
            QStringList keys = item->key().split('.');
            QJsonObject* obj = &json;
            for (int i = 0; i < keys.size() - 1; ++i) {
                if (!obj->contains(keys[i])) {
                    obj->insert(keys[i], QJsonObject());
                }
                auto obja = (*obj)[keys[i]].toObject();
                obj = &obja; //TODO test it
            }
            obj->insert(keys.last(), QJsonValue::fromVariant(item->serialize()));
        }
        return json;
    }


    void QConfig::loadConfig() {
        if (!file.exists()) return;
        QFile configFile(file.fileName());
        if (!configFile.open(QIODevice::ReadOnly)) return;
        QByteArray data = configFile.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject json = doc.object();
        for (auto it = json.begin(); it != json.end(); ++it) {
            QJsonObject group = it.value().toObject();
            for (auto it2 = group.begin(); it2 != group.end(); ++it2) {
                QString key = it.key() + "." + it2.key();
                if (configItems.contains(key)) {
                    configItems[key]->deserializeFrom(it2.value().toVariant());
                }
            }
        }
    }


    void QConfig::saveConfig() const {
        QJsonDocument doc(toJson());
        QFile configFile(file.fileName());
        if (configFile.open(QIODevice::WriteOnly)) {
            configFile.write(doc.toJson());
        }
    }



    RangeConfigItem::RangeConfigItem(
        const QString& group,
        const QString& name,
        const QVariant& defaultValue,
        RangeValidator* validator,
        bool restart
    ) : ConfigItem(group, name, defaultValue, validator, nullptr, restart)
    {}


    QString RangeConfigItem::toString() const {
        auto validator = dynamic_cast<RangeValidator*>(ConfigItem::validator);
        return QString("%1[range=(%2, %3), value=%4]").arg(metaObject()->className()).arg(validator->min).arg(validator->max).arg(value().toString());
    }


    QVariant RangeConfigItem::range() const {
        auto validator = dynamic_cast<RangeValidator*>(ConfigItem::validator);
        return QVariant::fromValue(QPair<double, double>(validator->min, validator->max));
    }



    OptionsConfigItem::OptionsConfigItem(
        const QString& group,
        const QString& name,
        const QVariant& defaultValue,
        OptionsValidator* validator,
        ConfigSerializer* serializer,
        bool restart
    ) : ConfigItem(group, name, defaultValue, validator, serializer, restart)
    {}


    QString OptionsConfigItem::toString() const {
        auto validator = dynamic_cast<OptionsValidator*>(ConfigItem::validator);
        return QString("%1[options=%2, value=%3]").arg(metaObject()->className()).arg(validator->options.join(", ")).arg(value().toString());
    }


    QStringList OptionsConfigItem::options() const {
        auto validator = dynamic_cast<OptionsValidator*>(ConfigItem::validator);
        return validator->options;
    }



    ColorConfigItem::ColorConfigItem(
        const QString& group,
        const QString& name,
        const QColor& defaultColor,
        bool restart
    ) : ConfigItem(group, name, defaultColor, new ColorValidator(defaultColor), new ColorSerializer(), restart)
    {}


    QString ColorConfigItem::toString() const {
        return QString("%1[value=%2]").arg(metaObject()->className()).arg(value().value<QColor>().name());
    }


    bool isDarkTheme() {
        return qconfig->get(qconfig->themeMode).toString() == "Dark";
    }


    Theme::Mode theme() {
        QString themeStr = qconfig->get(qconfig->themeMode).toString();
        QMetaEnum metaEnum = QMetaEnum::fromType<Theme::Mode>();
        return static_cast<Theme::Mode>(metaEnum.keyToValue(themeStr.toLatin1()));
    }

} // namespace fluent