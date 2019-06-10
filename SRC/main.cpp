#include "Application.h"

int main()
{
	Application* app_ = new Application();

	bool appRunning_ = true;

	while (appRunning_)
	{
		appRunning_ = app_->Run();
	}

	return 0;
}