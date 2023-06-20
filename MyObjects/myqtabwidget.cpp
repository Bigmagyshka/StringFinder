#include <QRegularExpression>
#include "MyObjects/myqtabwidget.h"
#include "MyObjects/myqtreewidget.h"

MyQTabWidget::MyQTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
}

QVector<QString> MyQTabWidget::GetPrevValues(int nLvl)
{
	QVector<QString> vecPrevValues;

	auto pCurrElem = this;
	auto pParent = dynamic_cast<MyQTabWidget *>(pCurrElem->parent()->parent()); //that cause elem of TabWidget is it's grandchild

	for (;nLvl > 1; nLvl--) {
		if(!pParent)
			return {};
		vecPrevValues.emplaceFront(pParent->tabText(pParent->indexOf(pCurrElem)));

		pCurrElem = pParent;
		pParent = dynamic_cast<MyQTabWidget *>(pParent->parent()->parent());
	}
	return vecPrevValues;
}

bool MyQTabWidget::SlotSearchChanged(QVector<QString> &vecFilter, int nMaxLvl, int nRecursionLvl/* = 0*/)
{
	int nDisabledTabs{0};

	for(int i{0}; i < count(); i++){
		auto tab = dynamic_cast<MyQTabWidget *>(widget(i));
		if(!tab)
		{
			auto treeWidget = dynamic_cast<MyQTreeWidget *>(widget(i));
			if(!treeWidget)
				continue;

			auto listSplitFolder = treeWidget->GetFileName().split("/", Qt::SkipEmptyParts);
			bool bIsVisible = listSplitFolder.size() - 1 >= nMaxLvl;

			setTabVisible(i, bIsVisible);
			if(!bIsVisible)
				nDisabledTabs++;
			continue;
		}

		auto vecPrevValues = tab->GetPrevValues(nRecursionLvl + 2);
		bool bIsVisible = vecFilter[vecPrevValues.size() - 1].isEmpty() || vecPrevValues.last() == vecFilter[vecPrevValues.size() - 1];

		if(bIsVisible)
		{
			if(!tab->SlotSearchChanged(vecFilter, nMaxLvl, nRecursionLvl + 1))
			{
				setTabVisible(i, false);
				nDisabledTabs++;
			}
			else
				setTabVisible(i, true);
		}
		else
		{
			setTabVisible(i, false);
			nDisabledTabs++;
		}
	}

	return nDisabledTabs != count();
}
