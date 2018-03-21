#include "ScreenPincode.hpp"
#include "ui_ScreenPincode.h"

#include "Session.hpp"

ScreenPincode::ScreenPincode(QWidget* parent) :
	Screen(parent),
	ui(new Ui::ScreenPincode()),
	attempts(0)
{
	ui->setupUi(this);

	connect(ui->backButton, SIGNAL(pressed()), this, SLOT(back()));
	connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clear()));
	connect(ui->acceptButton, SIGNAL(pressed()), this, SLOT(accept()));
	connect(ui->quitButton, SIGNAL(pressed()), this, SLOT(quit()));
}

ScreenPincode::~ScreenPincode()
{
	delete ui;
}

void ScreenPincode::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			main->getSession()->getPincode()->append(key);
			ui->picodeInput->setText(ui->picodeInput->text() + '*');
			break;
		case 'b':
			ui->backButton->click();
			break;
		case 'c':
			ui->clearButton->click();
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
		main->endSession(false);
	}
}

void ScreenPincode::back()
{
	ui->picodeInput->setText(ui->picodeInput->text().left(ui->picodeInput->text().length() - 1));
	*main->getSession()->getPincode() = main->getSession()->getPincode()->left(main->getSession()->getPincode()->length() - 1);
}

void ScreenPincode::clear()
{
	ui->picodeInput->clear();
	main->getSession()->getPincode()->clear();
}

void ScreenPincode::accept()
{
	if (main->getSession()->checkPincode())
	{
		main->getDatabase()->loginAttempt(true, main->getSession()->getUID());
		main->setScreen(SCREEN_OPTIONS);
	}
	else
	{
		main->getDatabase()->loginAttempt(false, main->getSession()->getUID());
		ui->error->setText("Onjuiste pincode");
		attempts += 1;
		ui->clearButton->click();

		if (attempts >= 3)
		{
			main->getDatabase()->block(main->getSession()->getUID());
			main->endSession(false);
		}
	}
}
