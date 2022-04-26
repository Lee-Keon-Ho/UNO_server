#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("221.144.254.105", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}