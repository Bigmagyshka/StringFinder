#include <QFile>
#include <QDirIterator>

#include "filehelper.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogchoosepath.h"
#include "MyObjects/myqcombobox.h"
#include "MyObjects/myqtreewidget.h"
#include "MyObjects/myparserrunnable.h"

#include <QThread>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	restoreGeometry(m_objSettings.value("MainWindow/geometry").toByteArray());
	restoreState(m_objSettings.value("MainWindow/windowState").toByteArray());

	m_sPath = m_objSettings.value("MainWindow/Path").toString();
	if(m_sPath.isEmpty())
	{
		auto mapSettings = FileHelper::ReadMapFromFile("Settings.ini");

		if(mapSettings.contains("path"))
			m_sPath = mapSettings["path"];

		if(mapSettings.contains("include"))
			m_sInclude = mapSettings["include"];

		if(mapSettings.contains("exclude"))
			m_sExclude = mapSettings["exclude"];

		if(mapSettings.contains("use_std_stream"))
			m_bUseByteArrayToRead = mapSettings["use_std_stream"] == 'y';
	}
	else
	{
		m_sInclude = m_objSettings.value("MainWindow/Includes").toString();
		m_sExclude = m_objSettings.value("MainWindow/Excludes").toString();
		m_bUseByteArrayToRead = m_objSettings.value("MainWindow/UseByteArrayToRead").toBool();
		m_nThreadCount = m_objSettings.value("MainWindow/ThreadCount").toInt();

		m_objThreadPool.setMaxThreadCount(m_nThreadCount);
	}

	ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	m_bForceStop = QSharedPointer<bool>(new bool {false});

	ReCreateDlgElements();
}

MainWindow::~MainWindow(){
	m_objSettings.setValue("MainWindow/geometry", saveGeometry());
	m_objSettings.setValue("MainWindow/windowState", saveState());

	m_objSettings.setValue("MainWindow/Path", m_sPath);
	m_objSettings.setValue("MainWindow/Includes", m_sInclude);
	m_objSettings.setValue("MainWindow/Excludes", m_sExclude);
	m_objSettings.setValue("MainWindow/UseByteArrayToRead", m_bUseByteArrayToRead);
	m_objSettings.setValue("MainWindow/ThreadCount", m_nThreadCount);

	delete ui;
}

void MainWindow::on_OpenDlgChoosePath_triggered(){

	DialogChoosePath dlgChoosePath(m_sPath, m_sInclude, m_sExclude, m_bUseByteArrayToRead, m_nThreadCount);

	if (dlgChoosePath.exec() == 1){
		m_sPath = dlgChoosePath.GetPath();
		m_sInclude = dlgChoosePath.GetInclude();
		m_sExclude = dlgChoosePath.GetExclude();
		m_bUseByteArrayToRead = dlgChoosePath.GetIsUseSTDStream();
		m_nThreadCount = dlgChoosePath.GetThreadCount();

		m_objSettings.setValue("MainWindow/Path", m_sPath);
		m_objSettings.setValue("MainWindow/Includes", m_sInclude);
		m_objSettings.setValue("MainWindow/Excludes", m_sExclude);
		m_objSettings.setValue("MainWindow/UseByteArrayToRead", m_bUseByteArrayToRead);
		m_objSettings.setValue("MainWindow/ThreadCount", m_nThreadCount);

		m_objThreadPool.setMaxThreadCount(m_nThreadCount);

		ReCreateDlgElements();
	}
}

void MainWindow::on_Reload_triggered(){
	ReCreateDlgElements();
}

void MainWindow::SlotThreadEndWork(QTreeWidgetItem *pItem, int nVersion){
	ui->progressBar->setValue(++m_nProgress);

	if(nVersion != m_nVersion || pItem->childCount() == 0){
		delete pItem;
	}
	else{
		m_listLocalItems.emplaceBack(pItem);
	}

	if(m_nProgress == m_vecCurrentPaths.size()){
		ui->treeWidget->insertTopLevelItems(0, m_listLocalItems);
		m_listLocalItems.clear();
		ui->treeWidget->ReloadAllItems(GetAllCmbText());
		ui->treeWidget->setEnabled(true);
		ReCreateSearchZone();
	}
}

