#ifndef FILEOPERATIONTHREAD_H
#define FILEOPERATIONTHREAD_H

#include <QThread>
#include <QTimer>
#include <QStringList>
#include <QString>
#include <QMutex>


class FileOperationThread : public QThread
{
    Q_OBJECT
public:
    FileOperationThread();

    void cancelOperation();
    void prepare(const QStringList& list, const QString& targetDir, int mode);
    void run();

signals:
    void operationError(QString message);
    void operationStateChanged(int progressPercent, QString messageString);
    void operationThreadFinished(int status);

protected slots:
    void timerUpdate();

protected:
    bool fileCopy(const QString& source, const QString& target, quint64 processedSize, quint64 sourceSize,  const QString& sourcePathDataSizeString);
    unsigned long getPrefferedBlockSize(const QString& fileName);

    QMutex mutex;
    QString message;
    QString targetDir;
    QStringList fileOperationPaths;
    QTimer* timer;
    bool cancelFlag;
    int fileOperationMode;
    int progressPercent;
};

#endif // FILEOPERATIONTHREAD_H
