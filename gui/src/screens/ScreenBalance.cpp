#include "ScreenBalance.hpp"
#include "ui_ScreenBalance.h"

#include "Session.hpp"

ScreenBalance::ScreenBalance(QWidget *parent) :
	Screen(parent),
	ui(new Ui::ScreenBalance())
{
	ui->setupUi(this);

	connect(ui->quitButton, SIGNAL(pressed()), this, SLOT(quit()));
	connect(ui->backButton, SIGNAL(pressed()), this, SLOT(back()));

	ui->subTitle->setText(ui->subTitle->text().replace("{SALDO}", "0"));

	QStringList labels;
	labels.append("Rekeningnummer");
	labels.append("Saldo");
	ui->tableWidget->setHorizontalHeaderLabels(labels);
	ui->tableWidget->setColumnWidth(1, 240);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 36pt; }");
	QApplication::instance()->processEvents();

	int totalBalance = 0;

	QStringList accounts = main->getDatabase()->getAccounts(main->getSession()->getUID());
	for (int i = 0; i < accounts.length(); i++)
	{
		ui->tableWidget->insertRow(i);
		ui->tableWidget->setItem(i, 0, new QTableWidgetItem(accounts.at(i)));

		ui->progressBar->setValue(ui->progressBar->value() + (100 / accounts.length() / 2));
		QApplication::instance()->processEvents();

		int balance = main->getDatabase()->getBalance(accounts.at(i));
		totalBalance += balance;
		ui->tableWidget->setItem(i, 1, new QTableWidgetItem("€" + QString::number(balance / 100)));
		if (balance < 0)
		{
			ui->tableWidget->item(i, 1)->setTextColor(QColor::fromRgb(255, 0, 0));
		}
		ui->progressBar->setValue(ui->progressBar->value() + (100 / accounts.length() / 2));
		QApplication::instance()->processEvents();
	}

	if (totalBalance < 0)
	{
		ui->subTitle->setText("<html><head/><body><p>Er staat op dit moment €<span style=\" font-weight:600; color:#ff0000;\">" + QString::number(totalBalance / 100) + "</span> op uw rekening(en)</p></body></html>");
	}
	else
	{
		ui->subTitle->setText(ui->subTitle->text().replace("0", QString::number(totalBalance / 100)));
	}
}

ScreenBalance::~ScreenBalance()
{
	delete ui;
}

void ScreenBalance::handle(QString line)
{
	if (line.startsWith("key:"))
	{
		char const key = line.toLower().right(1).toStdString().c_str()[0];

		switch (key)
		{
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

void ScreenBalance::back()
{
	main->setScreen(SCREEN_OPTIONS);
}
