#include "ScreenWithdrawSelection.hpp"
#include "ui_ScreenWithdrawSelection.h"

#include "Session.hpp"

ScreenWithdrawSelection::ScreenWithdrawSelection(QWidget *parent) :
	Screen(parent),
	ui(new Ui::ScreenWithdrawSelection())
{
	ui->setupUi(this);

	mapper = new QSignalMapper(this);

	connect(ui->moneyButton10, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton10, QString::number(10));
	connect(ui->moneyButton20, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton20, QString::number(20));
	connect(ui->moneyButton50, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton50, QString::number(50));

	connect(ui->moneyButton100, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton100, QString::number(100));
	connect(ui->moneyButton200, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton200, QString::number(200));
	connect(ui->moneyButton500, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton500, QString::number(500));

	connect(ui->moneyButton1000, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton1000, QString::number(1000));
	connect(ui->moneyButton2000, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton2000, QString::number(2000));
	connect(ui->moneyButton5000, SIGNAL(pressed()), mapper, SLOT(map()));
	mapper->setMapping(ui->moneyButton5000, QString::number(5000));

	connect(mapper, SIGNAL(mapped(QString)), this, SLOT(withdraw(QString)));
	connect(ui->customButton, SIGNAL(pressed()), this, SLOT(withdrawCustom()));
	connect(ui->backButton, SIGNAL(pressed()), this, SLOT(back()));
	connect(ui->quitButton, SIGNAL(pressed()), this, SLOT(quit()));
}

ScreenWithdrawSelection::~ScreenWithdrawSelection()
{
	delete ui;
}

void ScreenWithdrawSelection::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
		case '0':
			ui->customButton->click();
			break;
		case '1':
			ui->moneyButton10->click();
			break;
		case '2':
			ui->moneyButton100->click();
			break;
		case '3':
			ui->moneyButton1000->click();
			break;
		case '4':
			ui->moneyButton20->click();
			break;
		case '5':
			ui->moneyButton200->click();
			break;
		case '6':
			ui->moneyButton2000->click();
			break;
		case '7':
			ui->moneyButton50->click();
			break;
		case '8':
			ui->moneyButton500->click();
			break;
		case '9':
			ui->moneyButton5000->click();
			break;
		case 'b':
			ui->backButton->click();
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

void ScreenWithdrawSelection::withdraw(QString amount)
{
	main->getSession()->setWithdrawAmount(amount.toInt());
	main->setScreen(SCREEN_WITHDRAWCONFIRM);
}

void ScreenWithdrawSelection::withdrawCustom()
{
	main->setScreen(SCREEN_WITHDRAWCUSTOM);
}

void ScreenWithdrawSelection::back()
{
	main->setScreen(SCREEN_OPTIONS);
}
