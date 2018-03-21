#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <QWidget>

#include "src/MainWindow.hpp"

//Screen indexes
#define SCREEN_MAIN					0x00
#define SCREEN_PINCODE				0x01
#define SCREEN_OPTIONS				0x02
#define SCREEN_THANKS				0x03
#define SCREEN_BALANCE				0x04
#define SCREEN_WITHDRAWSELECTION	0x05
#define SCREEN_WITHDRAWCONFIRM		0x06
#define SCREEN_WITHDRAWCUSTOM		0x07

class Screen : public QWidget
{
	Q_OBJECT
public:
	virtual int index() = 0;

	explicit Screen(QWidget* parent = nullptr);
	virtual void handle(QString line) = 0;

protected:
	MainWindow* main;

public slots:
	void quit();
};

#endif // SCREEN_HPP
