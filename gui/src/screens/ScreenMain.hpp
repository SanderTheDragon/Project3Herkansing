#ifndef SCREENMAIN_HPP
#define SCREENMAIN_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenMain;
}

class ScreenMain : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_MAIN; }

	explicit ScreenMain(QWidget* parent = nullptr);
	~ScreenMain();

	void handle(QString line);

private:
	Ui::ScreenMain* ui;
};

#endif // SCREENMAIN_HPP
