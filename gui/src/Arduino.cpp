#include "Arduino.hpp"

#include <QApplication>
#include <QList>
#include <QtSerialPort/QSerialPortInfo>

#include "external/crc32.h"
#include "screens/Screen.hpp"

typedef uint8_t byte;

Arduino::Arduino(Screen** screen) :
	QObject(),
	screen(screen),
	opened(false),
	ready(false),
	lastline("")
{

}

Arduino::~Arduino()
{
	if (isOpen())
	{
		port.close();
	}
}

bool Arduino::find()
{
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.length(); i++)
	{
		if (ports[i].description().contains("Arduino") || ports[i].description().contains("Genuino"))
		{
			port.setPort(ports[i]);
			port.setBaudRate(QSerialPort::Baud9600);
			break;
		}
	}

	return port.portName().length() > 0;
}

bool Arduino::open()
{
	opened = port.open(QIODevice::ReadWrite);
	return opened;
}

bool Arduino::handshake()
{
	templine.clear();
	lastline = "";

	while (true)
	{
		if (templine.endsWith('\n'))
		{
			lastline = QString(templine).trimmed();

			if (lastline != "handshake")
			{
				break;
			}
			else
			{
				templine.clear();
				lastline = "";
			}
		}
		else
		{
			if (port.canReadLine())
			{
				templine.append(port.readAll());
			}
			else
			{
				QApplication::instance()->processEvents();
			}
		}
	}

	if (lastline.length() != 64)
	{
		return handshake();
	}

	Crc32 crc;
	crc.initInstance(0);

	QByteArray buff;
	for (int i = 0; i < 64; i++)
	{
		buff[i] = templine[i];
	}

	templine.clear();
	lastline = "";

	crc.pushData(0, QString(buff).toStdString().c_str(), 64);
	quint32 target = crc.releaseInstance(0);
	target = (int)((target % 256) ^ 'x') + ((byte)buff[11] + (byte)'0');
	port.write((QString::number(target) + '\n').toStdString().c_str());

	while (true)
	{
		if (templine.endsWith('\n'))
		{
			lastline = QString(templine).trimmed();
			templine.clear();
			break;
		}
		else
		{
			if (port.canReadLine())
			{
				templine.append(port.readAll());
			}
			else
			{
				QApplication::instance()->processEvents();
			}
		}
	}

	if (!lastline.startsWith("OK"))
	{
		lastline = "";
		return handshake();
	}

	hasher.clear();
	for (int i = 0; i < 64; i++)
	{
		hasher.append(buff[i] ^ (target % 32));
	}

	connect(&port, SIGNAL(readyRead()), this, SLOT(handleMessage()));

	ready = true;
	return ready;
}

void Arduino::sendHashed(QString message)
{
	QByteArray hashed;

	for (int i = 0; i < message.length(); i++)
	{
		if (message[i].toLatin1() == hasher[i % 64])
		{
			hashed.append(message[i].toLatin1() ^ (byte)'?');
		}
		else
		{
			hashed.append(message[i].toLatin1() ^ (byte)hasher[i % 64]);
		}
	}

	port.write(QString(hashed + '\n').toStdString().c_str());
}

void Arduino::handleMessage()
{
	templine.append(port.readAll());

	if (templine.contains('\n'))
	{
		QList<QByteArray> parts = templine.split('\n');
		templine = parts[0];
		lastline = "";

		if (QString(templine) == "handshake")
		{
			ready = false;
			templine = "";
			disconnect(this, SLOT(handleMessage()));
			handshake();
			return;
		}

		for (int i = 0; i < templine.length(); i++)
		{
			if ((templine[i] ^ (byte)'?') == hasher[i % 64])
			{
				lastline += QString(templine[i] ^ (byte)'?');
			}
			else
			{
				lastline += QString(templine[i] ^ hasher[i % 64]);
			}
		}

		if (parts.length() > 1)
		{
			parts.removeFirst();
			templine = parts.join('\n');
		}
		else
		{
			templine.clear();
		}
	}

	if (lastline.length() > 0)
	{
		if (lastline.startsWith("ping:"))
		{
			QString pingData = lastline.mid(lastline.indexOf(":") + 1);
			sendHashed("pong:" + pingData);
		}

		if (*screen != nullptr)
		{
			(*screen)->handle(lastline);
		}

		lastline = "";
	}
}
