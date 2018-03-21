#ifndef SCREENWITHDRAWCONFIRM_HPP
#define SCREENWITHDRAWCONFIRM_HPP

#include <QSignalMapper>
#include <QPushButton>

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenWithdrawConfirm;
}

class ScreenWithdrawConfirm : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_WITHDRAWCONFIRM; }

	explicit ScreenWithdrawConfirm(QWidget* parent = nullptr);
	~ScreenWithdrawConfirm();

	void handle(QString line);

private:
	Ui::ScreenWithdrawConfirm* ui;
	QSignalMapper* mapper;
	QString target;
	QPushButton* select[10];

public slots:
	void back();
	void accept();
	void setTarget(QString target);
};

#endif // SCREENWITHDRAWCONFIRM_HPP
