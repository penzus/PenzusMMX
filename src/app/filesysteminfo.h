#ifndef FILESYSTEMINFO_H
#define FILESYSTEMINFO_H


#include <QTimer>

#include <bb/core/FileSystemInfo.hpp>
#include <bb/device/SdCardInfo.hpp>

class FileSystemInfo : public QObject
{
    Q_OBJECT
public:
    static FileSystemInfo & instance ();

    Q_INVOKABLE QString percent() const;
    Q_INVOKABLE QString text() const;
    Q_INVOKABLE QString totalMemory() const;
    Q_INVOKABLE QString usedMemory() const;
    Q_INVOKABLE bool cardIsMounted() const;
    Q_INVOKABLE void getUsage(const QString& path);
    Q_INVOKABLE void update();

signals:
    void cardMounted(bool mounted);
    void dataChanged(int index, int percentValue, QString addInfo);

protected slots:
    void timerUpdate();

protected:
    FileSystemInfo();
    FileSystemInfo (const FileSystemInfo &fsInfo);
    FileSystemInfo & operator= (const FileSystemInfo &);

    void updateItem(const QString& path, int index);

    QString percentPri;
    QString textPri;
    QString totalMemoryPri;
    QString usedMemoryPri;
    QTimer* timer;
    bb::FileSystemInfo fileSystemInfo;
    bb::device::SdCardInfo cardInfo;
    bool cardMountedStatus;
};

#endif // FILESYSTEMINFO_H
