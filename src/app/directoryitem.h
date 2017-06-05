#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include <QString>
#include <QDateTime>
#include <QVector>

class DirectoryItem;
typedef QVector<DirectoryItem> DirectoryItemList;

class DirectoryItem
{
public:
    DirectoryItem();
    DirectoryItem(const QString& path, const QString& name, const QDateTime& timestamp);

    static void read(const QString& name, DirectoryItemList& directoryItemList);
    static void save(const QString& name, const DirectoryItemList& directoryItemList);

    QString name;
    QString path;
    QDateTime timestamp;
};

#endif // DIRECTORYITEM_H
