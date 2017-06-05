#include <QDirIterator>
#include <QDebug>

#include "directorycontentsworker.h"

void DirectoryContentsWorker::setContents(const QStringList& p)
{
    paths = p;
    totSize = 0;
    dirCount = 0;
    filCount = 0;
}

QString DirectoryContentsWorker::getPath() const
{
    if (paths.size() > 0) {
        QFileInfo fi(paths.at(0));
        return fi.path();
    }
    return QString();
}

void DirectoryContentsWorker::run()
{
    emit currentData(totSize, dirCount, filCount);
    QFileInfo fi;
    int emitCounter = 0;

    if (paths.size() > 1) {
        quint64 selectedDirectoriesCount = 0;
        quint64 selectedFilesCount = 0;
        for (int i = 0; i < paths.size(); i++) {
            QFileInfo fi(paths.at(i));
            if (fi.isDir()) {
                selectedDirectoriesCount++;
            }
            if (fi.isFile()) {
                selectedFilesCount++;
            }
        }
        emit selectedItems(selectedDirectoriesCount, selectedFilesCount);
    }

    for (int i = 0; i < paths.size(); i++) {
        QFileInfo fi(paths.at(i));
        if (fi.isFile()) {
            totSize += fi.size();
            filCount++;
            emitCounter++;
            if (emitCounter == 300) {
                emit currentData(totSize, dirCount, filCount);
                emitCounter = 0;
            }
            continue;
        } else if (fi.isDir()) {

            QDirIterator it(paths.at(i), QDirIterator::Subdirectories);
            while (it.hasNext()) {
                fi.setFile(it.next());
                QString fileName = fi.fileName();
                if (fi.isDir() ) {
                    if ((fileName == ".") || (fileName == "..") ) {
                        continue;
                    }
                    dirCount++;
                } else if (fi.isFile()) {
                    totSize += fi.size();
                    filCount++;
                }
                emitCounter++;
                if (emitCounter == 300) {
                    emit currentData(totSize, dirCount, filCount);
                    emitCounter = 0;
                }
            }
        }
    }
    emit currentData(totSize, dirCount, filCount);
}
