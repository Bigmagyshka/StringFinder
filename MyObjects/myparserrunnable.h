#pragma once

#include <QRunnable>
#include <QTreeWidgetItem>

class MyParserRunnable : public QObject, public QRunnable{
	Q_OBJECT
	QString m_sPath;
	QString m_sText;
	int m_nVersion {-1};
	bool m_bUseOldStyleStream {true};
	bool m_bIsCaseSensetive {false};
	bool m_bSearchFullPhrase {false};
	QSharedPointer<bool> m_bForceStop;

	void UseQByteArray();
	void UseQStream();
	bool CheckIsTextSuitable(const QString &sText);

public:
	MyParserRunnable(QString sPath, QString sText, int nVersion, bool bUseOldStyleStream, bool bIsCaseSensetive, bool bSearchFullPhrase, QSharedPointer<bool> &bForceStop);
	virtual ~MyParserRunnable() = default;
	void run();

signals:
	void SignalSendResult(QTreeWidgetItem *, int);
};
