#include "ScreenOptions.hpp"
#include "ui_ScreenOptions.h"

ScreenOptions::ScreenOptions(QWidget* parent) :
	Screen(parent),
	ui(new Ui::ScreenOptions())
{
	ui->setupUi(this);
}

ScreenOptions::~ScreenOptions()
{
	delete ui;
}

void ScreenOptions::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
		case '1':
			main->setScreen(SCREEN_BALANCE);
			break;
		case '2':
			main->setScreen(SCREEN_WITHDRAWSELECTION);
			break;
		case '-':
			quit();
			break;
		}
	}
	else if (line.startsWith("card:"))
	{
		quit();
	}
}
