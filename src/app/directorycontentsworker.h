#ifndef DIRECTORYCONTENTSWORKER_H
#define DIRECTORYCONTENTSWORKER_H

#include <QThread>
#include <QStringList>

class DirectoryContentsWorker : public QThread
{
    Q_OBJECT
public:
    QString getPath() const;
    quint64 directoriesCount() const;
    quint64 filesCount() const;
    quint64 totalSize() const;
    void run();
    void setContents(const QStringList& paths);

signals:
    void currentData(quint64 totalSize, quint64 directoriesCount, quint64 filesCount);
    void selectedItems(quint64 directoriesCount, quint64 filesCount);

protected:
    QStringList paths;
    quint64 dirCount;
    quint64 filCount;
    quint64 totSize;
};

#endif // DIRECTORYCONTENTSWORKER_H
