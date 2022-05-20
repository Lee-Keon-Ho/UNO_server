#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("192.168.123.17	", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}