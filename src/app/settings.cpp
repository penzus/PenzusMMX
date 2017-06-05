#include <bb/cascades/Application>
#include <QSettings>
#include <QDir>

#include "applicationui.h"
#include "settings.h"
#include "fileutils.h"
#include "common.h"

AppDateFormatter Settings::dt;
Settings & Settings::instance ()
{
    static Settings theSingleInstance;
    return theSingleInstance;
}

void Settings::saveSearchTagInfo()
{
    searchTI.save();
}

QString Settings:: appRootPath()
{
    return  "/accounts/1000/shared";
}

Settings::Settings()
{
    dt.setSkeleton("ddMMyyyyhhmm");
    dropBoxBlackBerry = false;
    oneDriveBlackBerry = false;
    boxBlackBerry = false;
    allowEditNoMultimedia = false;
    textFormat = 3;
    fontSize = 0;
    fontStyle = 0;
    fontWeight = 0;
    language = "";
    localizedDateTimeFormat = false;
    showTitleBar = true;
    statusDateTimeType = 0;
    showFileNameExtensionInMainView = true;
    showDescriptionInMainViewForFiles = true;
    showDescriptionInMainViewForDirs = false;
    showStatusInMainView = true;
    showFileNameExtensionWhenRename = false;
    openLastDirectoryOnStartup = false;
    showScrollActions = false;
    searchPath = appRootPath();
}

void Settings::read()
{
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("MainSettings");
    language = settings.value("language", language).toString();
    searchPath = settings.value("searchPath", searchPath).toString();
    localizedDateTimeFormat = settings.value("localizedDateTimeFormat", localizedDateTimeFormat).toBool();
    showScrollActions = ( settings.value("showScrollActions", showScrollActions).toBool() );
    setShowTitleBar ( settings.value("showTitleBar", showTitleBar).toBool() );
    showFileNameExtensionInMainView = settings.value("showFileNameExtensionInMainView", showFileNameExtensionInMainView).toBool();
    showDescriptionInMainViewForFiles = settings.value("showDescriptionInMainViewForFiles", showDescriptionInMainViewForFiles).toBool();
    showDescriptionInMainViewForDirs = settings.value("showDescriptionInMainViewForDirs", showDescriptionInMainViewForDirs).toBool();
    statusDateTimeType = settings.value("statusDateTimeType", statusDateTimeType).toInt();
    showStatusInMainView = settings.value("showStatusInMainView", showStatusInMainView).toBool();
    showFileNameExtensionWhenRename = settings.value("showFileNameExtensionWhenRename", showFileNameExtensionWhenRename).toBool();
    openLastDirectoryOnStartup = settings.value("openLastDirectoryOnStartup", openLastDirectoryOnStartup).toBool();
    dropBoxBlackBerry = settings.value("dropBoxBlackBerry", dropBoxBlackBerry).toBool();
    boxBlackBerry = settings.value("boxBlackBerry", boxBlackBerry).toBool();
    oneDriveBlackBerry = settings.value("oneDriveBlackBerry", oneDriveBlackBerry).toBool();
    allowEditNoMultimedia = settings.value("allowEditNoMultimedia", allowEditNoMultimedia).toBool();
    fontSize = settings.value("fontSize", fontSize).toInt();
    fontStyle = settings.value("fontStyle", fontStyle).toInt();
    fontWeight = settings.value("fontWeight", fontWeight).toInt();
    textFormat = settings.value("textFormat", textFormat).toInt();

    settings.endGroup();
    QDir d;
    d.mkpath(getDefaultOutputDir());
    searchTI.read();
}

QString Settings::getTmpDir() const
{
    return QString("tmp");
}

QString Settings::getDefaultOutputDir() const
{
    return QString("/accounts/1000/shared/misc/%1").arg(SHORT_APP_NAME);
}

