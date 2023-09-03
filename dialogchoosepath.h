#pragma once

#include <QDialog>

namespace Ui {
class DialogChoosePath;
}

class DialogChoosePath : public QDialog
{
	Q_OBJECT

public:
	explicit DialogChoosePath(QWidget *parent = nullptr);
	DialogChoosePath(QString sPath, QString sInclude, QString sExclude, bool bIsUseSTDStream, int nThreads, QWidget *parent = nullptr);
	~DialogChoosePath();

	QString GetPath();
	QString GetInclude();
	QString GetExclude();
	bool GetIsUseSTDStream();
	int GetThreadCount();

private slots:
	void on_button_OpenPathDlg_clicked();

private:
	Ui::DialogChoosePath *ui;
};
