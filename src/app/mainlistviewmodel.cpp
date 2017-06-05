#include "applicationui.h"
#include "mainlistviewmodel.h"
#include "settings.h"
#include "fileutils.h"
#include "filesysteminfo.h"

using namespace bb::cascades;

MainListViewModel::MainListViewModel(QObject *parent) : BaseItemsModel(parent)
{
    settings = &(Settings::instance());

    searchFilterPrepared = false;
    dirFlags = QDir::Files | QDir::Dirs |  QDir::NoDot  | QDir::NoSymLinks | QDir::Readable | QDir::NoDotDot;

    showDescriptionInMainViewForFiles = true;
    showDescriptionInMainViewForDirs = false;

    showStatusInMainView = true;
    showFileNameExtensionInMainView = true;
    isTopLevelDir = false;

    setSortingAndOrder();


    watchTimer = new QTimer(this);
    watchTimer->setSingleShot(true);
    connect(watchTimer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

    fileSystemWatcher = new QFileSystemWatcher(this);
    connect(fileSystemWatcher, SIGNAL(directoryChanged(const QString& )), this, SLOT(directoryChanged(const QString&)));
}

QString MainListViewModel::dataAsAbsoluteFilePath(const QVariantList &indexPath, bool addURI ) const
{
    return (((addURI) ? "file://" : "") + data(indexPath, "absoluteFilePath"));
}

bool MainListViewModel::cdUp()
{
    return fileSystem.cd("..");
}
bool MainListViewModel::cd(const QString& path)
{
    return fileSystem.cd(path);
}

QString MainListViewModel::currentDirectoryName() const
{
    return FileUtils::fileName(FileUtils::getHumanReadablePath((fileSystem.currentDir.absolutePath())));
}

QString MainListViewModel::currentPath(bool humanReadable ) const
{
    return humanReadable ? FileUtils::getHumanReadablePath(fileSystem.currentDir.absolutePath())  : fileSystem.currentDir.absolutePath();
}

bool MainListViewModel::currentPathIsTopLevelDeviceDir() const
{
    return fileSystem.currentPathIsTopLevelDeviceDir();
}

bool MainListViewModel::currentPathIsTopLevelDir() const
{
    return fileSystem.currentPathIsTopLevelDir();
}

QString MainListViewModel::getStatusBarText() const
{
    return ( tr("Folders") + QString(": ") + QString::number(dirsCount)
             + QString(", ") + tr("files") + ": " + QString::number(filesCount) );

}

void MainListViewModel::clearFilter(bool restoreFromCache)
{
    if (restoreFromCache) {
        setFilter(QString());
    }
    cachedItemsForFilter.clear();
    searchFilterPrepared = false;
}

void MainListViewModel::prepareFilter()
{
    cachedItemsForFilter.clear();
    for ( QVariantList indexPath = groupDataModel->first(); !indexPath.isEmpty(); indexPath = groupDataModel->after(indexPath) ) {
        cachedItemsForFilter.append(groupDataModel->data(indexPath).toMap());
    }
    searchFilterPrepared = true;
}

void MainListViewModel::setFilter(const QString& searchString)
{
    if (!searchFilterPrepared) {
        return;
    }

    filesCount = 0;
    dirsCount = 0;

    bool useFilter = true;
    if (searchString.size() == 0) {
        useFilter = false;
    }
    groupDataModel->clear();
    for (int i = 0; i < cachedItemsForFilter.size(); i++) {
        QVariantMap  entry =  cachedItemsForFilter.at(i);
        if (useFilter) {
            if (  !entry.value("title").toString().contains(searchString, Qt::CaseInsensitive) ) {
                continue;
            }
        }
        switch (entry.value("type").toInt()) {
        case Directory:
            dirsCount++;
            break;
        case File:
            filesCount++;
            break;
        default:
            ;
        }

        updateOrInsertEntry(0, entry, false);
    }
    emit statusBarTextChanged(getStatusBarText());
}

void MainListViewModel::postDeleteItemsProccessing()
{
    filesCount = 0;
    dirsCount = 0;

    fileSystem.currentDir.refresh();
    FileNamesHash tmpHash;

    QFileInfoList list = fileSystem.currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fi = list.at(i);

        bool fileNameAlreadyExists = tmpHash.value(fi.fileName());
        if (fileNameAlreadyExists) {
            continue;
        }

        if (fi.isDir()) {
            dirsCount++;
        }
        if (fi.isFile()) {
            filesCount++;
        }
        tmpHash.insert(fi.fileName(), true);
    }
    emit statusBarTextChanged(getStatusBarText());
}

void MainListViewModel::setSortingAndOrder()
{
    QStringList sortingKeys;
    groupDataModel->setGrouping(ItemGrouping::None);
    sortingKeys << "type" << "title";
    groupDataModel->setSortedAscending(true);
    groupDataModel->setSortingKeys(sortingKeys);
}

