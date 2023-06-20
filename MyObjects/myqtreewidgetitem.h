#pragma once

#include <QObject>
#include <QTreeWidgetItem>

class MyQTreeWidgetItem : public QTreeWidgetItem {

public:
	MyQTreeWidgetItem();
	MyQTreeWidgetItem(const QStringList &strings, int type = 1000);
	virtual ~MyQTreeWidgetItem() = default;
};
