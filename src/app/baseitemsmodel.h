#ifndef BASEITEMSMODEL_H
#define BASEITEMSMODEL_H

#include <QString>
#include <QVector>

#include <bb/cascades/GroupDataModel>
#include <QtCore/QObject>

class BaseItemsModel : public  QObject
{
    Q_OBJECT
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);
public:
    BaseItemsModel(QObject *parent = 0);

    Q_INVOKABLE QString data(const QVariantList &indexPath, const QString& value) const;
    Q_INVOKABLE bool isEmpty();
    Q_INVOKABLE int itemsCount() const;
    Q_INVOKABLE virtual void addItem(const QString& path);
    Q_INVOKABLE virtual void clear();
    Q_INVOKABLE virtual void deleteItem(const QString& path);
    Q_INVOKABLE virtual void updateModel() {};

protected:
    inline void updateOrInsertEntry(int indexPath, QVariantMap& entry, bool update) {
        if (!update) {
            groupDataModel->insert(entry);
        } else {
            QVariantList list;
            list.append(indexPath);
            groupDataModel->updateItem(list, entry);
        }
    }

    QString sortField;
    bb::cascades::GroupDataModel* groupDataModel;
    bb::cascades::GroupDataModel* model() const;
    bool sortedAscending;
    quint64 itCount;
};

#endif // BASEITEMSMODEL_H
