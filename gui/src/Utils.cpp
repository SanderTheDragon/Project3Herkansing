#include "Utils.hpp"

#include <QtCrypto/QtCrypto>

#include "external/csprng.hpp"

QString Utils::pbkdf2(QString password, QString salt)
{
	QCA::InitializationVector sSalt(QCA::SecureArray(salt.toUtf8()));
	QCA::SecureArray sPassword(password.toUtf8());
	QCA::SymmetricKey hashword = QCA::PBKDF2("sha1", "qca-ossl").makeKey(sPassword, sSalt, 256, 2048);

	return salt + "$" + QCA::arrayToHex(hashword.toByteArray());
}

QString Utils::pbkdf2(QString password)
{
	duthomhas::csprng rnd;
	int data[128];
	rnd(data, 128);

	QString salt;
	for (int i = 0; i < 128; i++)
	{
		salt += '0' + (abs(data[i]) % 10);
	}

	return pbkdf2(password, salt);
}

bool Utils::pbdkf2_verify(QString password, QString hash)
{
	if (hash.contains('$'))
	{
		return pbkdf2(password, hash.split('$')[0]) == hash;
	}

	return false;
}
