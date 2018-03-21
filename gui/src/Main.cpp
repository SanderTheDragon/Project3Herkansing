#include <QApplication>
#include <QtCrypto/QtCrypto>

#include "src/MainWindow.hpp"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QCA::Initializer init;
	MainWindow w;
	w.show();
	w.start();

	return a.exec();
}
