#include "System.hpp"
namespace kernel
{
	
	uint32_t System::s_systemCoreClock = 8000000;
	uint32_t System::s_peripheralClock1 = 8000000;
	uint32_t System::s_peripheralClock2 = 8000000;
	System System::s_instance;
	bool System::s_initialised = false;
}