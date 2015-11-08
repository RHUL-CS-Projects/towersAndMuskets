#include <string>

using namespace std;

class ComponentSystem {
private:
	string componentName;

public:
	string getName() { return componentName; }
	void update(float timestep);
};