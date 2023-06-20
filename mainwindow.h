#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QThreadPool>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow *ui;

	QSettings m_objSettings;
	QString m_sPath;
	QString m_sInclude;
	QString m_sExclude;
	bool m_bIsUseSTDStream {false};
	QVector<QString> m_vecCurrentPaths;
	QThreadPool m_objThreadPool;
	int m_nVersion{0};
	int m_nProgress{0};
	QSharedPointer<bool> m_bForceStop;

	QList<QTreeWidgetItem *> m_listLocalItems;

	void ReCreateDlgElements();
	void ReCreateSearchZone();

	QPair<QVector<QString>, double> GetPath2AllFiles();
	QVector<QSet<QString>> GetAllLevels(QVector<QString> &vec);
	QVector<QString> GetAllCmbText();

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_OpenDlgChoosePath_triggered();
	void on_button_Clear_released();
	void on_button_Search_released();
	void on_Reload_triggered();

	void on_button_Stop_released();

public slots:
	void SlotThreadEndWork(QTreeWidgetItem *pItem, int nVersion);
};
