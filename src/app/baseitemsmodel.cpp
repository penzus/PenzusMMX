#include <QDebug>

#include "baseitemsmodel.h"
#include "applicationui.h"


using namespace bb::cascades;

bb::cascades::GroupDataModel* BaseItemsModel::model() const
{
    return groupDataModel;
}

int BaseItemsModel::itemsCount() const
{
    return itCount;
}

QString BaseItemsModel::data(const QVariantList &indexPath, const QString& value) const
{
    if (!indexPath.isEmpty()) {
        const QVariantMap entry = groupDataModel->data(indexPath).toMap();
        return entry.value(value).toString();
    }
    return QString();
}

bool BaseItemsModel::isEmpty()
{
    return groupDataModel->isEmpty();
}

void BaseItemsModel::clear()
{
    groupDataModel->clear();
}

BaseItemsModel::BaseItemsModel(QObject *parent) : QObject(parent)
{
    sortedAscending = true;
    itCount = 0;
    groupDataModel = new GroupDataModel(this);

    groupDataModel->setGrouping(ItemGrouping::None);
    QStringList sortingKeys;
    sortingKeys << "name";
    groupDataModel->setSortedAscending(sortedAscending);
}

void BaseItemsModel::deleteItem(const QString& /*path*/)
{

}

void BaseItemsModel::addItem(const QString& /*path*/)
{

}
