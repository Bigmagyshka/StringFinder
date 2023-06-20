#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]){
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("BigMag inc");
	QCoreApplication::setApplicationName("Serious Pet 3");

	MainWindow w;
	w.show();
	return a.exec();
}
