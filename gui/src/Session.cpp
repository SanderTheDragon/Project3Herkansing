#include "Session.hpp"

#include "external/csprng.hpp"
#include "Utils.hpp"

Session::Session(MainWindow* main, QString uid) :
	QObject(),
	main(main),
	uid(uid),
	pincode(""),
	withdrawAmount(0),
	targetNumber("")
{
	duthomhas::csprng rnd;
	int data[64];
	rnd(data, 64);

	for (int i = 0; i < 64; i++)
	{
		sessionId += QString::number(data[i]);
	}
}

void Session::end()
{

}

bool Session::checkPincode()
{
	return Utils::pbdkf2_verify(pincode, main->getDatabase()->getPincode(uid));
}

bool Session::withdraw()
{
	return main->getDatabase()->withdraw(withdrawAmount * 100, targetNumber, uid);
}