void MainListViewModel::updateModel()
{
    QTime time;
    time.start();

    watchTimer->stop();

    filesCount = 0;
    dirsCount = 0;
    itCount =0;

    fileNamesHash.clear();
    groupDataModel->clear();

    isTopLevelDir = FileUtils::isTopLevelDir(fileSystem.currentDir.absolutePath());

    showDescriptionInMainViewForFiles = settings->getShowDescriptionInMainViewForFiles();
    showDescriptionInMainViewForDirs = (settings->getShowDescriptionInMainViewForDirs() && fileSystem.currentPathIsMountedLocalFileSystem());
    showStatusInMainView = settings->getShowStatusInMainView();
    showFileNameExtensionInMainView = settings->getShowFileNameExtensionInMainView();


    QDir::Filters currentDirFlags = dirFlags;
    fileSystem.currentDir.setFilter(currentDirFlags);

    QFileInfoList list = fileSystem.currentDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fi = list.at(i);
        if (!addItemPrivate(fi)) {
            continue;
        }
    }
    emit statusBarTextChanged(getStatusBarText());
    installFileSystemWatcher(fileSystem.currentDir.absolutePath());
}

void MainListViewModel::directoryChanged(const QString&)
{
    watchTimer->start(1000);
}

void MainListViewModel::stopFileSystemWatcher()
{
    fileSystemWatcher->removePaths(fileSystemWatcher->directories());
}

void MainListViewModel::timerUpdate()
{
    if (fileSystemWatcher->directories().length() == 0) {
        return;
    }
    updateModel();
}

void MainListViewModel::installFileSystemWatcher(const QString& path)
{
    watchTimer->stop();
    if (fileSystemWatcher->directories().length() > 0) {
        fileSystemWatcher->removePaths(fileSystemWatcher->directories());
    }
    fileSystemWatcher->addPath(path);
}

void MainListViewModel::deleteMultipleItems(const QStringList& array, const QStringList& failItems )
{
    bool failured = false;
    for (int i = 0; i < array.size(); i++) {

        bool foundInFail = false;
        for (int j = 0; j < failItems.size(); j++) {
            if (array.at(i) == failItems.at(j)) {
                foundInFail = true;
                break;
            }
        }
        if (!foundInFail) {
            if (!deleteItemPrivate(array.at(i))) {
                failured = true;
            }
        }
    }
    if (failured) {
        updateModel();
    }
}

void MainListViewModel::deleteItem(const QString& fileNamePath)
{
    if (!deleteItemPrivate(fileNamePath)) {
        updateModel();
    }
}

void MainListViewModel::addItem(const QString& fileNamePath)
{
    QFileInfo fi(fileNamePath);
    if (fi.exists()) {
        addItemPrivate(fi);
    } else { // safe check
        updateModel();
    }
}

QString MainListViewModel::path(const QVariantList &indexPath) const
{
    return BaseItemsModel::data(indexPath, "absoluteFilePath");
}

bool MainListViewModel::isDir(const QVariantList &indexPath) const
{
    if (!indexPath.isEmpty()) {
        const QVariantMap entry = groupDataModel->data(indexPath).toMap();
        return (entry.value("type").toInt() == Directory) ? true : false;
    }
    return false;
}

void MainListViewModel::renameItem(const QString& oldFileNamePath, const QString& newFileNamePath)
{
    QFileInfo fi(newFileNamePath);
    if (fi.exists()) {
        QString value = "absoluteFilePath";
        for (int i = 0; i < groupDataModel->size(); i++ ) {
            QVariantList indexPath;
            indexPath << i;
            QVariantMap entry = groupDataModel->data(indexPath).toMap();
            QString string = entry.value(value).toString();
            if (string == oldFileNamePath) {
                entry["title"] = fi.fileName();
                entry["absoluteFilePath"] = fi.absoluteFilePath();

                if (fi.isFile()) {
                    QString mimeImageFileName;
                    FileUtils::mimeTypeImageFromFileNamePath( fi.fileName(), mimeImageFileName);
                    entry["imageSource"] = mimeImageFileName;
                }

                updateOrInsertEntry(i, entry, true);
                QFile oldFi(oldFileNamePath);
                if ( fileNamesHash.remove(FileUtils::fileName(oldFi.fileName())) > 0) {
                    fileNamesHash.insert(fi.fileName(), true);
                } else { // fail?
                    updateModel();
                    return;
                }
                return;
            }
        }
    } else { // safe check
        updateModel();
    }
}

