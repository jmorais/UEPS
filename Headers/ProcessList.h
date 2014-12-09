//**************************************************************************************
//* Ultra Express Process Scheduler														
//* 08/05/2010 - 23:00																	
//* Magrelo Softwares - All Rigths Reversed (c) - 2010									
//* José Morais																
//**************************************************************************************
//* File: ProcessList.h																	
//**************************************************************************************

#ifndef PROCESSLIST_H
#define PROCESSLIST_H

#include "Priority.h"
#include "State.h"
#include "Process.h"
#include "SchedulingAlgorithm.h"

#include <iostream>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;
using std::string;

namespace ProcessScheduler {

	//******************************************************************************
	//* Metodos Auxiliares															
	//******************************************************************************

	bool compFifo(Process a, Process b) {
		return a.getPID() < b.getPID();
	}

	bool compSjf(Process a, Process b) {
		return a.getTime() < b.getTime();
	}

	bool compListaPriority(Process a, Process b) {
		return a.getPriority() < b.getPriority();
	}

	//******************************************************************************
	//* Classe ProcessList															
	//******************************************************************************

	class ProcessList {
	protected:

		deque<Process> lista;
		int size;

	public:

		void erase () {
            lista.erase(lista.begin(), lista.end());
        }

		deque<Process>::iterator begin() {

			return lista.begin();

		}

		deque<Process>::iterator end() {

			return lista.end();

		}

		void insertProcess(Process process) {

			lista.push_back(process);
			size++;

		}

		void removeProcess() {

			lista.pop_front();
			size--;

		}
		
		void removeProcess(int i) {
			
			lista.erase(lista.begin()+i);
			size--;
			
		}

		Process getFirstProcess() {

			Process aux;
			aux = lista.front();
			removeProcess();
			return aux;

		}

		bool isEmpty() {

			return lista.empty();

        }

		int getSize() {

			return size;

		}

	};

}

#endif
