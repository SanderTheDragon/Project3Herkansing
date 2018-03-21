#include "src/screens/Screen.hpp"

Screen::Screen(QWidget* parent) :
	QWidget(parent),
	main(qobject_cast<MainWindow*>(parent))
{

}

void Screen::quit()
{
	main->endSession();
}
