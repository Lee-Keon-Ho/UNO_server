#include "app.h"

int main(void)
{
	CApp app;

	if (!app.Initialize("211.218.197.24", 30002)) return 0;

	app.Update();

	app.Cleanup();

	return 0;
}	