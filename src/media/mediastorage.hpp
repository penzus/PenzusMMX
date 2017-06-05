
#ifndef MEDIASTORAGE_HPP
#define MEDIASTORAGE_HPP

#include <QtCore/QObject>

#include "mediafile.hpp"

namespace bbext {
    namespace multimedia
    {
        class MediaLibrary;
    }   }


class MediaStorage : public QObject
{
    Q_OBJECT
public:

    MediaStorage(QObject* parent = NULL);
    virtual ~MediaStorage() { }


    private slots:
    void onMediaAdded(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaDeleted(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaUpdated(const bbext::multimedia::MediaFile &mediaFile);
    void onMediaInvalidated(const bbext::multimedia::MediaFile &mediaFile);
    void onTestQuery();
    private:
    bbext::multimedia::MediaLibrary *mediaLibrary_;
};

#endif // MEDIASTORAGE_HPP
