#pragma once
#include <QTreeWidget>
#include <QObject>

class MyQTreeWidget : public QTreeWidget
{
	const QStringList m_listHeader {"FileName", "PozInFile", "FullString"};
	QString m_sPath;

	void HideShowAllItems(const QVector<QString> &vecFilter);

public:
	MyQTreeWidget(QWidget *parent = nullptr);
	virtual ~MyQTreeWidget() = default;
	void SetPath(QString sPath);
	bool CheckIsObjectFit(const QVector<QString> &vecFilter, QTreeWidgetItem *pItem);
	void ReloadAllItems(const QVector<QString> &vecFilter);

public slots:
	bool SlotSearchChanged(QVector<QString> &vecFilter);
};
