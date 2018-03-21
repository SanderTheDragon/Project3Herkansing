#ifndef SCREENWITHDRAWSELECTION_HPP
#define SCREENWITHDRAWSELECTION_HPP

#include <QSignalMapper>

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenWithdrawSelection;
}

class ScreenWithdrawSelection : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_WITHDRAWSELECTION; }

	explicit ScreenWithdrawSelection(QWidget* parent = nullptr);
	~ScreenWithdrawSelection();

	void handle(QString line);

private:
	Ui::ScreenWithdrawSelection* ui;
	QSignalMapper* mapper;

public slots:
	void withdraw(QString amount);
	void withdrawCustom();
	void back();
};

#endif // SCREENWITHDRAWSELECTION_HPP
