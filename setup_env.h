#ifndef _SETUP_ENV_H_
#define _SETUP_ENV_H_

#include <windows.h>
#include <tlhelp32.h>
#include <Windows.h>
#include <tchar.h>
#include <string>

#include "cfg_data.h" // GOAL: Move to tools_ktb
#include "../tools_ktb/logger.h"
#include "../tools_ktb/ConsoleColor.h"
#include "../tools_ktb/pubsub.h"

using namespace std;

//TODO: Create setup_env class

class setup_env
{

	// Return process ID by Window name 
	// Example: Use window name "Minesweeper" instead of process name "Winmine__XP"
	public:
		setup_env(kLogger*); // Constructor
		~setup_env(void);    // Destructor

		// Return process ID by Window name 
		string GetPIDByWinName(string);

		// Return process ID by Window name 
		string GetPIDByWinName(wstring);

		size_t ExecuteProcess(std::wstring, std::wstring, size_t);

	protected:

	private:
		kLogger * log; // declare pointer
};
#endif // _SETUP_ENV_H_