bool MainListViewModel::calculateDirectoryItemsCount(const QString& directoryPath, quint64& dirsCount, quint64& filesCount)
{
    dirsCount = 0;
    filesCount = 0;

    QHash<QString, bool> privateFileNamesHash;
    QDir dir(directoryPath);
    dir.setFilter(dirFlags);
    QFileInfoList list = dir.entryInfoList();
    quint64 itemsCount = 0;
    for (int i = 0; i < list.size(); ++i) {
        const QFileInfo& fi = list[i];
        bool fileNameAlreadyExists = privateFileNamesHash.value(fi.fileName());
        if (fileNameAlreadyExists) {
            continue;
        }
        if (fi.isDir()) dirsCount++;
        if (fi.isFile()) filesCount++;


        privateFileNamesHash.insert(fi.fileName(), true);
        itemsCount++;
    }
    return itemsCount;
}

bool MainListViewModel::deleteItemPrivate(const QString& fileNamePath)
{
    QString value = "absoluteFilePath";
    for (int i = 0; i < groupDataModel->size(); i++ ) {
        QVariantList indexPath;
        indexPath << i;
        QVariantMap entry = groupDataModel->data(indexPath).toMap();
        QString string = entry.value(value).toString();
        if (string == fileNamePath) {
            groupDataModel->removeAt(indexPath);

            quint64 dirsCountDeleted = 0;
            quint64 filesCountDeleted = 0;
            fileNamesHash.remove(FileUtils::fileName(fileNamePath));
            dirsCount  -= dirsCountDeleted;
            filesCount -= filesCountDeleted;

            return true;
        }
    }
    return false;
}

bool MainListViewModel::addItemPrivate(const QFileInfo& fi)
{
    QString mimeImageFileName;
    bool fileNameAlreadyExists = fileNamesHash.value(fi.fileName());
    if (fileNameAlreadyExists) {
        return false;
    }

    QVariantMap entry;
    entry["id"] = itCount++;

    if (fi.isDir()) {
        if (isTopLevelDir) {
            QString fileName = fi.fileName();
            if (fileName == "downloads") entry["imageSource"] = "asset:///images/listview/places/folder-downloads.png";
            else if (fileName == "documents") entry["imageSource"] = "asset:///images/listview/places/folder-documents.png";
            else if (fileName == "videos") entry["imageSource"] = "asset:///images/listview/places/folder-video.png";
            else if (fileName == "print") entry["imageSource"] = "asset:///images/listview/places/folder-print.png";
            else if (fileName == "camera") entry["imageSource"] = "asset:///images/listview/places/folder-image.png";
            else if (fileName == "photos") entry["imageSource"] = "asset:///images/listview/places/folder-image.png";
            else if (fileName == "music") entry["imageSource"] = "asset:///images/listview/places/folder-sound.png";
            else if (fileName == "voice") entry["imageSource"] = "asset:///images/listview/places/folder-sound.png";
            else if (fileName == "misc") entry["imageSource"] = "asset:///images/listview/places/folder-activities.png";
            else if (fileName == "books") entry["imageSource"] = "asset:///images/listview/places/folder-bookmark.png";
            else entry["imageSource"] = "asset:///images/listview/places/folder.png";
        } else {
            entry["imageSource"] = "asset:///images/listview/places/folder.png";
        }
        entry["type"] = Directory;
        entry["title"] = fi.fileName();
        if (showDescriptionInMainViewForDirs) {
            quint64 dirsCount;
            quint64 filesCount;
            bool haveItemsCount = calculateDirectoryItemsCount( fi.absoluteFilePath(), dirsCount, filesCount);
            if (!haveItemsCount) {
                entry["description"] = tr("Empty folder");
            } else {
                QString result;
                if (dirsCount) {
                    result = tr("Folders") + QString(": %1").arg(dirsCount);
                    if (filesCount) {
                        entry["description"] = result + QString(", ") + tr("files") + QString(": %1").arg(filesCount);
                    } else {
                        entry["description"] = result;
                    }
                } else {
                    entry["description"] = tr("Files") + QString(": %1").arg(filesCount);
                }
            }

        }
        dirsCount++;

    } else { // regular file
        FileUtils::mimeTypeImageFromFileNamePath( fi.fileName(), mimeImageFileName);
        entry["imageSource"] = mimeImageFileName;
        entry["type"] = File;
        entry["title"] = showFileNameExtensionInMainView ? fi.fileName() :  fi.completeBaseName();
        if (showDescriptionInMainViewForFiles) {
            entry["description"] = FileUtils::sizeToString( fi.size() );
        }
        filesCount++;
    }

    entry["absoluteFilePath"] = fi.absoluteFilePath();
    if (showStatusInMainView) {
        entry["status"] = settings->dateTimeI18n(fi.created());
    }

    fileNamesHash.insert(fi.fileName(), true);

    updateOrInsertEntry(0, entry, false);
    return true;
}
