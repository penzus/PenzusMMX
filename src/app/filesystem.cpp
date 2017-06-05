#include "filesystem.h"

FileSystem::FileSystem(QObject*)
{
    pathIsMountedLocalFileSystem = false;
}

FileSystem::~FileSystem()
{

}

bool FileSystem::cd(const QString& path) {
    bool result = currentDir.cd(path);
    if (result) {
        pathIsMountedLocalFileSystem = FileUtils::pathIsMountedLocalFileSystem(currentDir.absolutePath());
    }
    return result;
}
