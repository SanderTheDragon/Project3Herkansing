#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "src/screens/ScreenDebug.hpp"
#include "src/screens/ScreenMain.hpp"
#include "src/screens/ScreenPincode.hpp"
#include "src/screens/ScreenOptions.hpp"
#include "src/screens/ScreenThanks.hpp"
#include "src/screens/ScreenBalance.hpp"
#include "src/screens/ScreenWithdrawSelection.hpp"
#include "src/screens/ScreenWithdrawConfirm.hpp"
#include "src/screens/ScreenWithdrawCustom.hpp"
#include "src/Session.hpp"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow()),
	screen(nullptr),
	arduino(new Arduino(&screen)),
	session(nullptr),
	database(new Database())
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete arduino;
	delete ui;
}

void MainWindow::start()
{
	if (startup())
	{
		setScreen(SCREEN_MAIN);
	}
}

void MainWindow::setScreen(int screenIndex)
{
	Screen* newScreen;

	switch (screenIndex)
	{
	case SCREEN_MAIN:
		newScreen = new ScreenMain(this);
		break;
	case SCREEN_PINCODE:
		newScreen = new ScreenPincode(this);
		break;
	case SCREEN_OPTIONS:
		newScreen = new ScreenOptions(this);
		break;
	case SCREEN_THANKS:
		newScreen = new ScreenThanks(this);
		break;
	case SCREEN_BALANCE:
		newScreen = new ScreenBalance(this);
		break;
	case SCREEN_WITHDRAWSELECTION:
		newScreen = new ScreenWithdrawSelection(this);
		break;
	case SCREEN_WITHDRAWCONFIRM:
		newScreen = new ScreenWithdrawConfirm(this);
		break;
	case SCREEN_WITHDRAWCUSTOM:
		newScreen = new ScreenWithdrawCustom(this);
		break;
	default:
		newScreen = new ScreenDebug(this);
		break;
	}

	if (session != nullptr)
	{
		getDatabase()->logScreen(screen->index(), screenIndex, getSession()->getUID());
	}

	if (screen != nullptr) //Free memory
	{
		delete screen;
	}

	screen = newScreen;
	this->setCentralWidget(screen);
}

bool MainWindow::startup()
{
	ui->taskLabel->setText("Zoeken naar arduino...");
	QApplication::instance()->processEvents();
	if (arduino->find())
	{
		ui->progressBar->setValue(ui->progressBar->value() + 10);
	}
	else
	{
		ui->title->setText("Afgebroken");
		ui->subTitle->setText("Er kon geen Arduino gevonden worden");
		return false;
	}

	ui->taskLabel->setText("Proberen verbinding te maken...");
	QApplication::instance()->processEvents();
	if (arduino->open())
	{
		ui->progressBar->setValue(ui->progressBar->value() + 10);
	}
	else
	{
		ui->title->setText("Afgebroken");
		ui->subTitle->setText("Er kon geen verbinding met de Arduino worden gemaakt");
		return false;
	}

	ui->taskLabel->setText("Arduino verifiëren...");
	QApplication::instance()->processEvents();
	if (arduino->handshake())
	{
		ui->progressBar->setValue(ui->progressBar->value() + 10);
	}
	else
	{
		ui->title->setText("Afgebroken");
		ui->subTitle->setText("De Arduino kon niet worden geverifiëerd");
		return false;
	}

	return true;
}

void MainWindow::newSession(QString uid)
{
	session = new Session(this, uid);
}

void MainWindow::endSession(bool showBye)
{
	session->end();
	delete session;
	session = nullptr;

	if (showBye)
	{
		setScreen(SCREEN_THANKS);
	}
	else
	{
		setScreen(SCREEN_MAIN);
	}
}
