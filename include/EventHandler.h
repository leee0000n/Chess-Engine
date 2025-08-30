#pragma once

#include <SDL_events.h>
#include <array>

/*
* To use this event handler:
*   - set OBSERVER_FUNC_COUNT to correct number of observer functions
*   - edit event handler to handle all needed events
* For readability, define the indexes of observers
*/

// Set number of observer functions required by event handler
#define OBSERVER_FUNC_COUNT 1

// Define indexes for observer functions to make it easier
#define CHESS_CLASS 0

/*
* Event handler based off of observer pattern
* https://en.wikipedia.org/wiki/Observer_pattern
* Captures events by polling them
*/
class EventHandler {
public:
	/*
	* Default constructor used for inheritance purposes
	* Without it, the compiler error "No default constructor exists for EventHandler" is thrown
	*/
	EventHandler();

	/*
	* Construct EventHandler
	*/
	EventHandler(std::array<EventHandler*, OBSERVER_FUNC_COUNT> observerFuncs);

	/*
	* Poll events and pass to handleEvent()
	*/
	void pollEvent();

	/*
	* Handle event by sending them to correct observer class's event handler function
	*/
	virtual void handleEvent(SDL_Event &e);


private:

	// Array of pointers to observer classes
	std::array<EventHandler*, OBSERVER_FUNC_COUNT> m_observers;
};

