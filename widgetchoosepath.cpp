#include "widgetchoosepath.h"

WidgetChoosePath::WidgetChoosePath(QObject *parent)
	: QAbstractItemModel(parent)
{
}

QVariant WidgetChoosePath::headerData(int section, Qt::Orientation orientation, int role) const
{
	// FIXME: Implement me!
}

QModelIndex WidgetChoosePath::index(int row, int column, const QModelIndex &parent) const
{
	// FIXME: Implement me!
}

QModelIndex WidgetChoosePath::parent(const QModelIndex &index) const
{
	// FIXME: Implement me!
}

int WidgetChoosePath::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

int WidgetChoosePath::columnCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 0;

	// FIXME: Implement me!
}

QVariant WidgetChoosePath::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// FIXME: Implement me!
	return QVariant();
}
