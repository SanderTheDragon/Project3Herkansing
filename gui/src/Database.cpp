#include "Database.hpp"

#include <QSqlQuery>
#include <QSqlResult>
#include <QJsonDocument>
#include <QJsonObject>

Database::Database()
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setUserName("bank");
	db.setPassword("BankBankerBankst");
	db.setDatabaseName("bankst");
	db.open();
}

bool Database::cardExists(QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();

	return query.size() > 0;
}

bool Database::cardBlocked(QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();

	return !query.value("blocked_date").isNull();
}

QString Database::getPincode(QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();

	return query.value("pin").toString();
}

QStringList Database::getAccounts(QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();
	int card = query.value("id").toInt();

	query.prepare("SELECT * FROM `accounts_cards` WHERE `cards_id`=?");
	query.bindValue(0, card);
	query.exec();

	QStringList accounts;
	while (query.next())
	{
		QSqlQuery subQuery;
		subQuery.prepare("SELECT * FROM	`accounts` WHERE `id`=?");
		subQuery.bindValue(0, query.value("accounts_id").toInt());
		subQuery.exec();
		subQuery.next();

		accounts.append(subQuery.value("number").toString());
	}

	return accounts;
}

int Database::getBalance(QString number)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `log` WHERE `type`=?");
	query.bindValue(0, "transaction");
	query.exec();

	int balance = 0;
	while (query.next())
	{
		QString json = query.value("extra_data").toString();

		if (json.contains(number))
		{
			QJsonDocument document(QJsonDocument::fromJson(json.toLatin1()));
			if (document.object()["number"].toString() == number)
			{
				balance += document.object()["amount"].toInt();
			}
		}
	}

	return balance;
}

bool Database::withdraw(int amount, QString number, QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();
	int card = query.value("id").toInt();

	QString data = "{\r\n \"number\": \"" + number + "\",\r\n \"amount\": " + QString::number(-amount) + "\r\n}";
	query.prepare("INSERT INTO `log` (`id`, `type`, `time`, `extra_data`, `cards_id`) VALUES (NULL, 'transaction', CURRENT_TIMESTAMP, ?, ?)");
	query.bindValue(0, data);
	query.bindValue(1, card);
	query.exec();
	db.commit();

	return true;
}

void Database::logScreen(int previous, int next, QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();
	int card = query.value("id").toInt();

	QString data = "{\r\n \"previous\": " + QString::number(previous) + ",\r\n \"next\": " + QString::number(next) + "\r\n}";
	query.prepare("INSERT INTO `log` (`id`, `type`, `time`, `extra_data`, `cards_id`) VALUES (NULL, 'page', CURRENT_TIMESTAMP, ?, ?)");
	query.bindValue(0, data);
	query.bindValue(1, card);
	query.exec();
	db.commit();
}

void Database::loginAttempt(bool success, QString uid)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM `cards` WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	query.next();
	int card = query.value("id").toInt();

	QString data = "{\r\n \"success\": " + QString((success) ? "true" : "false") + "\r\n}";
	query.prepare("INSERT INTO `log` (`id`, `type`, `time`, `extra_data`, `cards_id`) VALUES (NULL, 'login', CURRENT_TIMESTAMP, ?, ?)");
	query.bindValue(0, data);
	query.bindValue(1, card);
	query.exec();
	db.commit();
}

void Database::block(QString uid)
{
	QSqlQuery query;
	query.prepare("UPDATE `cards` SET `blocked_date`=CURRENT_TIMESTAMP WHERE `uid`=?");
	query.bindValue(0, uid);
	query.exec();
	db.commit();
}
