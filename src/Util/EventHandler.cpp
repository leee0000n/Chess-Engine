#include "EventHandler.h"

#include "Globals.h"

EventHandler::EventHandler() { }

EventHandler::EventHandler(std::array<EventHandler*, OBSERVER_FUNC_COUNT> observerFuncs) {
	m_observers = observerFuncs;
}

void EventHandler::pollEvent() {
	SDL_Event e;

	// Loop whilst number of events polled is greater than 0
	while (SDL_PollEvent(&e) != 0) {
		handleEvent(e);
	}
}


void EventHandler::handleEvent(SDL_Event &e) {
	switch (e.type) {

	case SDL_QUIT:
		quit = true;
		break;

	default:
		m_observers[CHESS_CLASS]->handleEvent(e);
		break;
	}
}