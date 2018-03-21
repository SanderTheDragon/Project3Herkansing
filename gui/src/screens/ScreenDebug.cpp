#include "ScreenDebug.hpp"
#include "ui_ScreenDebug.h"

#include "src/MainWindow.hpp"
#include "src/Utils.hpp"

ScreenDebug::ScreenDebug(QWidget* parent) :
	Screen(parent),
	ui(new Ui::ScreenDebug())
{
	ui->setupUi(this);

	ui->loadScreenSelection->addItem("Main", QVariant(SCREEN_MAIN));
	ui->loadScreenSelection->addItem("Pincode", QVariant(SCREEN_PINCODE));
	ui->loadScreenSelection->addItem("Options", QVariant(SCREEN_OPTIONS));
	ui->loadScreenSelection->addItem("Thanks", QVariant(SCREEN_THANKS));
	ui->loadScreenSelection->addItem("Balance", QVariant(SCREEN_BALANCE));
	ui->loadScreenSelection->addItem("WithdrawSelection", QVariant(SCREEN_WITHDRAWSELECTION));
	ui->loadScreenSelection->addItem("WithdrawConfirm", QVariant(SCREEN_WITHDRAWCONFIRM));
	ui->loadScreenSelection->addItem("WithdrawCustom", QVariant(SCREEN_WITHDRAWCUSTOM));

	connect(ui->quitButton, SIGNAL(released()), QCoreApplication::instance(), SLOT(quit()));
	connect(ui->loadScreenSubmit, SIGNAL(released()), this, SLOT(loadScreen()));
	connect(ui->hashButton, SIGNAL(released()), this, SLOT(hashString()));
	connect(ui->hashString, SIGNAL(returnPressed()), this, SLOT(verifyHash()));
	connect(ui->hashHash, SIGNAL(returnPressed()), this, SLOT(verifyHash()));
	connect(ui->hashString, SIGNAL(textChanged(QString)), ui->hashVerifyLabel, SLOT(clear()));
	connect(ui->hashHash, SIGNAL(textChanged(QString)), ui->hashVerifyLabel, SLOT(clear()));
}

ScreenDebug::~ScreenDebug()
{
	delete ui;
}

void ScreenDebug::handle(QString line)
{
	ui->arduinoDataList->addItem(line);
	ui->arduinoDataList->scrollToBottom();

	ui->portName->setText("/dev/" + main->getArduino()->getPort()->portName());
	ui->hasher->setText(QString(main->getArduino()->getHasher()));
}

void ScreenDebug::loadScreen()
{
	main->setScreen(ui->loadScreenSelection->currentData().toInt());
}

void ScreenDebug::hashString()
{
	if (ui->hashString->text().length() > 0)
	{
		ui->hashHash->setText(Utils::pbkdf2(ui->hashString->text()));
	}

	verifyHash();
}

void ScreenDebug::verifyHash()
{
	bool result = false;

	if (ui->hashString->text().length() > 0 && ui->hashHash->text().length() > 0)
	{
		result = Utils::pbdkf2_verify(ui->hashString->text(), ui->hashHash->text());
	}

	ui->hashVerifyLabel->setText((result) ? "true" : "false");
}
