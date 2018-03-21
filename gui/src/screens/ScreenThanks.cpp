#include "ScreenThanks.hpp"
#include "ui_ScreenThanks.h"

#include <QTimer>

ScreenThanks::ScreenThanks(QWidget *parent) :
	Screen(parent),
	ui(new Ui::ScreenThanks())
{
	ui->setupUi(this);

	QTimer::singleShot(5000, this, SLOT(quit()));
}

ScreenThanks::~ScreenThanks()
{
	delete ui;
}

void ScreenThanks::handle(QString line)
{
	if (line.startsWith("card:") || line.startsWith("key:"))
	{
		quit();
	}
}

void ScreenThanks::quit()
{
	main->setScreen(SCREEN_MAIN);
}
