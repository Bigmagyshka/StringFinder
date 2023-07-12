#include "myparserrunnable.h"
#include <qfile.h>

MyParserRunnable::MyParserRunnable(QString sPath, QString sText, int nVersion, bool bUseOldStyleStream, bool bIsCaseSensetive, bool bSearchFullPhrase, QSharedPointer<bool> &bForceStop)
	: QObject(), QRunnable(), m_sPath(sPath), m_sText(sText), m_nVersion(nVersion)
	, m_bUseOldStyleStream(bUseOldStyleStream)
	, m_bIsCaseSensetive(bIsCaseSensetive)
	, m_bSearchFullPhrase(bSearchFullPhrase)
	, m_bForceStop(bForceStop)
{
}

bool MyParserRunnable::CheckIsTextSuitable(const QString &sText){
	if(sText.isEmpty())
		return false;
	if(!sText.contains(m_sText, m_bIsCaseSensetive ? Qt::CaseSensitivity::CaseSensitive : Qt::CaseSensitivity::CaseSensitive))
		return false;

	return true;
}

void MyParserRunnable::UseQByteArray(){
	QFile file(m_sPath);
	auto pResult = new QTreeWidgetItem(QStringList {m_sPath, "", ""});

	if(!file.open(QIODevice::ReadOnly))
	{
		emit SignalSendResult(pResult, m_nVersion);
		return;
	}

	int nLine{0};

	QByteArray data = file.readAll();
	auto text {QString::fromLocal8Bit(data)};
	data.clear();
	auto listSplited = text.split('\n');
	text.clear();

	for(const auto &sLine : listSplited){
		if(*m_bForceStop)
			break;

		++nLine;
		auto sLineText = in.readLine().trimmed();

		if(!CheckIsTextSuitable(sLineText))
			continue;

		pResult->addChild(new QTreeWidgetItem(QStringList{"", QString::number(nLine), sLineText}));
	}
	file.close();

	emit SignalSendResult(pResult, m_nVersion);
}

void MyParserRunnable::UseQStream()
{
	QFile file(m_sPath);
	auto pResult = new QTreeWidgetItem(QStringList {m_sPath, "", ""});

	if(!file.open(QIODevice::ReadOnly))
	{
		emit SignalSendResult(pResult, m_nVersion);
		return;
	}

	int nLine{0};
	QTextStream in(&file);

	while(!in.atEnd()){
		if(*m_bForceStop)
			break;

		++nLine;
		auto sLineText = in.readLine().trimmed();

		if(!CheckIsTextSuitable(sLineText))
			continue;

		pResult->addChild(new QTreeWidgetItem(QStringList{"", QString::number(nLine), sLineText}));
	}
	file.close();

	emit SignalSendResult(pResult, m_nVersion);
}

void MyParserRunnable::run()
{
	if(m_bUseOldStyleStream){
		UseQByteArray();
	}
	else{
		UseQStream();
	}
}
