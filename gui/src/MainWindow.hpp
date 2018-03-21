#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "src/Arduino.hpp"
#include "src/Database.hpp"

namespace Ui {
	class MainWindow;
}

class Session;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void start();
	void setScreen(int screenIndex = -1);
	bool startup();

	void newSession(QString uid);
	void endSession(bool showBye=true);

	Ui::MainWindow* getUi() { return ui; }
	Arduino* getArduino() { return arduino; }
	Session* getSession() { return session; }
	Database* getDatabase() { return database; }

private:
	Ui::MainWindow* ui;
	Screen* screen;
	Arduino* arduino;
	Session* session;
	Database* database;
};

#endif // MAINWINDOW_HPP
