#include "filehelper.h"

namespace FileHelper
{
	void WriteStringToOStream(QTextStream &osStream, QPair<QString, QString> sKeyAndString){
		osStream << sKeyAndString.first << "=" << sKeyAndString.second << Qt::endl;
	}

	bool WriteVecToFile(QString sFileName, QVector<QPair<QString, QString>> vecKeyAndString, bool bReWrite /*= true*/){
		QFile file(sFileName);
		if (!file.open(QIODevice::WriteOnly|(bReWrite ? QIODevice::NotOpen : QIODevice::Append)))
			return false;

		if(bReWrite)
			file.resize(0);

		QTextStream out(&file);
		out.setEncoding(QStringConverter::Utf8);

		for(auto elem : vecKeyAndString)
			WriteStringToOStream(out, elem);

		return true;
	}

	QMap<QString, QString> ReadMapFromFile(QString sFileName){
		QFile file(sFileName);
		if (!file.open(QIODevice::ReadOnly))
			return {};

		QTextStream in(&file);
		in.setEncoding(QStringConverter::Utf8);

		auto sTemp = in.readAll();
		if(sTemp.isEmpty())
			return {};

		auto sReadLines = sTemp.split("\n", Qt::SkipEmptyParts);
		sTemp.clear();

		QMap<QString, QString> mapValues;
		for(auto sLine : sReadLines){

			auto sSepLine = sLine.split("=");
			if(sSepLine.size() != 2)
				continue;

			mapValues[sSepLine[0].trimmed()] = sSepLine[1].trimmed();
		}

		return mapValues;
	}
}
