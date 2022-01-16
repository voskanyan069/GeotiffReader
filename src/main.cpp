#include "application.hpp"

int main(int argc, char* argv[])
{
	Application app(argc, argv);
	int options = app.parse_options();
	if (options)
	{
		return options;
	}
	app.run_tests();
	return 0;
}
