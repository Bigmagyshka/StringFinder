#pragma once

#include <QRunnable>
#include <QTreeWidgetItem>

class MyParserRunnable
	: public QObject
	, public QRunnable {
	Q_OBJECT
	QString m_sPath;
	QVector<QString> m_vecIncludeText;
	QVector<QRegularExpression> m_vecRegExpInclude;
	bool m_bIsCaseSensetiveInclude {false};
	bool m_bSearchFullPhraseInclude {false};

	QVector<QString> m_vecExcludeText;
	QVector<QRegularExpression> m_vecRegExpExclude;
	bool m_bIsCaseSensetiveExclude {false};
	bool m_bSearchFullPhraseExclude {false};

	int m_nVersion {-1};
	bool m_bUseOldStyleStream {true};

	QSharedPointer<bool> m_bForceStop;

	void UseQByteArray();
	void UseQStream();
	bool CheckIsTextSuitable(const QString &sText);

public:
	MyParserRunnable(const QString &sPath, const QString &sIncludeText, const QString &sExcludeText, int nVersion, bool bUseOldStyleStream,
					 bool bIsCaseSensetiveInclude, bool bSearchFullPhraseInclude, bool bIsCaseSensetiveExclude, bool bSearchFullPhraseExclude,
					 QSharedPointer<bool> &bForceStop);
	virtual ~MyParserRunnable() = default;
	void run();

signals:
	void SignalSendResult(QTreeWidgetItem *, int);
};
