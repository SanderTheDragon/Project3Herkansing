#ifndef SESSION_HPP
#define SESSION_HPP

#include <QObject>

#include "MainWindow.hpp"

class Session : public QObject
{
	Q_OBJECT

public:
	explicit Session(MainWindow* main, QString uid);

	void end();

	bool checkPincode();
	bool withdraw();

	void setWithdrawAmount(int amount) { withdrawAmount = amount; }
	void setTargetNumber(QString target) { targetNumber = target; }

	QString* getPincode() { return &pincode; }
	int getWithdrawAmount() { return withdrawAmount; }
	QString getUID() { return uid; }
	QString getTargetNumber() { return targetNumber; }

private:
	MainWindow* main;

	QString sessionId;
	QString uid;
	QString pincode;

	int withdrawAmount;
	QString targetNumber;
};

#endif // SESSION_HPP
