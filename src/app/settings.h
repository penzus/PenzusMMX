#ifndef SETTINGS_H
#define SETTINGS_H

#include <QStringList>
#include <QObject>
#include "taginfo.h"
#include <bb/utility/i18n/CustomDateFormatter>

typedef bb::utility::i18n::CustomDateFormatter AppDateFormatter;

class Settings: public QObject
{
    Q_OBJECT
public:
    static Settings & instance ();

    Q_PROPERTY(bool allowEditNoMultimedia READ getAllowEditNoMultimedia WRITE setAllowEditNoMultimedia)
    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize)
    Q_PROPERTY(int fontStyle READ getFontStyle WRITE setFontStyle)
    Q_PROPERTY(int fontWeight READ getFontWeight WRITE setFontWeight)
    Q_PROPERTY(int textFormat READ getTextFormat WRITE setTextFormat)

    Q_PROPERTY(bool boxBlackBerry READ getBoxBlackBerry WRITE setBoxBlackBerry NOTIFY boxBlackBerryChanged)
    Q_PROPERTY(bool dropBoxBlackBerry READ getDropBoxBlackBerry WRITE setDropBoxBlackBerry NOTIFY dropBoxBlackBerryChanged)
    Q_PROPERTY(bool oneDriveBlackBerry READ getOneDriveBlackBerry WRITE setOneDriveBlackBerry NOTIFY oneDriveBlackBerryChanged)

    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage)
    Q_PROPERTY(QString searchPath READ getSearchPath WRITE setSearchPath)
    Q_PROPERTY(bool localizedDateTimeFormat READ getLocalizedDateTimeFormat WRITE setLocalizedDateTimeFormat)
    Q_PROPERTY(bool openLastDirectoryOnStartup READ getOpenLastDirectoryOnStartup WRITE setOpenLastDirectoryOnStartup)
    Q_PROPERTY(bool showDescriptionInMainViewForDirs READ getShowDescriptionInMainViewForDirs WRITE setShowDescriptionInMainViewForDirs)
    Q_PROPERTY(bool showDescriptionInMainViewForFiles READ getShowDescriptionInMainViewForFiles WRITE setShowDescriptionInMainViewForFiles)
    Q_PROPERTY(bool showFileNameExtensionInMainView READ getShowFileNameExtensionInMainView WRITE setShowFileNameExtensionInMainView)
    Q_PROPERTY(bool showFileNameExtensionWhenRename READ getShowFileNameExtensionWhenRename WRITE setShowFileNameExtensionWhenRename)
    Q_PROPERTY(bool showScrollActions READ getShowScrollActions WRITE setShowScrollActions)
    Q_PROPERTY(bool showStatusInMainView READ getShowStatusInMainView WRITE setShowStatusInMainView)
    Q_PROPERTY(bool showTitleBar READ getShowTitleBar WRITE setShowTitleBar NOTIFY showTitleBarChanged)
    Q_PROPERTY(int statusDateTimeType READ getStatusDateTimeType WRITE setStatusDateTimeType)

    Q_INVOKABLE TagInfo* searchTagInfo() const {
        return &searchTI;
    }

    Q_INVOKABLE QString getDefaultOutputDir() const;
    Q_INVOKABLE QString getHumanReadableSearchPath() const;
    Q_INVOKABLE QString getLanguage() const;
    Q_INVOKABLE QString getSearchPath() const;
    Q_INVOKABLE QString getTmpDir() const;
    Q_INVOKABLE bool getLocalizedDateTimeFormat() const;
    Q_INVOKABLE bool getOpenLastDirectoryOnStartup() const;
    Q_INVOKABLE bool getShowFileNameExtensionWhenRename() const;
    Q_INVOKABLE bool getShowScrollActions() const;
    Q_INVOKABLE bool getShowTitleBar() const;
    Q_INVOKABLE void read();
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveSearchTagInfo();
    Q_INVOKABLE void setLanguage(const QString& newLanguage);
    Q_INVOKABLE void setLocalizedDateTimeFormat(bool set);
    Q_INVOKABLE void setOpenLastDirectoryOnStartup(bool set);
    Q_INVOKABLE void setSearchPath(const QString& newPath);
    Q_INVOKABLE void setShowDescriptionInMainViewForDirs(bool set);
    Q_INVOKABLE void setShowDescriptionInMainViewForFiles(bool set);
    Q_INVOKABLE void setShowFileNameExtensionInMainView(bool set);
    Q_INVOKABLE void setShowFileNameExtensionWhenRename(bool set);
    Q_INVOKABLE void setShowScrollActions(bool set);
    Q_INVOKABLE void setShowStatusInMainView(bool set);
    Q_INVOKABLE void setShowTitleBar(bool set);
    Q_INVOKABLE void setStatusDateTimeType(int type);

    Q_INVOKABLE inline  int getStatusDateTimeType ()const {
        return statusDateTimeType;
    }

    Q_INVOKABLE inline  bool getShowFileNameExtensionInMainView() const {
        return showFileNameExtensionInMainView;
    }

    Q_INVOKABLE inline  bool getShowDescriptionInMainViewForFiles() const {
        return showDescriptionInMainViewForFiles;
    }

    Q_INVOKABLE inline  bool getShowDescriptionInMainViewForDirs() const {
        return showDescriptionInMainViewForDirs;
    }

    Q_INVOKABLE inline  bool getShowStatusInMainView() const {
        return showStatusInMainView;
    }

    static QString appRootPath();

    inline QString dateTimeI18n(const QDateTime& dateTime) {
        if (localizedDateTimeFormat) {
            return dt.format(dateTime);
        }
        return dateTime.toString(Settings::dateTimeFormat());
    }

    inline static QString dateTimeFormat() {
        return "dd.MM.yyyy hh:mm";
    }

    int getTextFormat() const {
        return textFormat;
    }
    void setTextFormat(int value) {
        textFormat = value;
    }

    int getFontSize() const {
        return fontSize;
    }
    void setFontSize(int value) {
        fontSize = value;
    }

    int getFontStyle() const {
        return fontStyle;
    }
    void setFontStyle(int value) {
        fontStyle = value;
    }

    int getFontWeight() const {
        return fontWeight;
    }
    void setFontWeight(int value) {
        fontWeight = value;
    }

    bool getAllowEditNoMultimedia() const;
    bool getBoxBlackBerry() const;
    bool getDropBoxBlackBerry() const;
    bool getOneDriveBlackBerry() const;
    void setAllowEditNoMultimedia(bool set);
    void setBoxBlackBerry(bool set);
    void setDropBoxBlackBerry(bool set);
    void setOneDriveBlackBerry(bool set);

signals:
    void showTitleBarChanged(bool set);
    void dropBoxBlackBerryChanged(bool set);
    void oneDriveBlackBerryChanged(bool set);
    void boxBlackBerryChanged(bool set);

protected:
    Settings ();
    Settings (const Settings & settings);
    Settings & operator= (const Settings &);

    QString language;
    QString searchPath;
    bool localizedDateTimeFormat;
    bool openLastDirectoryOnStartup;
    bool showDescriptionInMainViewForDirs;
    bool showDescriptionInMainViewForFiles;
    bool showFileNameExtensionInMainView;
    bool showFileNameExtensionWhenRename;
    bool showScrollActions;
    bool showStatusInMainView;
    bool showTitleBar;
    int statusDateTimeType;
    mutable TagInfo searchTI;
    static AppDateFormatter dt;

    // CLOUDS
    bool boxBlackBerry;
    bool dropBoxBlackBerry;
    bool oneDriveBlackBerry;

    bool allowEditNoMultimedia;

    // EDITOR
    int fontSize;
    int fontStyle;
    int fontWeight;
    int textFormat; // Plain = 1, Html = 2, Auto = 3
};

#endif // SETTINGS_H
