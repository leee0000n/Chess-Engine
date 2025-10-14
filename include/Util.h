#ifndef UTIL_H
#define UTIL_H

#include <string>

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(std::string);

//Frees media and shuts down SDL
void close();

#endif 
