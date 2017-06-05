#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDebug>

#include "directoryitem.h"
#include "common.h"

DirectoryItem::DirectoryItem()
{

}
DirectoryItem::DirectoryItem(const QString& p, const QString& n, const QDateTime& t)
{
    path = p;
    name = n;
    timestamp = t;
}

void DirectoryItem::read(const QString& name, DirectoryItemList& directoryItemList)
{
    directoryItemList.clear();
    QSettings settings(ORGANIZATION, name);
    if (!QFile::exists(settings.fileName())) {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        DirectoryItem directoryItem;

        directoryItem.name =  QObject::tr("Device");
        directoryItem.path = "/accounts/1000/shared";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        directoryItem.name =   QObject::tr("SD card");
        directoryItem.path = "/accounts/1000/removable/sdcard";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        directoryItem.name =   QObject::tr("Images");
        directoryItem.path = "/accounts/1000/shared/camera";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        directoryItem.name =   QObject::tr("Videos");
        directoryItem.path = "/accounts/1000/shared/videos";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        directoryItem.name =   QObject::tr("Music");
        directoryItem.path = "/accounts/1000/shared/music";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        directoryItem.name =   QObject::tr("Downloads");
        directoryItem.path = "/accounts/1000/shared/downloads";
        directoryItem.timestamp = currentDateTime;
        directoryItemList.append(directoryItem);

        save(name, directoryItemList);
        return;
    }

    QStringList groupsList = settings.childGroups();
    for (int i =0; i < groupsList.size(); i++) {
        settings.beginGroup(groupsList[i]);
        DirectoryItem directoryItem;
        directoryItem.name = settings.value("name", QString()).toString();
        directoryItem.path = settings.value("path", QString()).toString();
        directoryItem.timestamp = settings.value("timestamp", QDateTime()).toDateTime();
        if (!directoryItem.name.isEmpty() ) {
            directoryItemList.append(directoryItem);
        }
        settings.endGroup();
    }
}

void DirectoryItem::save(const QString& name, const DirectoryItemList& directoryItemList)
{
    QSettings settings(ORGANIZATION, name);
    settings.clear();
    for (int i =0; i < directoryItemList.size(); i++) {
        settings.beginGroup( name + QString("-") + QString::number(i+1));
        settings.setValue("name",directoryItemList[i].name);
        settings.setValue("path",directoryItemList[i].path);
        settings.setValue("timestamp",directoryItemList[i].timestamp);
        settings.endGroup();
    }
}
