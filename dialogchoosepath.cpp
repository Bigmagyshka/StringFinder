#include "dialogchoosepath.h"
#include "ui_dialogchoosepath.h"

DialogChoosePath::DialogChoosePath(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogChoosePath){
	ui->setupUi(this);
}

DialogChoosePath::DialogChoosePath(QString sPath, QString sInclude, QString sExclude, bool bIsUseSTDStream, QWidget *parent) :
	QDialog(parent), ui(new Ui::DialogChoosePath)
{
	ui->setupUi(this);
	ui->textBrowser_Path->setText(sPath);
	ui->textBrowser_Include->setText(sInclude);
	ui->textBrowser_Exclude->setText(sExclude);
	ui->checkBoxUseSTD->setChecked(bIsUseSTDStream);
}

DialogChoosePath::~DialogChoosePath(){
	delete ui;
}

QString DialogChoosePath::GetPath(){
	return ui->textBrowser_Path->text();
}

QString DialogChoosePath::GetInclude(){
	return ui->textBrowser_Include->text();
}

QString DialogChoosePath::GetExclude(){
	return ui->textBrowser_Exclude->text();
}

bool DialogChoosePath::GetIsUseSTDStream(){
	return ui->checkBoxUseSTD->isChecked();
}

void DialogChoosePath::on_button_OpenPathDlg_clicked(){
	auto sPathToOpenDlg = ui->textBrowser_Path->text();
	sPathToOpenDlg.chop(sPathToOpenDlg.size() - sPathToOpenDlg.lastIndexOf("/"));
	auto sPathName = QFileDialog::getExistingDirectory(this, tr("Path"), sPathToOpenDlg);

	if (!sPathName.isEmpty())
		ui->textBrowser_Path->setText(sPathName);
}

void DialogChoosePath::WriteToFile(){
	FileHelper::WriteVecToFile("Settings.ini"
								, {{"path", GetPath()}
								   , {"include", GetInclude()}
								   , {"exclude", GetExclude()}
								   , {"use_std_stream", {GetIsUseSTDStream() ? 'y' : 'n'}}
								});
}

void DialogChoosePath::on_buttonBox_accepted(){
	WriteToFile();
}
