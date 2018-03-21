#ifndef SCREENDEBUG_HPP
#define SCREENDEBUG_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenDebug;
}

class ScreenDebug : public Screen
{
	Q_OBJECT

public:
	int index() { return -1; }

	explicit ScreenDebug(QWidget* parent = nullptr);
	~ScreenDebug();

	void handle(QString line);

private:
	Ui::ScreenDebug* ui;

public slots:
	void loadScreen();
	void hashString();
	void verifyHash();
};

#endif // SCREENDEBUG_HPP
