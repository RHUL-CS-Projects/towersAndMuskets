#include <TestClass.h>
#include <iostream>

void TestClass::setName(std::string n) {
  name = n;
}

void TestClass::printName() {
  std::cout << "Name: " << name << std::endl;
}

