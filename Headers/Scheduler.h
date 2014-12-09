//**************************************************************************************
//* Ultra Express Process Scheduler														
//* 08/05/2010 - 23:00																	
//* Magrelo Softwares - All Rigths Reversed (c) - 2010									
//* José Morais																
//**************************************************************************************
//* File: Scheduler.h																	
//**************************************************************************************

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Priority.h"
#include "State.h"
#include "Process.h"
#include "ProcessList.h"
#include "ReadyList.h"
#include "SleepList.h"
#include "SchedulingAlgorithm.h"

#include <iostream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;
using std::string;

namespace ProcessScheduler {

	class Scheduler {
	private:

		//******************************************************************************
		//* Atributos do Escalonador (Listas)											
		//******************************************************************************

		ReadyList readyList;											//Lista de processos em estado Pronto
		SleepList sleepList;											//Lista de processos em estado Bloqueado
		int size;																	//Quantidade de processos em estado Pronto
		int blocked;  														//Quantidade de processos em estado Bloqueado

		//******************************************************************************
		//* Atributos do Escalonador (Processos)										
		//******************************************************************************

		int lastID;																//PID do último processo criado
		int quantum;															//Tempo máximo de processamento
		Process onCPU;														//Processo que esté sendo processado na CPU

		//******************************************************************************
		//* Atributos do Gerente de Memória										        
		//******************************************************************************

		int totalMemory;													//Memória total do sistema
		int freeMemory;														//Memória livre no sistema
		int usedMemory;														//Memória em uso no sistema

		//******************************************************************************
		//* Atributos do Escalonador (Algoritmos)										
		//******************************************************************************

		SchedulingAlgorithm schedulingAlgorithm;	//Algoritmo de escalonamento utilizado
		string sAlgorithmName;										//Nome da politica de escalonamento
		bool preemptive;													//Tipo da politica de escalonamento (preemptiva/não-preemptiva)

	public:

		//******************************************************************************
		//* Construtor															        
		//******************************************************************************

		Scheduler() {

			lastID = 1;
			size = 0;
			blocked = 0;

			quantum = 10;

			totalMemory = 1000;
			freeMemory = totalMemory;
			usedMemory = 0;

			Process aux;
			onCPU = aux;

			schedulingAlgorithm = FIFO;
			sAlgorithmName = "FIFO";
			preemptive = false;

		}

		//******************************************************************************
		//* Operadores															        
		//******************************************************************************

		//Retorna o i-ésimo processo na lista de prontos
		Process operator[](int i) {

			return readyList.lista[i];

		}

		//Retorna o i-ésimo processo na lista de espera
		Process index(int i) {

			return sleepList.lista[i];

		}

		//******************************************************************************
		//* Metodos (Setters)													        
		//******************************************************************************

		//Modifica o algoritmo de escalonamento
		void setSchedulingAlgorithm(SchedulingAlgorithm schedulingAlgorithm) {

			this->schedulingAlgorithm = schedulingAlgorithm;

			switch (schedulingAlgorithm) {
				case FIFO: sAlgorithmName = "FIFO"; preemptive = false; break;
				case SJF : sAlgorithmName = "Shortest Job First"; preemptive = false; break;
				case ROUND_ROBIN: sAlgorithmName = "Round Robin"; preemptive = true; break;
				case SRT: sAlgorithmName = "Shortest Remaining Time"; preemptive = true; break;
			}
			
			scheduling();
		}

		void setQuantum(int quantum) {
			
			this->quantum = quantum;
			
		}
		
		void setTotalMemory(int totalMemory) {
			
			this->totalMemory = totalMemory;
			
		}
		
		//******************************************************************************
		//* Metodos (Getters)													        
		//******************************************************************************

		//Retorna o algoritmo que está sendo utilizado
		SchedulingAlgorithm getSchedulingAlgorithm() {

			return schedulingAlgorithm;

		}

		//Retorna o nome do algoritmo que está sendo utilizado
		string getAlgorithmName() {

			return sAlgorithmName;

		}

		//Retorna true se não houver nada na readyList, e false, caso contrário
		bool isEmpty() {

			return readyList.isEmpty();

		}

		//Retorna o processo em execução, na CPU
		Process getOnCPU() {

			return onCPU;

        }

		//Retorna true se o algoritmo utilizado é preemptivo, caso não, retorna false
		bool isPreemptive() {

			return preemptive;
			
		}

		//Retorna a o tamanho total da memória disponível no sistema
		int getTotalMemory() {

			return totalMemory;

		}

		//Retorna a o tamanho da memória livre no sistema
		int getFreeMemory() {

			return freeMemory;

		}

