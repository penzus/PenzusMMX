#include <QDebug>
#include <bb/cascades/AbstractPane>

#include "fileutils.h"
#include "filesysteminfo.h"
#include "applicationui.h"

FileSystemInfo& FileSystemInfo::instance ()
{
    static FileSystemInfo theSingleInstance;
    return theSingleInstance;
}

FileSystemInfo::FileSystemInfo()
{
    cardMountedStatus = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(10 * 1000);
    timerUpdate();
}

void FileSystemInfo::timerUpdate()
{
    update();
}

void FileSystemInfo::update()
{
    updateItem("/accounts/1000", 0);
    cardMountedStatus = (cardInfo.state() == bb::device::SdCardState::Mounted) ? true : false;
    emit cardMounted(cardMountedStatus);
    updateItem("/accounts/1000/removable/sdcard/", 1);

}

bool FileSystemInfo::cardIsMounted() const
{
    return cardMountedStatus;
}

void FileSystemInfo::updateItem(const QString& path, int index)
{
    quint64 total = fileSystemInfo.fileSystemCapacity(path);
    quint64 available = fileSystemInfo.availableFileSystemSpace(path);
    quint64 used =  total - available;

    QString totalMemory = FileUtils::sizeToString(total,false);
    QString usedMemory  =  FileUtils::sizeToString(used,false);

    QString totalMemoryA = FileUtils::sizeToString(total,true);
    QString usedMemoryA  =  FileUtils::sizeToString(used,true);

    int percent = ceil ((used * 100 )/total);
    QString text = (tr("Used") + QString(" ") + usedMemoryA + QString(" ") + tr("of") + QString(" ") + totalMemoryA);

    emit dataChanged(index, percent, FileUtils::sizeToString(available,true));
}

void FileSystemInfo::getUsage(const QString& path)
{
    quint64 total = fileSystemInfo.fileSystemCapacity(path);
    quint64 available = fileSystemInfo.availableFileSystemSpace(path);
    quint64 used =  total - available;

    totalMemoryPri = FileUtils::sizeToString(total,false);
    usedMemoryPri  =  FileUtils::sizeToString(used,false);

    QString totalMemoryA = FileUtils::sizeToString(total,true);
    QString usedMemoryA  =  FileUtils::sizeToString(used,true);

    percentPri = QString::number(ceil ((used * 100 )/total));
    textPri = (tr("Used") + QString(" ") + usedMemoryA + QString(" ") + tr("of") + QString(" ") + totalMemoryA);
}

QString FileSystemInfo::usedMemory() const
{
    return usedMemoryPri;
}

QString FileSystemInfo::totalMemory() const
{
    return totalMemoryPri;
}

QString FileSystemInfo::percent() const
{
    return percentPri;
}

QString FileSystemInfo::text() const
{
    return textPri;
}
