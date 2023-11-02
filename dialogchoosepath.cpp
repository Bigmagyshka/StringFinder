#include "dialogchoosepath.h"

#include "qthread.h"
#include "ui_dialogchoosepath.h"

#include <QFileDialog>

DialogChoosePath::DialogChoosePath(QWidget *parent) : QDialog(parent), ui(new Ui::DialogChoosePath) {
	ui->setupUi(this);
}

DialogChoosePath::DialogChoosePath(QString sPath, QString sInclude, QString sExclude, bool bIsUseSTDStream, int nThreads, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DialogChoosePath) {
	ui->setupUi(this);
	ui->textBrowser_Path->setText(sPath);
	ui->textBrowser_Include->setText(sInclude);
	ui->textBrowser_Exclude->setText(sExclude);
	ui->checkBoxUseSTD->setChecked(bIsUseSTDStream);
	ui->spinBox_ThreadCount->setValue(nThreads);
	ui->spinBox_ThreadCount->setMaximum(QThread::idealThreadCount());

	auto sVersion = "version: " + QString(APP_VERSION);
	ui->version->setText(sVersion);
}

DialogChoosePath::~DialogChoosePath() {
	delete ui;
}

QString DialogChoosePath::GetPath() {
	return ui->textBrowser_Path->text();
}

QString DialogChoosePath::GetInclude() {
	return ui->textBrowser_Include->text();
}

QString DialogChoosePath::GetExclude() {
	return ui->textBrowser_Exclude->text();
}

bool DialogChoosePath::GetIsUseSTDStream() {
	return ui->checkBoxUseSTD->isChecked();
}

int DialogChoosePath::GetThreadCount() {
	return ui->spinBox_ThreadCount->value();
}

void DialogChoosePath::on_button_OpenPathDlg_clicked() {
	auto sPathToOpenDlg = ui->textBrowser_Path->text();
	sPathToOpenDlg.chop(sPathToOpenDlg.size() - sPathToOpenDlg.lastIndexOf("/"));
	auto sPathName = QFileDialog::getExistingDirectory(this, tr("Path"), sPathToOpenDlg);

	if(!sPathName.isEmpty())
		ui->textBrowser_Path->setText(sPathName);
}
