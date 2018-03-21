#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QSqlDatabase>

class Database
{
public:
	Database();

	bool cardExists(QString uid);
	bool cardBlocked(QString uid);
	QString getPincode(QString uid);
	QStringList getAccounts(QString uid);
	int getBalance(QString number);
	bool withdraw(int amount, QString number, QString uid);
	void logScreen(int previous, int next, QString uid);
	void loginAttempt(bool success, QString uid);
	void block(QString uid);

private:
	QSqlDatabase db;
};

#endif // DATABASE_HPP
