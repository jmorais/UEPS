//**************************************************************************************
//* Ultra Express Process Scheduler														
//* 08/05/2010 - 23:00																	
//* Magrelo Softwares - All Rigths Reversed (c) - 2010									
//* José Morais																
//**************************************************************************************
//* File: ReadyList.h																	
//**************************************************************************************

#ifndef READYLIST_H
#define READYLIST_H

#include "Priority.h"
#include "State.h"
#include "Process.h"
#include "ProcessList.h"
#include "SchedulingAlgorithm.h"

#include <iostream>
#include <string>
#include <list>

using namespace std;
using std::string;

namespace ProcessScheduler {

	class ReadyList : public ProcessList {
	public:
		
		friend class Scheduler;

	};

}

#endif
