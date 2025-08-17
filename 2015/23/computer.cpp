#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;


const string FILENAME = "input.txt";


enum command {
	HLF,
	TPL,
	INC,
	JMP,
	JIE,
	JIO
};

enum register_name {
	A,
	B
};

struct instruction {
	enum command command;
	int offset_arg;
	enum register_name register_arg;
};


struct computer {
	long a = 0L;
	long b = 0L;
};

const map<string, enum command> COMMAND_MAP = {
	{"HLF", HLF},
	{"TPL", TPL},
	{"INC", INC},
	{"JMP", JMP},
	{"JIE", JIE},
	{"JIO", JIO},
	{"hlf", HLF},
	{"tpl", TPL},
	{"inc", INC},
	{"jmp", JMP},
	{"jie", JIE},
	{"jio", JIO}
};

const map<string, enum register_name> REGISTER_MAP = {
	{"A", A},
	{"A,", A},
	{"B", B},
	{"B,", B},
	{"a", A},
	{"a,", A},
	{"b", B},
	{"b,", B},
};


vector<struct instruction> read_program(string filename) {
	vector<struct instruction> program;

	string line;
	string command;
	string register_arg;
	int offset_arg;

	ifstream file(filename);

	while (!file.eof()) {
		struct instruction instruction;
		file >> command;
		instruction.command = COMMAND_MAP.at(command);
		if (instruction.command != JMP) {
			file >> register_arg;
			instruction.register_arg = REGISTER_MAP.at(register_arg);
		}
		switch(instruction.command) {
			case JMP:
			case JIE:
			case JIO:
				file >> offset_arg;
				instruction.offset_arg = offset_arg;
		}
		program.push_back(instruction);
		file >> ws;
	}

	file.close();
	return program;
}

void run_program(const vector<struct instruction>& program, int start_value_a, int start_value_b) {
	struct computer computer = {start_value_a, start_value_b};

	long program_counter = 0;
	while (program_counter >= 0 && program_counter < program.size()) {
		struct instruction instr = program.at(program_counter);
		long& register_val = instr.register_arg == A ? computer.a : computer.b;
		switch (instr.command) {
			case HLF:
				register_val /= 2;
				break;

			case TPL:
				register_val *= 3;
				break;

			case INC:
				register_val++;
				break;

			case JMP:
				program_counter += instr.offset_arg - 1;
				break;

			case JIE:
				if (!(register_val & 1)) {
					program_counter += instr.offset_arg - 1;
				}
				break;

			case JIO:
				if (register_val == 1) {
					program_counter += instr.offset_arg - 1;
				}
				break;
		}
		program_counter++;
	}
	cout << "A: " << computer.a << "; B: " << computer.b << endl;
}

int main() {
	const vector<struct instruction> program = read_program(FILENAME);
	run_program(program, 0, 0);
	run_program(program, 1, 0);
}
