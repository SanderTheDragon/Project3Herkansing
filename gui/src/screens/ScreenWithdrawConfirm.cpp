#include "ScreenWithdrawConfirm.hpp"
#include "ui_ScreenWithdrawConfirm.h"

#include "Session.hpp"

ScreenWithdrawConfirm::ScreenWithdrawConfirm(QWidget *parent) :
	Screen(parent),
	ui(new Ui::ScreenWithdrawConfirm()),
	target("")
{
	ui->setupUi(this);

	mapper = new QSignalMapper();

	ui->subTitle->setText(ui->subTitle->text().replace("{BEDRAG}", QString::number(main->getSession()->getWithdrawAmount())));

	connect(ui->quitButton, SIGNAL(pressed()), this, SLOT(quit()));
	connect(ui->backButton, SIGNAL(pressed()), this, SLOT(back()));
	connect(ui->acceptButton, SIGNAL(pressed()), this, SLOT(accept()));

	QStringList labels;
	labels.append("");
	labels.append("Rekeningnummer");
	labels.append("Saldo");
	ui->tableWidget->setHorizontalHeaderLabels(labels);
	ui->tableWidget->setColumnWidth(2, 240);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 36pt; }");
	QApplication::instance()->processEvents();

	QStringList accounts = main->getDatabase()->getAccounts(main->getSession()->getUID());
	for (int i = 0; i < accounts.length(); i++)
	{
		select[i] = new QPushButton(this);
		select[i]->setText("[" + QString::number(i + 1) + "]");
		connect(select[i], SIGNAL(pressed()), mapper, SLOT(map()));
		mapper->setMapping(select[i], accounts[i]);

		ui->tableWidget->insertRow(i);
		ui->tableWidget->setRowHeight(i, 72);
		ui->tableWidget->setCellWidget(i, 0, select[i]);
		ui->tableWidget->setItem(i, 1, new QTableWidgetItem(accounts.at(i)));

		ui->progressBar->setValue(ui->progressBar->value() + (100 / accounts.length() / 2));
		QApplication::instance()->processEvents();

		int balance = main->getDatabase()->getBalance(accounts.at(i));
		ui->tableWidget->setItem(i, 2, new QTableWidgetItem("€" + QString::number(balance / 100)));

		if (balance / 100 < main->getSession()->getWithdrawAmount())
		{
			ui->tableWidget->item(i, 2)->setTextColor(QColor::fromRgb(255, 0, 0));
			select[i]->setEnabled(false);
		}
		else
		{
			if (target.isEmpty())
			{
				setTarget(accounts.at(i));
			}
		}
		ui->progressBar->setValue(ui->progressBar->value() + (100 / accounts.length() / 2));
		QApplication::instance()->processEvents();
	}

	ui->tableWidget->hideColumn(2);

	if (target.isEmpty())
	{
		ui->error->setText("U heeft niet genoeg saldo op één van uw rekeningen");
		ui->acceptButton->setEnabled(false);
	}

	connect(mapper, SIGNAL(mapped(QString)), this, SLOT(setTarget(QString)));
}

ScreenWithdrawConfirm::~ScreenWithdrawConfirm()
{
	delete ui;
}

void ScreenWithdrawConfirm::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			int num = QString(key).toInt();
			if (ui->tableWidget->rowCount() >= num)
			{
				((QPushButton*)ui->tableWidget->cellWidget(num - 1, 0))->click();
			}
			break;
		}
		case 'b':
			ui->backButton->click();
			break;
		case 'd':
			ui->tableWidget->showColumn(2);
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

void ScreenWithdrawConfirm::back()
{
	main->getSession()->setWithdrawAmount(0);
	main->setScreen(SCREEN_WITHDRAWSELECTION);
}

void ScreenWithdrawConfirm::accept()
{
	main->getSession()->setTargetNumber(target);

	if (!main->getSession()->withdraw())
	{
		ui->error->setText("Er is iets fout gegaan, probeer het opnieuw");
	}
	else
	{
		quit();
	}
}

void ScreenWithdrawConfirm::setTarget(QString target)
{
	this->target = target;
	ui->subTitle->setText("U staat op het punt €{BEDRAG} op te nemen van");
	ui->subTitle->setText(ui->subTitle->text().replace("{BEDRAG}", QString::number(main->getSession()->getWithdrawAmount())));
	ui->subTitle->setText(ui->subTitle->text() + " " + this->target);
}