QVector<QString> MainWindow::GetAllCmbText(){
	QVector<QString> vecCurValues;

	for(auto &elem : ui->scrollArea_Subdirectories->widget()->children()){
		auto cmb = dynamic_cast<MyQComboBox *>(elem);
		if(!cmb)
			continue;
		vecCurValues.emplaceBack(cmb->currentText());
	}

	return vecCurValues;
}

void MainWindow::ReCreateDlgElements(){
	auto [vecCurrentPaths, dSize] = GetPath2AllFiles();
	m_vecCurrentPaths = vecCurrentPaths;

	ui->lcd_Count->display(static_cast<double>(m_vecCurrentPaths.size()));
	ui->lcd_Volume->display(dSize);

	m_objThreadPool.clear();
	++m_nVersion;
	ui->treeWidget->clear();
	ui->treeWidget->SetPath(m_sPath);
	ui->progressBar->setRange(0, m_vecCurrentPaths.size());
	ui->progressBar->setValue(0);
}

void MainWindow::ReCreateSearchZone(){
	for(auto elem : ui->scrollAreaWidgetContents->children()){
		if(auto layout = dynamic_cast<QLayout *>(elem))
			continue;
		//We need to delete all items instead of layout
		elem->deleteLater();
	}

	QVector<QString> vecCurItemsPath;
	vecCurItemsPath.reserve(ui->treeWidget->topLevelItemCount());
	for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i){
		auto item = ui->treeWidget->topLevelItem(i);
		auto sFullPath = item->text(0);
		auto sLocalPath = sFullPath.replace(m_sPath + "/", "");
		vecCurItemsPath.emplaceBack(sLocalPath);
	}

	auto vecAllLevels = GetAllLevels(vecCurItemsPath);
	auto layout = ui->scrollAreaWidgetContents->layout();
	QVector<MyQComboBox *> vecNewCmb;
	int nLvl{1};

	for (const auto &set : vecAllLevels){

		auto label = new QLabel{"Lvl" + QString::number(nLvl)};
		label->setMaximumSize(350, 25);
		layout->addWidget(label);

		auto cmb = new MyQComboBox(nLvl++, &m_vecCurrentPaths);
		cmb->addItems(QStringList(set.begin(), set.end()));
		cmb->addItem("");
		cmb->model()->sort(0);
		cmb->setCurrentText("");
		cmb->setMaximumSize(350, 25);
		layout->addWidget(cmb);

		vecNewCmb.emplaceBack(cmb);
	}


	for(int nFirst{0}; nFirst < vecNewCmb.size() - 1; nFirst++){
		auto cmbFirst = vecNewCmb[nFirst];

		for(int nSecond{nFirst + 1}; nSecond < vecNewCmb.size(); nSecond++){
			connect(cmbFirst, &MyQComboBox::SignalValueChanged
				, vecNewCmb[nSecond], &MyQComboBox::SlotValueChanged);
		}

		connect(cmbFirst, &MyQComboBox::SignalValueChanged
			, ui->treeWidget, &MyQTreeWidget::SlotSearchChanged);
	}

	if(!vecNewCmb.isEmpty())
		connect(vecNewCmb.last(), &MyQComboBox::SignalValueChanged
			, ui->treeWidget, &MyQTreeWidget::SlotSearchChanged);
}

