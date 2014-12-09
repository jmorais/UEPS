
//Ambiente Unix: Define N_KBHIT
//Ambiente Windows: Define KBHIT
#define N_KBHIT
#define CMD_CLEAR "clear"

#include "Headers/Libraries.h"

using namespace ProcessScheduler;
using namespace std;
using std::string;

typedef vector<string> Strings;

Scheduler scheduler;

bool execute = true;

/*************************************************************/
/* Internal functions - declaration													 */
/*************************************************************/
void delay();
void interpreter(string line);
bool isInt(string number);
int toInt(string number);
void readLine();
int kbhit(void);
bool keystroke();
Strings split(string line);

/*************************************************************/
/* Commands functions - declaration													 */
/*************************************************************/
void block(Strings strings);
void clear();
void exit();
void help(Strings strings);
void insert(Strings strings);
void options();
void pause(Strings strings);
void remove(Strings strings);
void setAlgorithm(Strings strings);
void show(Strings strings);
void start(Strings strings);
void wake(Strings strings);

/*************************************************************/
/* Internal functions - Implementation											 */
/*************************************************************/

void delay(int ms = 1000) {

 	usleep(ms * 1000);
     
}

void interpreter(string line) {
	
	Strings strings = split(line);
	
	for (int i = 0; i < strings[0].length(); i++)
		strings[0][i] = toupper(strings[0][i]); 
	
	if (strings[0] ==  "HELP") {
		help(strings);
	} else if (strings[0] == "INSERT") {
		insert(strings);
	} else if (strings[0] == "REMOVE") {
		remove(strings);	
	} else if (strings[0] == "SHOW") {
		show(strings);
	} else if (strings[0] == "BLOCK") {
		block(strings);
	} else if (strings[0] == "WAKE") {
		wake(strings);
	} else if (strings[0] == "SETALGORITHM") {
		setAlgorithm(strings);
	} else if (strings[0] == "START") {
		start(strings);
	} else if (strings[0] == "PAUSE") {
		
	} else if (strings[0] == "EXIT") {
		exit();
	} else if (strings[0] == "CLEAR") {
		clear();
	} else if (strings[0] == "OPTIONS") {
		options();
	} else {
		cout << "ERROR > " << strings[0] << ": command not found" << endl;
		cout << '\n'; 
	}
	
}

bool isInt(string number) {
	
	for (int i = 0; i < number.length(); i++) {
		if (!isdigit(number[i])) {
			return false;
		}
	}
	
	return true;
	
}

int toInt(string number) {
	return atoi(number.c_str());	
}

void readLine() {
	
	string line;
	cout << "$> ";
	getline(cin, line);
	
	if (line.empty()) {
		return;
	}
	
	interpreter(line);
	
}

int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}


bool keystroke() {

	#ifndef KBHIT
		return kbhit();
	#else 
		return _kbhit();
	#endif

}

Strings split(string line) {
	
	string buffer;
	stringstream ss(line);
	
	Strings strings;
	
	while (ss >> buffer) {
		strings.push_back(buffer);
	}
	
	return strings;
	
}

/*************************************************************/
/* Command functions - Implementation												 */
/*************************************************************/

