#include "applicationui.h"
#include "favoriteslistviewmodel.h"
#include "settings.h"
#include "fileutils.h"

using namespace bb::cascades;

FavoritesListViewModel::FavoritesListViewModel(const QString& name, QObject *parent) : BaseItemsModel(parent)
{
    internalName = name;
    DirectoryItem::read(internalName,favoritesList);
    itCount = favoritesList.size();

    if ( (internalName == "history") ) {
        groupDataModel->setSortedAscending(false);
        QStringList sl;
        sl <<  "timestamp";
        groupDataModel->setSortingKeys(sl);
    }
}

void FavoritesListViewModel::updateModel()
{
    groupDataModel->clear();
    for (int i = 0; i < favoritesList.size(); i++) {
        QVariantMap entry;
        entry["id"] = i;
        entry["name"] = favoritesList[i].name;
        entry["absoluteFilePath"] = favoritesList[i].path;
        entry["humanReadablePath"] = FileUtils::getHumanReadablePath(favoritesList[i].path );
        entry["timestamp"] = favoritesList[i].timestamp;
        entry["status"] = Settings::instance().dateTimeI18n(favoritesList[i].timestamp);
        groupDataModel->insert(entry);
    }
}

bool FavoritesListViewModel::deleteItemPrivate(const QString& fileNamePath)
{
    QString value = "absoluteFilePath";
    for (int i = 0; i < groupDataModel->size(); i++ ) {
        QVariantList indexPath;
        indexPath << i;
        QVariantMap entry = groupDataModel->data(indexPath).toMap();
        QString string = entry.value(value).toString();
        if (string == fileNamePath) {
            for (int j =0; j< favoritesList.size(); j++) {
                if (favoritesList.at(j).path == fileNamePath) {
                    groupDataModel->removeAt(indexPath);
                    favoritesList.remove(j);
                }
            }
            return true;
        }
    }
    return false;
}

void FavoritesListViewModel::deleteItem(const QString& path)
{
    if (!deleteItemPrivate(path)) {
        updateModel();
    }
    DirectoryItem::save(internalName,favoritesList);
    itCount = favoritesList.size();
}

void FavoritesListViewModel::updateItem(const QString& oldName, const QString& oldPath, const QString& newName, const QString& newPath )
{
    if ((oldName == newName) && (oldPath == newPath)) {
        return;
    }

    for (int i = 0; i < favoritesList.size(); i++) {
        if ( (favoritesList[i].path == oldPath) && (favoritesList[i].name == oldName)  ) {
            favoritesList.remove(i);
            break;
        }
    }
    addItem(newName, newPath);
}


void FavoritesListViewModel::addItem(const QString& name, const QString& path)
{
    DirectoryItem directoryItem(path, name, QDateTime::currentDateTime());
    if (directoryItem.name.isEmpty() || directoryItem.path.isEmpty()) {
        return;
    }
    for (int i = 0; i < favoritesList.size(); i++) {
        if (favoritesList[i].path == directoryItem.path) {
            favoritesList.remove(i);
            break;
        }
    }
    favoritesList.append(directoryItem);
    if ( (internalName == "history") && (favoritesList.size() > 100) ) {
        favoritesList.remove(0);
    }
    DirectoryItem::save(internalName,favoritesList);
    itCount = favoritesList.size();
    updateModel();
}

void FavoritesListViewModel::addItem(const QString& path)
{
    for (int i = 0; i < favoritesList.size(); i++) {
        if (favoritesList[i].path == path) {
            favoritesList.remove(i);
            break;
        }
    }
    DirectoryItem item;
    item.name = FileUtils::fileName(FileUtils::getHumanReadablePath(path));
    item.path = path;
    item.timestamp = QDateTime::currentDateTime();
    favoritesList.append(item);

    if ( (internalName == "history") && (favoritesList.size() > 100) ) {
        favoritesList.remove(0);
    }

    DirectoryItem::save(internalName,favoritesList);
    itCount = favoritesList.size();
    updateModel();
}

void FavoritesListViewModel::clear()
{
    favoritesList.clear();
    DirectoryItem::save(internalName,favoritesList);
    itCount = favoritesList.size();
    BaseItemsModel::clear();
}

QString FavoritesListViewModel::lastItemByTimestamp() const
{
    if (favoritesList.size() > 0) {
        int foundIndex = 0;
        for (int i = 0; i < favoritesList.size(); i++) {
            if (favoritesList.at(i).timestamp > favoritesList.at(foundIndex).timestamp) {
                foundIndex = i;
            }
        }
        return favoritesList.at(foundIndex).path;
    }
    return QString();
}
