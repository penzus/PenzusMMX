#include <QDebug>
#include <QDir>
#include "fileoperationthread.h"
#include "fileutils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h> // for utimes
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <ctime>
#include <limits.h>
#include <cstdio>

unsigned long FileOperationThread::getPrefferedBlockSize(const QString& fileName)
{
    struct stat sb;
    QByteArray ba = fileName.toLocal8Bit();
    const char* argv = ba.data();

    if (stat(argv, &sb) == -1) {
        return 4096; // as default
    }
    return sb.st_blksize;
}

FileOperationThread::FileOperationThread()
{
    cancelFlag = false;
    timer = new QTimer(this);
    timer->setSingleShot(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
}

void FileOperationThread::prepare(const QStringList& list, const QString& tDir, int mode)
{
    fileOperationPaths = list;
    fileOperationMode = mode;
    targetDir = tDir;
    cancelFlag = false;
    progressPercent = 0;
    message.clear();
}

void FileOperationThread::run()
{
    if (fileOperationPaths.size() == 0) {
        return;
    }

    //1. Calculate source file size
    quint64 sourceSize = 0;
    quint64 processedSize = 0;
    QFileInfoList inputFileInfoList;
    QStringList failedFileList;
    bool useSystemMove = false;

    if (fileOperationMode == 2) {
        useSystemMove = FileUtils::directoryInTheSameDevice(targetDir);

        if (useSystemMove) {
            for (int i = 0; i < fileOperationPaths.size(); i++) {
                QFileInfo fi(fileOperationPaths.at(i));
                inputFileInfoList.append(fi);

                QString targetFileNamePath = inputFileInfoList.at(i).absoluteFilePath().replace(0,fi.absolutePath().size(),targetDir);
                QString sourceFileNameNamePath = inputFileInfoList.at(i).absoluteFilePath();

                QFile f(sourceFileNameNamePath);
                if (!f.rename(targetFileNamePath)) {
                    failedFileList.append(sourceFileNameNamePath);
                }
            }
            emit operationThreadFinished(failedFileList.size());
            return;
        } else {
            //
        }
    }

    for (int i = 0; i < fileOperationPaths.size(); i++) {
        QFileInfoList inputFileInfoListTmp;
        sourceSize += FileUtils::fileInfoSize(fileOperationPaths.at(i),inputFileInfoListTmp, true);
        inputFileInfoList += inputFileInfoListTmp;
    }

    quint64 availableSpace = FileUtils::availableFileSystemSpace(targetDir);
    if (sourceSize > availableSpace) {
        emit operationError(tr("Insufficient space left in source"));
        return;
    }

    QFileInfo fi(fileOperationPaths.at(0));
    QString sourcePath = fi.absolutePath();
    int sourcePathSize = sourcePath.size();
    QString sourcePathDataSizeString = FileUtils::sizeToString(sourceSize);
    {
        QMutexLocker mutexLocker(&mutex);
        message = FileUtils::sizeToString(0) + " " + tr("of") + " " + sourcePathDataSizeString;
    }
    timerUpdate();
    timer->start(1000);

    for (int i = 0; i < inputFileInfoList.size(); i++) {
        QString sourceFileNamePath = inputFileInfoList.at(i).absoluteFilePath();
        QString targetFileNamePath = inputFileInfoList.at(i).absoluteFilePath().replace(0,sourcePathSize,targetDir);

        if (inputFileInfoList.at(i).isDir()) {
            QDir d(targetFileNamePath);
            bool created = d.mkpath(targetFileNamePath);
            if (!created) {
                failedFileList << sourceFileNamePath;
            }
        } else {
            int progressPercentSav = progressPercent;
            QString messageSave = message;

            if (!fileCopy(sourceFileNamePath, targetFileNamePath, processedSize, sourceSize, sourcePathDataSizeString)) {
                QFileInfo fi(targetFileNamePath);
                QDir d(fi.absolutePath());
                d.mkpath(fi.absolutePath());
                if (!fileCopy(sourceFileNamePath, targetFileNamePath,processedSize, sourceSize, sourcePathDataSizeString)) {
                    failedFileList << sourceFileNamePath;

                    QMutexLocker mutexLocker(&mutex);
                    progressPercent = progressPercentSav;
                    message = messageSave;

                    continue;
                }
            }
        }

        {
            QMutexLocker mutexLocker(&mutex);

            processedSize += inputFileInfoList.at(i).size();
            if (sourceSize > 0) {
                progressPercent = (processedSize * 100)/sourceSize;
            }
            if (progressPercent > 100) {
                progressPercent = 0;
            }
            message = FileUtils::sizeToString(processedSize) + " " + tr("of") + " " + sourcePathDataSizeString ;
            if (cancelFlag) {
                break;
            }
        }
    }

    timer->stop();

    QMutexLocker mutexLocker(&mutex);
    if (cancelFlag) {
        return;
    }

    if (failedFileList.size() > 0) {
        if (fileOperationMode == 1) {
            emit operationError(tr("Cannot copy some files or directoies"));
        } else if (fileOperationMode == 2) {
            emit operationError(tr("Cannot move some files or directoies."));
        }
    } else {
        if (fileOperationMode == 2) { //
            QStringList failedMovedItems  = FileUtils::deleteFileItems(fileOperationPaths);
            if (failedMovedItems.size() > 0) {
                emit operationError(tr("Cannot delete some source files or directoies."));
                emit operationThreadFinished(failedMovedItems.size());
                return;
            }
        }
    }

    emit operationThreadFinished(failedFileList.size());
}

void FileOperationThread::timerUpdate()
{
    QMutexLocker mutexLocker(&mutex);
    emit operationStateChanged(progressPercent, message);
}

void FileOperationThread::cancelOperation()
{
    QMutexLocker mutexLocker(&mutex);
    cancelFlag = true;
}

bool FileOperationThread::fileCopy(const QString& sourceFileName, const QString& targetFileName, quint64 processedSizeTotal, quint64 sourceSizeTotal, const QString& sourcePathDataSizeString)
{
    int blockSize = getPrefferedBlockSize( sourceFileName );

    clock_t start, end;
    start = clock();

    char buf[blockSize];
    size_t size;

    int source = open( sourceFileName.toUtf8().constData(), O_RDONLY, 0);
    int dest = open( targetFileName.toUtf8().constData(), O_WRONLY | O_CREAT , 0644); ///*| O_TRUNC/**/

    if ((source < 0) || (dest < 0)) {
        close(source);
        close(dest);
        return false;
    }
    quint64 processedSize = processedSizeTotal;

    while ((size = read(source, buf, blockSize)) > 0) {
        size_t bytesWritten = write(dest, buf, size);
        {
            QMutexLocker mutexLocker(&mutex);
            if (cancelFlag) {
                close(source);
                close(dest);
                QFile::remove(targetFileName);
                return false;
            }

            if (bytesWritten >= 0) {
                processedSize += bytesWritten;
                if (sourceSizeTotal > 0) {
                    progressPercent = (processedSize * 100)/sourceSizeTotal;
                }
                if (progressPercent > 100) {
                    progressPercent = 0;
                }
                message = FileUtils::sizeToString(processedSize) + " " + tr("of") + " " + sourcePathDataSizeString ;
            }  else  {
                close(source);
                close(dest);
                QFile::remove(targetFileName);
                return false;
            }
        }
    }

    close(source);
    close(dest);

    utimes(targetFileName.toUtf8().constData(), NULL);

    end = clock();
    return true;
}
