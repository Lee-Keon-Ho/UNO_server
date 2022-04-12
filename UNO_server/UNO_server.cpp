#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("127.0.0.1", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}