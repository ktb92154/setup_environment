﻿// setup_env.cpp : Defines the entry point for the console application.
//
/*
Goal: Create a .exe that starts up an array of executables.
Example: Click setup_env.exe -> which opens minesweeper.exe, RemoteDLL.exe, and others
Status: I'm able to click on the exe which spawns processes for minesweeper.exe and remoteDLL.exe.
        But it requires duplicating the code for every executable path.

Issues: Code is more clunky then I'd like, unable to get createProcess to read an array of exe paths.
        Ideally, input an array -> into method -> which spawns # of processes depending on array size.
*/

#include "setup_env.h"

using namespace std;

// TODO: create method to return process ID by process name

// Constructor
setup_env::setup_env(kLogger* logger) 
{
	// Get value pointed to by logger address.
	// dereference operator (*)
	kLogger * log = logger; // declare pointer
	log->Debug("Created logger for setup_env class");
}

// Destructor
setup_env::~setup_env(void)
{
}

// Goal: return process ID by window name
// Example: Use window name "Minesweeper" instead of process name "Winmine__XP"
string setup_env::GetPIDByWinName(string windowName)
{
	DWORD * PID = new DWORD;

	// std::string -> std::wstring
	std::wstring wStrWinName;
	wStrWinName.assign(windowName.begin(), windowName.end());

	HWND windowHandle = FindWindowW(NULL, wStrWinName.c_str());

	if (windowHandle) {
		GetWindowThreadProcessId(windowHandle, PID);

		std::string strPID = std::to_string(*PID);

		log->Debug("Found window: " + windowName + " with PID: " + strPID);
		return strPID;
	}
	else
	{
		log->Warning("No window named: " + windowName);
		return "";
	}
	return "";
}

// Goal: return process ID by window name
// Example: Use window name "Minesweeper" instead of process name "Winmine__XP"
string setup_env::GetPIDByWinName(wstring windowName)
{
	DWORD * PID = new DWORD;
	HWND windowHandle = FindWindowW(NULL, windowName.c_str());

	// wstring -> string
	std::string strWinName;
	strWinName.assign(windowName.begin(), windowName.end());

	if (windowHandle) 
	{
		GetWindowThreadProcessId(windowHandle, PID);

		std::string strPID = std::to_string(*PID);

		log->Debug("Found window: " + strWinName + " with PID: " + strPID);
		return strPID;
	}
	else
	{
		log->Warning("No window named: " + strWinName);
		return "";
	}
	return "";
}

size_t setup_env::ExecuteProcess(std::wstring FullPathToExe, std::wstring Parameters, size_t SecondsToWait)
{
	/* CreateProcess API initialization */
	STARTUPINFOW siStartupInfo = { 0 };
	PROCESS_INFORMATION piProcessInfo = { 0 };
	size_t iMyCounter = 0, iReturnVal = 0, iPos = 0;
	DWORD dwExitCode = 0;
	std::wstring sTempStr = L"";

	/* - NOTE - You should check here to see if the exe even exists */

	/* Add a space to the beginning of the Parameters */
	if (Parameters.size() != 0)
	{
		if (Parameters[0] != L' ')
		{
			Parameters.insert(0, L" ");
		}
	}

	/* The first parameter needs to be the exe itself */
	sTempStr = FullPathToExe;

	log->Debug(sTempStr);

	iPos = sTempStr.find_last_of(L"\\");
	sTempStr.erase(0, iPos + 1);
	Parameters = sTempStr.append(Parameters);

	/* CreateProcessW can modify Parameters thus we allocate needed memory */
	wchar_t * pwszParam = new wchar_t[Parameters.size() + 1];
	if (pwszParam == 0)
	{
		return 1;
	}
	const wchar_t * pchrTemp = Parameters.c_str();
	wcscpy_s(pwszParam, Parameters.size() + 1, pchrTemp);

	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	std::cout << green << "Main process: " << piProcessInfo.dwProcessId << white << std::endl;

	BOOL result = CreateProcessW(const_cast<LPCWSTR>(FullPathToExe.c_str()),
		pwszParam,      // Command line
		0,              // Process handle not inheritable
		0,              // Thread handle not inheritable
		false,          // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE, // Create separate terminal, otherwise use CREATE_DEFAULT_ERROR_MODE
							//CREATE_DEFAULT_ERROR_MODE, // No creation flags and no terminal
		0,              // Use parent's environment block
		0,              // Use parent's starting directory 
		&siStartupInfo, // Pointer to STARTUPINFO structure
		&piProcessInfo  // Pointer to PROCESS_INFORMATION structure
		);

	if(result)
	{
		/* Watch the process. */
		DWORD pid = GetProcessId(piProcessInfo.hProcess);
		//std::cout << green << "Created PID: " << pid << " for " << wstrTostr(FullPathToExe.c_str()) << white << std::endl;
		std::cout << green << "Created PID: " << pid << white << std::endl;

		//dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, INFINITE);
		dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (SecondsToWait * 1000));


	}
	else
	{
		/* CreateProcess failed */
		iReturnVal = GetLastError();
		std::cout << red << "Create new PID failed: " << iReturnVal << white << std::endl;
	}

	/* Free memory */
	delete[]pwszParam;
	pwszParam = 0;

	/* Release handles */
	CloseHandle(piProcessInfo.hProcess);
	CloseHandle(piProcessInfo.hThread);

	return iReturnVal;
}



