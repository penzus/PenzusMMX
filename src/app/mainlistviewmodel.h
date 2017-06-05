#ifndef MAINLISTVIEWMODEL_H
#define MAINLISTVIEWMODEL_H

#include <QHash>
#include <QDir>
#include <QTimer>
#include <QFileSystemWatcher>

#include "baseitemsmodel.h"
#include "filesystem.h"

class Settings;

class MainListViewModel : public BaseItemsModel
{
    Q_OBJECT
public:
    MainListViewModel(QObject *parent = 0);
    void updateModel();

    Q_PROPERTY(QString statusBarText READ getStatusBarText NOTIFY statusBarTextChanged)

    Q_INVOKABLE QString currentDirectoryName() const;
    Q_INVOKABLE QString currentPath(bool humanReadable = true) const;
    Q_INVOKABLE QString dataAsAbsoluteFilePath(const QVariantList &indexPath, bool addURI = false) const;
    Q_INVOKABLE QString getStatusBarText() const;
    Q_INVOKABLE QString path(const QVariantList &indexPath) const;
    Q_INVOKABLE bool cd(const QString& path);
    Q_INVOKABLE bool cdUp();
    Q_INVOKABLE bool currentPathIsTopLevelDeviceDir() const;
    Q_INVOKABLE bool currentPathIsTopLevelDir() const;
    Q_INVOKABLE bool isDir(const QVariantList &indexPath) const;
    Q_INVOKABLE void addItem(const QString& fileNamePath);
    Q_INVOKABLE void clearFilter(bool restoreFromCache = true);
    Q_INVOKABLE void deleteItem(const QString& fileNamePath);
    Q_INVOKABLE void deleteMultipleItems(const QStringList& array, const QStringList& failItems);
    Q_INVOKABLE void postDeleteItemsProccessing();
    Q_INVOKABLE void prepareFilter();
    Q_INVOKABLE void renameItem(const QString& oldFileNamePath, const QString& newFileNamePath);
    Q_INVOKABLE void setFilter(const QString& string);
    Q_INVOKABLE void stopFileSystemWatcher();

signals:
    void statusBarTextChanged(QString message);

protected slots:
    void directoryChanged(const QString& path);
    void timerUpdate();

private:
    typedef QHash<QString, bool> FileNamesHash;
    enum Type {Unknow = 0, Directory = 1, File = 2};

    bool calculateDirectoryItemsCount(const QString& directoryPath, quint64& dirsCount, quint64& filesCount);
    bool deleteItemPrivate(const QString& fileNamePath);
    inline bool addItemPrivate(const QFileInfo& fi);
    void initializeData();
    void installFileSystemWatcher(const QString& path);
    void setSortingAndOrder();

    FileNamesHash fileNamesHash;
    FileSystem fileSystem;
    QDir::Filters dirFlags;
    QFileSystemWatcher* fileSystemWatcher;
    QList<QVariantMap> cachedItemsForFilter;
    QTimer* watchTimer;
    Settings* settings;
    bool isTopLevelDir;
    bool searchFilterPrepared;
    bool showDescriptionInMainViewForDirs;
    bool showDescriptionInMainViewForFiles;
    bool showFileNameExtensionInMainView;
    bool showStatusInMainView;
    quint64 dirsCount;
    quint64 filesCount;
};

#endif // MAINLISTVIEWMODEL_H
