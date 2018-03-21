#ifndef SCREENTHANKS_HPP
#define SCREENTHANKS_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenThanks;
}

class ScreenThanks : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_THANKS; }

	explicit ScreenThanks(QWidget* parent = nullptr);
	~ScreenThanks();

	void handle(QString line);

private:
	Ui::ScreenThanks* ui;

public slots:
	void quit();
};

#endif // SCREENTHANKS_HPP
