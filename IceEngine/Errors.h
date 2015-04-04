#pragma once
#include <string>
//This is a place to define any error functions
//Not made into a class so an object doesn't have t be made each time
//any functions want to be used.
namespace IceEngine
{
	extern void FATAL_ERROR(std::string errorString, bool log = true);
}