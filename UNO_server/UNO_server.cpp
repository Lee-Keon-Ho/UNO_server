#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("172.30.1.40", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}