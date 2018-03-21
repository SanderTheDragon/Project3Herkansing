#include "ScreenMain.hpp"
#include "ui_ScreenMain.h"

ScreenMain::ScreenMain(QWidget *parent) :
	Screen(parent),
	ui(new Ui::ScreenMain())
{
	ui->setupUi(this);
}

ScreenMain::~ScreenMain()
{
	delete ui;
}

void ScreenMain::handle(QString line)
{
	if (line.startsWith("card:") && line.split(':')[1] != "removed")
	{
		if (main->getDatabase()->cardExists(line.split(':')[1]))
		{
			if (!main->getDatabase()->cardBlocked(line.split(':')[1]))
			{
				main->newSession(line.split(':')[1]);
				main->setScreen(SCREEN_PINCODE);
			}
			else
			{
				ui->error->setText("Kaart is geblokkeerd");
			}
		}
		else
		{
			ui->error->setText("Kaart is onbekend");
		}
	}
	else
	{
		ui->error->setText("");
	}
}
