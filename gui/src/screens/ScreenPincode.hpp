#ifndef SCREENPINCODE_HPP
#define SCREENPINCODE_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenPincode;
}

class ScreenPincode : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_PINCODE; }

	explicit ScreenPincode(QWidget* parent = nullptr);
	~ScreenPincode();

	void handle(QString line);

private:
	Ui::ScreenPincode* ui;
	int attempts;

public slots:
	void back();
	void clear();
	void accept();
};

#endif // SCREENPINCODE_HPP
