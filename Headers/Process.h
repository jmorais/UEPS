//**************************************************************************************
//* Ultra Express Process Scheduler														
//* 08/05/2010 - 23:00																	
//* Magrelo Softwares - All Rigths Reversed (c) - 2010									
//* José Morais																
//**************************************************************************************
//* File: Process.h																		
//**************************************************************************************

#ifndef PROCESS_H
#define PROCESS_H

#include "Priority.h"
#include "State.h"
#include "Process.h"

#include <iostream>
#include <string>

using namespace std;
using std::string;

namespace ProcessScheduler {
	
	class Process {
	private:
		
		string name;						//Nome do Processo
		int PID;								//Process IDentification - Identificador do Processo
		int time;								//Tempo necessário para o término do processo
		int timeCPU;						//Tempo de uso da CPU
		int memory;             //Memória necessária para a alocação do processo
		Priority priority;			//Prioridade do processo
		State state;     				//Estado do processo

	public:
				
		//******************************************************************************
		//* Construtores														        
		//******************************************************************************
		
		Process() {
			this->name = "";
			this->PID = 0;
			this->time = 0;
			this->timeCPU = 0;
			this->memory = 0;
			this->priority = LOW;
			this->state = READY;
		}
		
		Process(string name, int PID, int time, int memory, Priority priority = LOW) {
			
			this->name = name;
			this->PID = PID;
			this->time = time;
			this->timeCPU = 0;
			this->memory = memory;
			this->priority = priority;
			this->state = READY;
			
		}
		
		//******************************************************************************
		//* Operadores		   															
		//******************************************************************************
		
		void operator=(Process process) {
			
			this->name = process.name;
			this->PID = process.PID;
			this->time = process.time;
			this->timeCPU = process.timeCPU;
			this->memory = process.memory;
			this->priority = process.priority;
			this->state = process.state;
			
		}
		
		bool operator==(Process process) {
			
			return this->name == process.name &&
			this->PID == process.PID &&
			this->time == process.time &&
			this->memory == process.memory &&
			this->priority == process.priority &&
			this->state == process.state;
			
		}
		
		//******************************************************************************
		//* Metodos (Getters)															
		//******************************************************************************
		
		string getName() {
			
			return name;
			
		}
		
		int getPID() {
			
			return PID;
			
		}
		
		int getTime() {
			
			return time;
			
		}
		
		int getTimeCPU() {
			
			return timeCPU;
			
		}
		
		int getMemory() {
			
			return memory;
			
		}
		
		Priority getPriority() {
			
			return priority;
			
		}
		
		State getState() {
			
			return state;
			
		}
		
		
		void restartTimeCPU() {
		
			timeCPU = 0;
			
		}
		
		void decreaseTime() {
			
			time--;
			timeCPU++;
			
		}
		
	};

}
#endif
