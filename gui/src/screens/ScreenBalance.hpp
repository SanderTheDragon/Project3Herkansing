#ifndef SCREENBALANCE_HPP
#define SCREENBALANCE_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenBalance;
}

class ScreenBalance : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_BALANCE; }

	explicit ScreenBalance(QWidget* parent = nullptr);
	~ScreenBalance();

	void handle(QString line);

private:
	Ui::ScreenBalance* ui;

public slots:
	void back();
};

#endif // SCREENBALANCE_HPP
