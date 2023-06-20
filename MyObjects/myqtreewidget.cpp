#include "MyObjects/myqtreewidget.h"
#include "MyObjects/myqtreewidgetitem.h"

MyQTreeWidget::MyQTreeWidget(QWidget *parent /*= nullptr*/)
	: QTreeWidget(parent){
	setColumnCount(3);
	setHeaderLabels(m_listHeader);
}

void MyQTreeWidget::SetPath(QString sPath){
	m_sPath = sPath;
}

bool MyQTreeWidget::CheckIsObjectFit(const QVector<QString> &vecFilter, QTreeWidgetItem *pItem){
	int nMaxLvlSearch {-1};
	for(int nLvl{0}; nLvl < vecFilter.size(); ++nLvl){
		if(!vecFilter[nLvl].isEmpty())
		nMaxLvlSearch = nLvl;
	}
	if(nMaxLvlSearch == -1)
		return true;

	auto sFileName = pItem->text(0);
	sFileName.remove(0, m_sPath.size());
	auto listSplitFolder = sFileName.split("/", Qt::SkipEmptyParts);

	if(listSplitFolder.size() - 1 <= nMaxLvlSearch)
	return false;

	for(int nLvl{0}; nLvl <= nMaxLvlSearch; ++nLvl){
		if(vecFilter[nLvl].isEmpty())
			continue;

		if(vecFilter[nLvl] != listSplitFolder[nLvl])
			return false;
	}
	return true;
}

void MyQTreeWidget::HideShowAllItems(const QVector<QString> &vecFilter){
	int nIndex{0};
	auto nItemCount = invisibleRootItem()->childCount();
	for(; nIndex < nItemCount; ++nIndex){
		auto nItem = invisibleRootItem()->child(nIndex);
		nItem->setHidden(!CheckIsObjectFit(vecFilter, nItem));
	}
}

void MyQTreeWidget::ReloadAllItems(const QVector<QString> &vecFilter){
	HideShowAllItems(vecFilter);
}

bool MyQTreeWidget::SlotSearchChanged(QVector<QString> &vecFilter){
	HideShowAllItems(vecFilter);
	return true;
}