void block(Strings strings) {
	
	if (strings.size() > 2) {
		cout << "ERROR > BLOCK: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 2) {
		cout << "ERROR > BLOCK: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	cout << "Process " << strings[1] << " blocked successfully." << endl;
	cout << '\n';
	
}

void clear() {
	system(CMD_CLEAR);
}

void exit() {
	execute = false;
	cout << "Bye-Bye" << endl;
}

void help(Strings strings) {
	
	ifstream file;
	
	if (strings.size() == 1) 
		file.open("ManPages//general.txt");
	else 
		file.open(("ManPages//" + strings[1] + ".txt").c_str());

	string ou;
	
	if (file.is_open()) {
		
		while (!file.eof()) {
			getline(file, ou);
			cout << ou << endl;
		}
		
		cout << endl;
		file.close();
		
	} else {
		cout << "ERROR > HELP > Help file not found." << endl;
		cout << '\n';
	}
	
}

void insert(Strings strings) {
	
	if (strings.size() > 4) {
		cout << "ERROR > INSERT: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 4) {
		cout << "ERROR > INSERT: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	if (!isInt(strings[2]) || !isInt(strings[3])) {
		cout << "ERROR > INSERT: Invalid argument type." << endl;
		cout << '\n';
		return;		
	}
	
	stringstream a(strings[2]), b(strings[3]);
	int time, memory;
	
	a >> time;
	b >> memory;
	
	if (scheduler.fork(strings[1], time, memory)) {
		cout << "Process " << strings[1] << " created successfully." << endl;
		cout << '\n';		
	} else {
		cout << "ERROR > INSERT: No memory avaiable to process." << endl;
		cout << '\n';
		return;
	}

}

void options() {
	
	string memory, quantum;
	
	cout << "Memory Size: ";
	cin >> memory;
	
	if (!isInt(memory)) { 
		cout << "ERROR > OPTIONS: Integer value expected." << endl << endl;
		return;
	} 
	
	cout << "Quantum Value: ";
	cin >> quantum;
	
	if (!isInt(quantum)) {
		cout << "ERROR > OPTIONS: Integer value expected." << endl << endl;
		return;
	}
	
	scheduler.setQuantum(toInt(quantum));
	scheduler.setTotalMemory(toInt(memory));
	
	cout << '\n';
	getchar();
	
}

void remove(Strings strings) {
	
	if (strings.size() > 2) {
		cout << "ERROR > REMOVE: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 2) {
		cout << "ERROR > REMOVE: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	cout << "Process " << strings[1] << " removed successfully." << endl;
	cout << '\n';

}

void setAlgorithm(Strings strings) {
	
	if (strings.size() > 2) {
		cout << "ERROR > SETALGORITHM: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 2) {
		cout << "ERROR > SETALGORITHM: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	for (int i = 0; i < strings[1].length(); i++)
		strings[1][i] = toupper(strings[1][i]); 	
	
	if (!(strings[1] == "FIFO" || strings[1] == "SJF" || strings[1] == "SRT" || strings[1] == "RR")) {
		cout << "ERROR > SETALGORITHM: Invalid argument." << endl;
		cout << '\n';
		return;
	}
		
	if (strings[1] == "FIFO") {
		scheduler.setSchedulingAlgorithm(FIFO);
	} else if (strings[1] == "SJF") {
		scheduler.setSchedulingAlgorithm(SJF);
	} else if (strings[1] == "RR") {
		scheduler.setSchedulingAlgorithm(ROUND_ROBIN);
	} else {
		scheduler.setSchedulingAlgorithm(SRT);
	}
		
	cout << "Scheduler algorithm setted to: " << scheduler.getAlgorithmName() << endl;
	cout << '\n';
	
}

void show(Strings strings) {
	
	if (strings.size() > 2) {
		cout << "ERROR > SHOW: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 2) {
		cout << "ERROR > SHOW: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	for (int i = 0; i < strings[1].length(); i++)
		strings[1][i] = toupper(strings[1][i]); 	
		
	if (!(strings[1] == "READYLIST" || strings[1] == "SLEEPLIST")) {
		cout << "ERROR > SHOW: Invalid argument." << endl;
		cout << '\n';
		return;
	}
	
	cout << '\n';
	cout << "No. | PID | Process Name         | Time  | Memory" << endl;
	cout << "==================================================" << endl;
	
	if (strings[1] == "READYLIST") {
                   
       for (int i = 0; i < scheduler.getSize(); i++) {
           cout << setw(3)  << right << i + 1 << " |";
           cout << setw(4)  << right << scheduler[i].getPID()  << " |";
           cout << setw(21) << right << scheduler[i].getName() << " |";
           cout << setw(6)  << right << scheduler[i].getTime() << " |";
           cout << setw(7)  << right << scheduler[i].getMemory();
           cout << '\n';
           
       }           
          
    } else {
     
       for (int i = 0; i < scheduler.getBlocked(); i++) {
           cout << setw(3)  << right << i + 1 << " |";
           cout << setw(4)  << right << scheduler.index(i).getPID()  << " |";
           cout << setw(21) << right << scheduler.index(i).getName() << " |";
           cout << setw(6)  << right << scheduler.index(i).getTime() << " |";
           cout << setw(7)  << right << scheduler.index(i).getMemory();
           cout << '\n';
           
       }              
       
    }
    
	cout << "==================================================" << endl;
	cout << '\n';
	
}

void start(Strings strings) {
	
	if (strings.size() > 1) {
		cout << "ERROR > START: Too many arguments." << endl;
		cout << '\n';
		return;
	}		
	
	if (scheduler.getSize() == 0) {
		cout << "Nothing to schedule." << endl;
		cout << '\n';
		return;
	}
	
	bool execute = true;
	
	if (!scheduler.CPUInUse())
    	scheduler.start();
	
	cout << "Starting scheduling simulation..." << endl;
	cout << '\n';
	
	while (execute) {
		
		clear();
		
		if ((scheduler.getSize() == 0) && !(scheduler.CPUInUse()) && (scheduler.getBlocked() == 0)) {
			cout << "Nothing to schedule." << endl;
			cout << '\n';
			return;
		}
		
		cout << "[I]nsert / [B]lock / [U]nblock / [K]ill / [E]xit " << endl;
		cout << '\n';
		
		cout << "Scheduler's Algorithm: " << scheduler.getAlgorithmName() << endl;
		cout << "CPU's quantum: " << scheduler.getQuantum() << endl;
		cout << '\n';
		
		cout << "Process on CPU: " << scheduler.getOnCPU().getName() << endl;
		cout <<	"Time remaining: " << scheduler.getOnCPU().getTime() << endl;
		cout << '\n';
		
		cout << "No. | PID | Process Name         | Time  | Memory" << endl;
		cout << "==================================================" << endl;
			
		for (int i = 0; i < scheduler.getSize(); i++) {
			cout << setw(3)  << right << i + 1 << " |";
			cout << setw(4)  << right << scheduler[i].getPID()  << " |";
			cout << setw(21) << right << scheduler[i].getName() << " |";
			cout << setw(6)  << right << scheduler[i].getTime() << " |";
			cout << setw(7)  << right << scheduler[i].getMemory();
			cout << '\n';
			
		}           
		cout << "==================================================" << endl;
		cout << '\n';	
		
		if(keystroke()) {
           	char option;
            option = getchar();
       		
			switch (toupper(option)) {			
                   
			case 'B':
				scheduler.sleep();
				break;
				
			case 'I': {
                
                string strings;
                cout << "$> insert ";
                getline(cin, strings);
                strings = "insert " + strings;
                insert(split(strings));
				break;
            }
			case 'K':
                 
				scheduler.kill();
				break;
				
			case 'E': {
				clear();
				return;
            }

			case 'U': {
           		cout << "No. | PID | Process Name         | Time  | Memory" << endl;
             	cout << "==================================================" << endl;
                for (int i = 0; i < scheduler.getBlocked(); i++) {
					cout << setw(3)  << right << i + 1 << " |";
					cout << setw(4)  << right << scheduler.index(i).getPID()  << " |";
					cout << setw(21) << right << scheduler.index(i).getName() << " |";
					cout << setw(6)  << right << scheduler.index(i).getTime() << " |";
					cout << setw(7)  << right << scheduler.index(i).getMemory();
					cout << '\n';
                }             
                cout << "==================================================" << endl;
                cout << '\n';          
                string strings;
                cout << "$> wake ";
                getline(cin, strings);
                strings = "wake " + strings;                
                wake(split(strings));
                break;				
			}	
			
        }
             
		}	

		cout << '\n';
		
		delay(1000);	
		scheduler.refresh();
	}	
	
}

void wake(Strings strings) {
	
	if (strings.size() > 2) {
		cout << "ERROR > WAKE: Too many arguments." << endl;
		cout << '\n';
		return;
	} else if (strings.size() < 2) {
		cout << "ERROR > WAKE: Too few arguments." << endl;
		cout << '\n';
		return;
	}
	
	if (scheduler.wakeUp(strings[1])) 
       cout << "Process " << strings[1] << " unblocked successfully." << endl;
    else {
       cout << "ERROR > WAKE: Process not found." << endl;
       cout << '\n';
       return;
    }
	
    cout << '\n';

}


/*************************************************************/
/* Main														 													*/
/*************************************************************/

int main () {
	
	clear();
	
	cout << "Welcome to Ultra Express Process Scheduler" << endl;
	cout << "Type \'help\' to learn more about the Ultra Express Process Scheduler" << endl;
	cout << '\n';
	
	while (execute) {
		readLine();
	}

	return 0;

}
