#include "RemoteControl.h"
#include "TVSet.h"
#include <iostream>

int main()
{
	CTVSet tv;
	const CRemoteControl remote(tv, std::cin, std::cout);
	return remote.HandleCommand();
}