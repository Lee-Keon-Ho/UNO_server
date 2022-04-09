#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("59.30.46.232", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}