		//Retorna a o tamanho da memória que está sendo consumida no sistema
		int getUsedMemory() {

			return usedMemory;

		}

		//Retorna a quantidade de processos na fila de pronto
		int getSize() {

			return size;

		}

		//Retorna a quantidade de processos na fila de espera
		int getBlocked() {

			return blocked;

		}

		bool CPUInUse() {
		
			return onCPU.getPID() > 0 && onCPU.getPID() < lastID;
		
		}
		
		int getQuantum() {
			
			return quantum;
		
		}

		//******************************************************************************
		//* Métodos																        
		//******************************************************************************

		//Adiciona um novo processo na lista de pronto
		bool fork(string name, int time, int memory, Priority priority = LOW) {

			
			if (memory <= totalMemory) {
				Process *aux = new Process(name, lastID, time, memory, priority);
				
				readyList.insertProcess(*aux);
				
				delete aux;
				
				lastID++;
				size++;
				
				scheduling();
				
				return true;
			} else 
				return false;

		}

		//Adiciona um novo processo na lista de prontos (
		void fork(Process process) {

			readyList.insertProcess(process);

			lastID++;
			size++;

			scheduling();


		}

		//Mata o processo que está na CPU e retorna true, se conseguiu
		bool kill() {

			if (CPUInUse()) {
				putProcessOnCPU();
				return true;
			} else
				return false;

		}

		//Coloca o primeiro processo da lista na CPU
		void putProcessOnCPU() {

			if (readyList.isEmpty()) {

				Process aux;
				onCPU = aux;

			} else {

				onCPU = readyList.getFirstProcess();
				size--;
				
			}

		}

		//Bloquea o processo em execução, colocando em sleepList
		bool sleep() {

			if (CPUInUse()) {
				sleepList.insertProcess(onCPU);
				blocked++;
				putProcessOnCPU();
				return true;
			} else
				return false;

		}

		// Acorda um processo colocando-o de volta na lista de prontos
		bool wakeUp(string processName) {
			
			int i;
			bool exist = false;
			
			for (i = 0; i < blocked; i++) 
				if (this->index(i).getName() == processName) {
					exist = true;
					break;
				}
			
			if (exist) {
				fork(index(i));
				sleepList.removeProcess(i);
				blocked--;
				return true;
			} else 
				return false;

		}

		//Inicia o escalonamento dos processos || faz a troca de processos
		void start() {

			putProcessOnCPU();

		}


		//Atualiza todos os valores do escalonador (por segundo)
		void refresh() {

			if (CPUInUse()) {

				// Diminui o tempo restante do processo e incrementa o tempo dele na CPU
				onCPU.decreaseTime();

				// Se o processo terminou de executar
				if (onCPU.getTime() == 0)
				// Entra outro processo na CPU
					putProcessOnCPU();
				// Se não terminou, verifica se o algoritmo é preemptivo
				else if (preemptive)
				// Se for, verifica o quantum
						if (onCPU.getTimeCPU() == quantum)
				// Se o processo atingiu o quantum, atualiza a lista
							preemptiveUpdate();

			} else if (readyList.getSize() > 0)
			  putProcessOnCPU();

		}

		// ******************************************************************************
		//* Algoritmos de Escalonamento													
		//******************************************************************************

		//Controla todos os algoritmos de escalonamento
		void scheduling() {

			switch (schedulingAlgorithm) {
				case FIFO:
					sortNPreemptive(FIFO);
					break;
				case SJF:
					sortNPreemptive(SJF);
					break;
				case ROUND_ROBIN:
					sortNPreemptive(FIFO);
					break;
				case SRT:
					sortNPreemptive(SJF);
					break;
			}
		
		}

		//Faz a ordenação dos não-preemptivos
		void sortNPreemptive(SchedulingAlgorithm sAlgo) {

			switch(sAlgo) {
				case FIFO:
					sort(readyList.lista.begin(), readyList.lista.end(), compFifo);
					break;
				case SJF:
					sort(readyList.lista.begin(), readyList.lista.end(), compSjf);
					break;
			}

		}

		//Faz a modificação necessária de algoritmos preemptivos
		void preemptiveUpdate() {

			onCPU.restartTimeCPU();

			switch (schedulingAlgorithm) {
				case ROUND_ROBIN:
					roundRobin();
					break;
				case SRT:
					srt();
					break;

			}

		}

		// Pega o processo em execução, coloca no fim da lista de pronto e coloca o primeiro da lista de pronto
		// em execução
		void roundRobin() {

			readyList.insertProcess(onCPU);
			size++;
			putProcessOnCPU();

		}
		
		void srt() {
			
			fork(onCPU);
			sortNPreemptive(SJF);
			putProcessOnCPU();
			
		}

	};

}

#endif
