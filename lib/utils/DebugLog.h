#pragma once 
//#include <iostream>
//#include <sstream>

#ifdef DEBUG
#define LOG(message) logDebug(std::ostringstream() << message)
#else
#define LOG(message) // Define as empty for release builds
#endif

// enum LOGLEVEL
// {
//     DEBUG_ONLY,
//     ALWAYS,
// };

// //#define CURR_LOGLEVEL LL_DEBUG

// Forward declaration of logDebug function
void logDebug(const String& message);

// Log function for DEBUG builds
void logDebug(const std::ostringstream& oss) {
    logDebug(oss.str());    
}

// Actual log function implementation
void logDebug(const String& message) {
    std::cout << message << std::endl;
}