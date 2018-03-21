#ifndef SCREENWITHDRAWCUSTOM_HPP
#define SCREENWITHDRAWCUSTOM_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenWithdrawCustom;
}

class ScreenWithdrawCustom : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_WITHDRAWCUSTOM; }

	explicit ScreenWithdrawCustom(QWidget* parent = nullptr);
	~ScreenWithdrawCustom();

	void handle(QString line);

private:
	Ui::ScreenWithdrawCustom* ui;
	QString amount;

public slots:
	void back();
	void clear();
	void accept();
	void correct();
};

#endif // SCREENWITHDRAWCUSTOM_HPP
