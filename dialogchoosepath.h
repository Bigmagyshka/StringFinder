#pragma once

#include <QDialog>
#include "filehelper.h"

namespace Ui {
class DialogChoosePath;
}

class DialogChoosePath : public QDialog
{
	Q_OBJECT

public:
	explicit DialogChoosePath(QWidget *parent = nullptr);
	DialogChoosePath(QString sPath, QString sInclude, QString sExclude, bool bIsUseSTDStream, QWidget *parent = nullptr);
	~DialogChoosePath();

	QString GetPath();
	QString GetInclude();
	QString GetExclude();
	bool GetIsUseSTDStream();

private slots:
	void on_button_OpenPathDlg_clicked();
	void on_buttonBox_accepted();

private:
	Ui::DialogChoosePath *ui;
	void WriteToFile();
};
