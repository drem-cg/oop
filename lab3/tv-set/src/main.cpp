#include "RemoteControl.h"
#include "TVSet.h"
#include <iostream>

int main() {
  CTVSet tv;
  CRemoteControl remote(tv, std::cin, std::cout);

  while (remote.HandleCommand()) {
  }

  return 0;
}