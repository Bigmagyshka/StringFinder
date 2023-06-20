#ifndef MYQTABWIDGET_H
#define MYQTABWIDGET_H

#include <QTabWidget>
#include <QObject>

class MyQTabWidget : public QTabWidget
{
public:
	MyQTabWidget(QWidget *parent = nullptr);
	virtual ~MyQTabWidget() = default;

	QVector<QString> GetPrevValues(int nLvl);

public slots:
	bool SlotSearchChanged(QVector<QString> &vecFilter, int nMaxLvl, int nRecursionLvl = 0);
};

#endif // MYQTABWIDGET_H
