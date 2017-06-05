#ifndef FAVORITESLISTVIEWMODEL_H
#define FAVORITESLISTVIEWMODEL_H

#include "baseitemsmodel.h"
#include "directoryitem.h"

class FavoritesListViewModel : public BaseItemsModel
{
    Q_OBJECT
public:
    FavoritesListViewModel(const QString& name, QObject *parent = 0);

    Q_INVOKABLE QString lastItemByTimestamp() const;
    Q_INVOKABLE void addItem(const QString& name, const QString& path);
    Q_INVOKABLE void addItem(const QString& path);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void deleteItem(const QString& path);
    Q_INVOKABLE void updateItem(const QString& oldName, const QString& oldPath, const QString& newName, const QString& newPath);
    Q_INVOKABLE void updateModel();

protected:
    bool deleteItemPrivate(const QString& path);

    DirectoryItemList favoritesList;
    QString internalName;
};

#endif // FAVORITESLISTVIEWMODEL_H
