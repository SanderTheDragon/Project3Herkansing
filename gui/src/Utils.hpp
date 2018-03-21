#ifndef UTILS_HPP
#define UTILS_HPP

#include <QString>

class Utils
{
public:
	static QString pbkdf2(QString password, QString salt);
	static QString pbkdf2(QString password);
	static bool pbdkf2_verify(QString password, QString hash);
};

#endif // UTILS_HPP
