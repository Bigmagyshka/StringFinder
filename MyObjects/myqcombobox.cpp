#include "MyObjects/myqcombobox.h"

MyQComboBox::MyQComboBox() : QComboBox() {}

MyQComboBox::MyQComboBox(int nLvl, QVector<QString> *pvecCurParhsForRC)
	: QComboBox()
	, m_pvecCurPathsForRC(pvecCurParhsForRC)
	, m_nLvl(nLvl)
{
	connect(this, &MyQComboBox::currentTextChanged
			, this, &MyQComboBox::DoEmitCurrentTextChanged);

	setFixedSize(310, 25);
	setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
}

void MyQComboBox::SetLvl(int nLvl){
	m_nLvl = nLvl;
}

int MyQComboBox::GetLvl() const {
	return m_nLvl;
}

void MyQComboBox::DoEmitCurrentTextChanged(){
	auto vecFilter = GetAllCmbText();
	emit SignalValueChanged(vecFilter);
}

QVector<QString> MyQComboBox::GetVectorFilter(){
	QVector<QString> vecPrevCmbTexts;
	vecPrevCmbTexts.reserve(m_nLvl - 1);
	for(int n{0}; n < m_nLvl - 1; n++)
		vecPrevCmbTexts.emplaceBack("");

	for(auto &elem : parent()->children()){
		auto cmb = dynamic_cast<MyQComboBox *>(elem);
		if(!cmb || cmb->GetLvl() >= m_nLvl)
			continue;

		vecPrevCmbTexts[cmb->GetLvl() - 1] = cmb->currentText();
	}

	return vecPrevCmbTexts;
}

QVector<QString> MyQComboBox::GetAllCmbText(){
	if(!parent())
		return {};
	QVector<QString> vecPrevCmbTexts;

	for(auto &elem : parent()->children()){
		auto cmb = dynamic_cast<MyQComboBox *>(elem);
		if(!cmb)
			continue;

		vecPrevCmbTexts.emplaceBack(cmb->currentText());
	}

	return vecPrevCmbTexts;
}

void MyQComboBox::SlotValueChanged(){
	if(!m_pvecCurPathsForRC)
		return;

	auto sOldText = currentText();

	auto vecFilter = GetVectorFilter();
	bool bAllEmpty {true};
	for(const auto &sStr : vecFilter){
		if(sStr.isEmpty())
			continue;

		bAllEmpty = false;
		break;
	}

	for(int i{0}; i < count(); ++i){
		if(!itemText(i).isEmpty()){
			removeItem(i--);
		}
	}

	QSet<QString> setNewElems;
	for(auto &sElem : *m_pvecCurPathsForRC){
		auto sSepLines = sElem.split("/");
		if(sSepLines.size() - 1 < m_nLvl)
			continue;

		if(!bAllEmpty){
			bool bInFilter {true};
			for(int nElem{0}; nElem < vecFilter.size(); nElem++){
				if(vecFilter[nElem].isEmpty() || sSepLines[nElem] == vecFilter[nElem])
					continue;

				bInFilter = false;
				break;
			}

			if(!bInFilter)
				continue;
		}

		setNewElems.insert(sSepLines[m_nLvl - 1]);
	}

	if(setNewElems.isEmpty()){
		setDisabled(true);
		return;
	}

	setDisabled(false);
	addItems(QStringList(setNewElems.begin(), setNewElems.end()));
	model()->sort(0);
	setCurrentText(sOldText);
}
