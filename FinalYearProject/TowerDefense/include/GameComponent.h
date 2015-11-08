#include <string>

using namespace std;

class GameComponent {
private:
	string componentName;
	int parentObjectID;
public:
	int getObjectID() { return parentObjectID; }
	string getName() { return componentName; }
};