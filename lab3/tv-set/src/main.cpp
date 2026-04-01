#include "RemoteControl.h"
#include "TVSet.h"
#include <iostream>

int main()
{
	TVSet tv;
	RemoteControl remote(tv, std::cin, std::cout);

	while (remote.HandleCommand())
	{
	}

	return 0;
}