QPair<QVector<QString>, double> MainWindow::GetPath2AllFiles(){
	QVector<QString> vecLocalPaths;
	double dSize {0.0};
	QDirIterator it(m_sPath, QDir::Files, QDirIterator::Subdirectories);

	auto sExcludeLines = m_sExclude.split(",", Qt::SkipEmptyParts);
	for(auto &sLine : sExcludeLines){
		sLine = sLine.trimmed();
	}
	auto sIncludeLines = m_sInclude.split(",", Qt::SkipEmptyParts);
	for(auto &sLine : sIncludeLines){
		sLine = sLine.trimmed();
	}

	while (it.hasNext()){
		QFile f(it.next());

		bool bSkip {false};
		for(const auto &sLine : sExcludeLines){
			if(it.filePath().contains(sLine, Qt::CaseSensitivity::CaseSensitive)){
				bSkip = true;
				break;
			}
		}
		if(bSkip)
			continue;

		bSkip = !sIncludeLines.empty();
		for(const auto &sLine : sIncludeLines){
			if(it.filePath().contains(sLine, Qt::CaseSensitivity::CaseSensitive)){
				bSkip = false;
				break;
			}
		}
		if(bSkip)
			continue;

		dSize += static_cast<double>(it.fileInfo().size())/1024/1024/1024;
		vecLocalPaths.emplaceBack(it.filePath().remove(0, m_sPath.size() + 1));
	}
	return {vecLocalPaths, dSize};
}

QVector<QSet<QString>> MainWindow::GetAllLevels(QVector<QString> &vec){

	QVector<QSet<QString>> vecAllLvls;

	for(const auto &sElem : vec){
		auto sSepLines = sElem.split("/");
		if(sSepLines.size() == 1)
			continue;

		for(int nVecs = vecAllLvls.size(); nVecs < sSepLines.size() - 1; nVecs++)
			vecAllLvls.push_back(QSet<QString>());

		for(int nLvl = 0; nLvl < sSepLines.size() - 1; nLvl++)
			vecAllLvls[nLvl].insert(sSepLines[nLvl]);
	}

	return vecAllLvls;
}

void MainWindow::on_button_Clear_released(){
   for(auto &elem : ui->scrollArea_Subdirectories->widget()->children()){
	   auto cmb = dynamic_cast<MyQComboBox *>(elem);
	   if(!cmb)
		   continue;
		cmb->setCurrentIndex(0);
   }
}

void MainWindow::on_button_Search_released(){
	if(ui->textBrowser_Dlg_Name->toPlainText().isEmpty())
		return;

	++m_nVersion;
	m_objThreadPool.clear();
	ui->treeWidget->clear();
	ui->treeWidget->setEnabled(false);
	for(auto p : m_listLocalItems)
		delete p;
	m_listLocalItems.clear();

	m_nProgress = 0;
	ui->progressBar->setValue(m_nProgress);
	auto sText = ui->textBrowser_Dlg_Name->toPlainText().trimmed();
	auto bIsCaseSensetive = ui->checkBoxCaseSensetive->isChecked();
	auto bIsSearchFullPhrase = ui->checkBoxSearchFullPhrase->isChecked();

	for(const auto &sPath : m_vecCurrentPaths){
		auto pFuncForCalc = new MyParserRunnable(m_sPath + "/" + sPath, sText, m_nVersion, m_bUseByteArrayToRead, bIsCaseSensetive, bIsSearchFullPhrase, m_bForceStop);
		connect(pFuncForCalc, &MyParserRunnable::SignalSendResult
				, this, &MainWindow::SlotThreadEndWork);

		m_objThreadPool.start(pFuncForCalc);
	}
}


void MainWindow::on_button_Stop_released(){
	//Накручиваем версию, выставляем флаг на сброс и чистим пул потоков
	++m_nVersion;
	*m_bForceStop = true;
	m_bForceStop.reset(new bool {false});
	m_objThreadPool.clear();

	//Выводим всё, что успели найти
	ui->treeWidget->insertTopLevelItems(0, m_listLocalItems);
	m_listLocalItems.clear();
	ui->treeWidget->ReloadAllItems(GetAllCmbText());
	ui->treeWidget->setEnabled(true);
}

