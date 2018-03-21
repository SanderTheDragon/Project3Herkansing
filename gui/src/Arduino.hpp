#ifndef ARDUINO_HPP
#define ARDUINO_HPP

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Screen;
class Arduino : public QObject
{
	Q_OBJECT

public:
	explicit Arduino(Screen** screen);
	~Arduino();

	bool find();
	bool open();
	bool handshake();

	void sendHashed(QString message);

	bool isOpen() { return opened; }
	bool isReady() { return ready; }

	QSerialPort* getPort() { return &port; }
	QByteArray getHasher() { return hasher; }

private:
	Screen** screen;
	QSerialPort port;

	bool opened;
	bool ready;

	QByteArray templine;
	QByteArray hasher;

	QString lastline;

public slots:
	void handleMessage();
};

#endif // ARDUINO_HPP
