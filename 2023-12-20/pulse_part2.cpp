#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>

using namespace std;


#define NUM_BUTTON_TAPS 1000

struct Signal {
	string from;
	string to;
	bool signal;
};

class Module {
	public:
		vector<string> outputs;

		virtual void register_input(string input) {}

		virtual vector<Signal> process_signal(Signal signal) {
			return vector<Signal>();
		};

		virtual bool is_in_initial_state() {
			return true;
		}

		virtual long long add_state(long long initial_state) {}

		virtual bool is_conjunction() {
			return false;
		}
};

class Flipflop : public Module {
	public:
		bool state = false;

		vector<Signal> process_signal(Signal signal) {
			vector<Signal> output_signals;
			if (!signal.signal) {
				state = !state;
				for (string output : outputs) {
					output_signals.push_back({signal.to, output, state});
				}
			}
			return output_signals;
		}

		bool is_in_initial_state() {
			return !state;
		}

		long long add_state(long long input_state) {
			input_state <<= 1;
			if (input_state < 0) {
				throw;
			}
			return input_state | (state ? 0b1 : 0b0);
		}
};

class Conjunction : public Module {
	public:
		map<string, bool> inputs;

		void register_input(string input) {
			inputs.insert(make_pair(input, false));
		}

		vector<Signal> process_signal(Signal signal) {
			inputs.at(signal.from) = signal.signal;
			bool output_signal = false;
			for (pair<string, bool> entry : inputs) {
				if (!entry.second) {
					output_signal = true;
					break;
				}
			}
			vector<Signal> output_signals;
			for (string output : outputs) {
				output_signals.push_back({signal.to, output, output_signal});
			}
			return output_signals;
		}

		bool is_in_initial_state() {
			bool retval = true;
			for (pair<string, bool> entry : inputs) {
				if (entry.second) {
					retval = false;
					break;
				}
			}
			return retval;
		}

		long long add_state(long long input_state) {
			for (pair<string, bool> input : inputs) {
				input_state <<= 1;
				if (input_state < 0) {
					throw;
				}
				input_state |= input.second ? 0b1 : 0b0;
			}
			return input_state;
		}

		virtual bool is_conjunction() {
			return true;
		}
};


struct SignalPath {
	string start_module;
	string last_module = "";
	map<string, Module*> modules;

	SignalPath(const map<string, Module*>& all_modules, string path_start) {
		start_module = path_start;
		follow_path(all_modules, path_start);

		// Ensure there is exactly one NAND gate before "rx" and get its inputs
	}

	void follow_path(const map<string, Module*>& all_modules, string module_name) {
		if (modules.find(module_name) == modules.end()) {
			map<string, Module*>::const_iterator it = all_modules.find(module_name);
			if (it != all_modules.end()) {
				Module* module = it->second;
				modules[module_name] = module;
				for (const string& output : module->outputs) {
					if (output == "rx") {
						if (last_module != "") {
							throw;
						} else {
							last_module = module_name;
						}
					}
					follow_path(all_modules, output);
				}
			}
		}
	}
};


struct SignalPathData {
	long offset;
	long cycle_length;
	vector<long> pos_out_signals_at;
};


SignalPathData process_signals(const SignalPath& signal_path) {
	SignalPathData sig_path_data;
	map<long long, long> states;
	for (long i = 0; true; i++) {
		vector<Signal> signals;
		signals.push_back({"", signal_path.start_module, false});
		// Module& start_module = *modules.at(start_module_name);
		// for (string output : start_module.outputs) {
		// 	signals.push_back({start_module_name, output, false});
		// }
		int sig_neg_last = 0;
		int sig_pos_last = 0;
		while (!signals.empty()) {
			vector<Signal> out_signals_all;
			for (Signal signal : signals) {
				if (signal.to == signal_path.last_module) {
					if (signal.signal)
						sig_pos_last++;
					else
						sig_neg_last++;
				}
				map<string, Module*>::const_iterator it = signal_path.modules.find(signal.to);
				if (it != signal_path.modules.end()) {
				vector<Signal> out_signals = it->second->process_signal(signal);
				out_signals_all.insert(out_signals_all.end(),
				                       out_signals.begin(), out_signals.end());
				}
			}
			signals = out_signals_all;
		}

		// cout << "rx received " << sig_neg_rx << " negative and " << sig_pos_rx << " positive pulses." << endl;

		if (sig_pos_last > 0) {
			cout << "rx triggered after " << i + 1 << " presses of button." << endl;
			cout << "       pos signals received: " << sig_pos_last << endl;
			cout << "       neg signals received: " << sig_neg_last << endl;
		}

		// Save status
		long long state = 0b1;
		for (const pair<string, Module*> entry : signal_path.modules) {
			state = entry.second->add_state(state);
		}
		map<long long, long>::iterator it = states.find(state);
		if (it == states.end()) {
			states[state] = i;
		} else {
			sig_path_data.cycle_length = i - it->second;
			sig_path_data.offset = it->second;
			cout << "Recurring state: " << state << " after " << i << " button presses, ";
			cout << "cycle: " << sig_path_data.cycle_length << " iterations" << endl << endl;
			break;
		}
	}
	return sig_path_data;
}


int main() {
	map<string, Module*> modules;
	ifstream file("input.txt");
	string name;
	string arrow;
	while (file >> name >> arrow) {
		Module* mod;
		if (name.front() == '%') {
			mod = new Flipflop();
			name = name.substr(1);
		} else if (name.front() == '&') {
			mod = new Conjunction();
			name = name.substr(1);
		} else {
			mod = new Module();
		}

		string output;
		bool last = false;
		while (!last) {
			file >> output;
			if (output.back() == ',') {
				output = output.substr(0, output.length() - 1);
			} else {
				last = true;
			}
			mod->outputs.push_back(output);
		}
		modules.insert(make_pair(name, mod));
	}
	file.close();

	// Store all inputs of Conjunctions
	for (pair<string, Module*> entry : modules) {
		Module& this_mod = *entry.second;
		for (string output : this_mod.outputs) {
			map<string, Module*>::iterator it = modules.find(output);
			if (it != modules.end()) {
				it->second->register_input(entry.first);
			}
		}
	}

	// Determine independent signal paths starting from "broadcaster"
	vector<SignalPath> signal_paths;
	Module& broadcaster = *modules.at("broadcaster");
	for (string path_start : broadcaster.outputs) {
		map<string, Module*> path;
		SignalPath signal_path(modules, path_start);
		signal_paths.push_back(signal_path);
	}

	// Ensure last module of all signal paths before "rx" is a conjunction
	string last_module = signal_paths[0].last_module;
	if (!modules.at(last_module)->is_conjunction()) {
		throw;
	}
	vector<SignalPathData> sig_path_data;
	long long overall_cycle = 1;
	for (const SignalPath& signal_path : signal_paths) {
		// Ensure all signal paths have the same module as last module
		if (signal_path.last_module != last_module) {
			throw;
		}
		SignalPathData sig_path_data = process_signals(signal_path);
		overall_cycle = lcm(overall_cycle, sig_path_data.cycle_length);
	}
	cout << "Overall cycle: " << overall_cycle << endl;
}
