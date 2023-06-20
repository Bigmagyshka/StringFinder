#pragma once

#include <QComboBox>

class MyQComboBox : public QComboBox
{
	Q_OBJECT

	QVector<QString> *m_pvecCurPathsForRC {nullptr};
	int m_nLvl {-1};

	QVector<QString> GetVectorFilter();
	QVector<QString> GetAllCmbText();

public:
	MyQComboBox();
	MyQComboBox(int nLvl, QVector<QString> *pvecCurParhsForRC);
	virtual ~MyQComboBox() = default;

	void SetLvl(int nLvl);
	int GetLvl() const;


private slots:
	void DoEmitCurrentTextChanged();

public slots:
	void SlotValueChanged();

signals:
	void SignalValueChanged(QVector<QString> &);
};
