#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QDir>
#include "fileutils.h"

class FileSystem : public QObject
{
    Q_OBJECT
public:
    FileSystem(QObject* parent = NULL);
    ~FileSystem();

    bool cd(const QString& path);

    inline bool currentPathIsTopLevelDeviceDir() const {
        return FileUtils::isTopLevelDeviceDir(currentDir.absolutePath());
    }

    inline bool currentPathIsTopLevelDir() const{
        return FileUtils::isTopLevelDir(currentDir.absolutePath());
    }

    inline bool currentPathIsMountedLocalFileSystem() const{
        return pathIsMountedLocalFileSystem;
    }

    QDir currentDir;

protected:
    bool pathIsMountedLocalFileSystem;

};

#endif // FILESYSTEM_H