void Settings::save()
{
    QSettings settings(ORGANIZATION, SHORT_APP_NAME);
    settings.beginGroup("MainSettings");
    settings.setValue("language",language);
    settings.setValue("searchPath",searchPath);
    settings.setValue("localizedDateTimeFormat",localizedDateTimeFormat);
    settings.setValue("showScrollActions",showScrollActions);
    settings.setValue("showTitleBar",showTitleBar);
    settings.setValue("showFileNameExtensionInMainView",showFileNameExtensionInMainView);
    settings.setValue("showDescriptionInMainViewForFiles",showDescriptionInMainViewForFiles);
    settings.setValue("showDescriptionInMainViewForDirs",showDescriptionInMainViewForDirs);
    settings.setValue("statusDateTimeType",statusDateTimeType);
    settings.setValue("showStatusInMainView",showStatusInMainView);
    settings.setValue("showFileNameExtensionWhenRename",showFileNameExtensionWhenRename);
    settings.setValue("openLastDirectoryOnStartup",openLastDirectoryOnStartup);
    settings.setValue("dropBoxBlackBerry",dropBoxBlackBerry);
    settings.setValue("oneDriveBlackBerry",oneDriveBlackBerry);
    settings.setValue("boxBlackBerry",boxBlackBerry);
    settings.setValue("allowEditNoMultimedia",allowEditNoMultimedia);
    settings.setValue("fontSize", fontSize);
    settings.setValue("fontStyle", fontStyle);
    settings.setValue("fontWeight", fontWeight);
    settings.setValue("textFormat", textFormat);
    settings.endGroup();
}

QString Settings::getLanguage() const
{
    return language;
}

void Settings::setLanguage(const QString& newLanguage)
{
    language = newLanguage;
}

void Settings::setShowFileNameExtensionWhenRename(bool set)
{
    showFileNameExtensionWhenRename = set;
}

bool Settings::getShowFileNameExtensionWhenRename() const
{
    return showFileNameExtensionWhenRename;
}

void Settings::setShowStatusInMainView(bool set)
{
    showStatusInMainView = set;
}

void Settings::setShowFileNameExtensionInMainView(bool set)
{
    showFileNameExtensionInMainView = set;
}

void Settings::setShowDescriptionInMainViewForFiles(bool set)
{
    showDescriptionInMainViewForFiles = set;
}

void Settings::setShowDescriptionInMainViewForDirs(bool set)
{
    showDescriptionInMainViewForDirs = set;
}

QString Settings::getHumanReadableSearchPath() const
{
    return FileUtils::getHumanReadablePath( getSearchPath() );
}

QString Settings::getSearchPath() const
{
    return searchPath;
}

void Settings::setSearchPath(const QString& newPath)
{
    searchPath = newPath;
}

bool Settings::getLocalizedDateTimeFormat() const
{
    return localizedDateTimeFormat;
}

void Settings::setLocalizedDateTimeFormat(bool set)
{
    localizedDateTimeFormat = set;
}

void Settings::setOpenLastDirectoryOnStartup(bool set)
{
    openLastDirectoryOnStartup = set;
}

bool Settings::getOpenLastDirectoryOnStartup() const
{
    return openLastDirectoryOnStartup;
}

void Settings::setShowTitleBar(bool set)
{
    showTitleBar = set;
    emit showTitleBarChanged(showTitleBar);
}

bool Settings::getShowTitleBar() const
{
    return showTitleBar;
}

void Settings::setShowScrollActions(bool set)
{
    showScrollActions = set;
}

bool Settings::getShowScrollActions() const
{
    return showScrollActions;
}

void Settings::setStatusDateTimeType(int type)
{
    statusDateTimeType = type;
}

void Settings::setDropBoxBlackBerry(bool set)
{
    dropBoxBlackBerry = set;
    emit dropBoxBlackBerryChanged(set);
}

void Settings::setOneDriveBlackBerry(bool set)
{
    oneDriveBlackBerry = set;
    emit oneDriveBlackBerryChanged(set);
}

void Settings::setBoxBlackBerry(bool set)
{
    boxBlackBerry = set;
    emit boxBlackBerryChanged(set);
}

bool Settings::getDropBoxBlackBerry() const
{
    return dropBoxBlackBerry;
}

bool Settings::getOneDriveBlackBerry() const
{
    return oneDriveBlackBerry;
}

bool Settings::getBoxBlackBerry() const
{
    return boxBlackBerry;
}

bool Settings::getAllowEditNoMultimedia() const
{
    return allowEditNoMultimedia;
}

void Settings::setAllowEditNoMultimedia(bool set)
{
    allowEditNoMultimedia = set;
}
