// setup_env.cpp : Defines the entry point for the console application.
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

#include "../tools_ktb/ConsoleColor.h"
#include "../tools_ktb/pubsub.h"
#include "../tools_ktb/logger.h"
//#include "../tools_ktb/cfg_tool.h"

#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/file_mapping.hpp>

using namespace std;
using namespace boost::interprocess;

void print_quotient(float, float);

void print_string(std::string);

int main()
{

	kLogger log;

	log.InitLog("test2"); // Initialize log with filename test2 + date
	
	// Test Logging
	/*
		log.Trace("A trace severity message 1");
		log.Debug("Debug");
		log.Info("Info");
		log.Warning("A warning severity message");
		log.Fatal("A fatal severity message"); 
	*/

	// Signal with no arguments and a void return value
	boost::signals2::signal<void(float, float)> sig2;
	sig2.connect(&print_quotient);

	sig2(5., 3.);
	// Initialize and read configuration file for dir paths
	cfg_data cfg("config.ini");

	std::wstring program = cfg.wget("Paths.Minesweeper"); // WORKS!

	// Pass address of logger
	// Address-of operator (&)
	setup_env setup(&log); // initialize setup_env class

	setup.ExecuteProcess(program, L"", 1); // WORKS

	//std::cout << "Test 1: " << setup.GetPIDByWinName("Minesweeper") << std::endl; //WORKS
	//std::cout << "Test 2: " << setup.GetPIDByWinName(L"Minesweeper") << std::endl; //WORKS
	//std::cout << "Bad Test 3: " << setup.GetPIDByWinName("Minesweeper2") << std::endl; //Test failure, should return ""
	//std::cout << "Bad Test 4: " << setup.GetPIDByWinName(L"Minesweeper2") << std::endl; //Test failure, should return ""

	std::wstring dll = cfg.wget("Paths.Dll"); // WORKS!
	setup.ExecuteProcess(dll, L"", 1); // WORKS

	using namespace boost::interprocess;
	typedef std::pair<double, int> MyType;

	try {
		//An special shared memory where we can
		//construct objects associated with a name.
		//First remove any old shared memory of the same name, create 
		//the shared memory segment and initialize needed resources
		shared_memory_object::remove("MySharedMemory");
		managed_shared_memory segment
			//create       segment name    segment size
			(create_only, "MySharedMemory", 65536);

		//Create an object of MyType initialized to {0.0, 0}
		MyType *instance = segment.construct<MyType>
			("MyType instance")  //name of the object
			(0.0, 0);            //ctor first argument

								 //Create an array of 10 elements of MyType initialized to {0.0, 0}
		MyType *array = segment.construct<MyType>
			("MyType array")     //name of the object
			[10]                 //number of elements
		(0.0, 0);            //Same two ctor arguments for all objects

							 //Create an array of 3 elements of MyType initializing each one
							 //to a different value {0.0, 0}, {1.0, 1}, {2.0, 2}...
		float float_initializer[3] = { 0.0, 1.0, 2.0 };
		int   int_initializer[3] = { 0, 1, 2 };

		MyType *array_it = segment.construct_it<MyType>
			("MyType array from it")   //name of the object
			[3]                        //number of elements
		(&float_initializer[0]    //Iterator for the 1st ctor argument
			, &int_initializer[0]);    //Iterator for the 2nd ctor argument
	}
	catch (...) {
		shared_memory_object::remove("MySharedMemory");
		throw;
	}

	std::cout << red << "Press any key to exit terminal" << white << std::endl;
	std::cin.get();
	shared_memory_object::remove("MySharedMemory");
	return 0;
}

void print_quotient(float x, float y)
{
	std::cout << "The quotient is " << x / y << std::endl;
}

void print_string(std::string x)
{

	std::cout << "The string is " << x << std::endl;
}

