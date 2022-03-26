#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("14.54.160.37", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}