#include <string>

class TestClass {
private:
  std::string name;  
  
public :
  TestClass() {}
  ~TestClass() {}
  void setName(std::string n);
  void printName();
};