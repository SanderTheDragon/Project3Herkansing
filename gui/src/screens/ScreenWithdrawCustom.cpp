#include "ScreenWithdrawCustom.hpp"
#include "ui_ScreenWithdrawCustom.h"

#include "Session.hpp"

ScreenWithdrawCustom::ScreenWithdrawCustom(QWidget* parent) :
	Screen(parent),
	ui(new Ui::ScreenWithdrawCustom()),
	amount("")
{
	ui->setupUi(this);

	connect(ui->backButton, SIGNAL(pressed()), this, SLOT(back()));
	connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
	connect(ui->acceptButton, SIGNAL(pressed()), this, SLOT(accept()));
	connect(ui->quitButton, SIGNAL(pressed()), this, SLOT(quit()));
	connect(ui->correctButton, SIGNAL(pressed()), this, SLOT(correct()));

	ui->correctButton->setVisible(false);
}

ScreenWithdrawCustom::~ScreenWithdrawCustom()
{
	delete ui;
}

void ScreenWithdrawCustom::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
		case '0':
			if (amount.length() == 0)
			{
				break;
			}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			amount.append(key);
			ui->picodeInput->setText(amount + "0");

			if (key != '0')
			{
				ui->backButton->setVisible(false);
				ui->correctButton->setVisible(true);
			}
			break;
		case 'b':
			if (amount.length() > 0)
			{
				ui->correctButton->click();

				if (amount.length() > 0)
				{
					ui->backButton->setVisible(false);
					ui->correctButton->setVisible(true);
				}
				else
				{
					ui->backButton->setVisible(true);
					ui->correctButton->setVisible(false);
				}
			}
			else
			{
				ui->backButton->click();
			}
			break;
		case 'c':
			ui->clearButton->click();
			ui->backButton->setVisible(true);
			ui->correctButton->setVisible(false);
			break;
		case '-':
			quit();
			break;
		case '+':
			ui->acceptButton->click();
			break;
		}
	}
	else if (line.startsWith("card:"))
	{
		quit();
	}
}

void ScreenWithdrawCustom::back()
{
	main->setScreen(SCREEN_WITHDRAWSELECTION);
}

void ScreenWithdrawCustom::clear()
{
	ui->picodeInput->setText("0");
	amount.clear();
}

void ScreenWithdrawCustom::accept()
{
	if (amount.length() > 9)
	{
		ui->error->setText("Bedrag te hoog");
		return;
	}

	if ((amount.toInt() * 10) > 0)
	{
		main->getSession()->setWithdrawAmount(amount.toInt() * 10);
		main->setScreen(SCREEN_WITHDRAWCONFIRM);
	}
	else
	{
		ui->error->setText("U kunt niet €0 opnemen");
	}
}

void ScreenWithdrawCustom::correct()
{
	amount = amount.left(amount.length() - 1);
	ui->picodeInput->setText(amount + "0");
}
