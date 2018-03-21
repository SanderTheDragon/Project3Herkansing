#ifndef SCREENOPTIONS_HPP
#define SCREENOPTIONS_HPP

#include "src/screens/Screen.hpp"

namespace Ui {
	class ScreenOptions;
}

class ScreenOptions : public Screen
{
	Q_OBJECT

public:
	int index() { return SCREEN_OPTIONS; }

	explicit ScreenOptions(QWidget* parent = nullptr);
	~ScreenOptions();

	void handle(QString line);

private:
	Ui::ScreenOptions* ui;
};

#endif // SCREENOPTIONS_HPP
