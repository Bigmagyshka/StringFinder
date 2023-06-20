#pragma once

#include <QFileDialog>

namespace FileHelper
{
	bool WriteVecToFile(QString sFileName, QVector<QPair<QString, QString>> vecKeyAndString, bool bReWrite = true);
	void WriteStringToOStream(QTextStream &osStream, QString sString);

	QMap<QString, QString> ReadMapFromFile(QString sFileName);
};
