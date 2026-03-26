#include "RemoteControl.h"
#include "TVSet.h"
#include <iostream>

int main()
{
	TVSet tv;
	const RemoteControl remote(tv, std::cin, std::cout);
	return remote.HandleCommand();
}