#ifndef GUISUBJECT_H
#define GUISUBJECT_H

#include <list>
#include <GuiObserver.h>

enum GUI_EVENTS {
	mouseOver,
	mousePressed
};

class GuiSubject {
private:
	std::list<GuiObserver*> observers;
public:
	void registerObserver(GuiObserver* o) {
		observers.push_back(o);
	}
	
	virtual void notify(int id, int eventID) {
		for (GuiObserver* o : observers) {
			o->onNotify(id, eventID);
		}
	}
};

#endif