#include <iostream>
#include <fstream>
#include <vector>
#include <map>

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
};


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

	for (pair<string, Module*> entry : modules) {
		Module& this_mod = *entry.second;
		for (string output : this_mod.outputs) {
			map<string, Module*>::iterator it = modules.find(output);
			if (it != modules.end()) {
				it->second->register_input(entry.first);
			}
		}
	}

	vector<long> signals_negative(1, 0);
	vector<long> signals_positive(1, 0);
	long sig_neg = 0;
	long sig_pos = 0;
	bool button_taps_finished = false, rx_finished = false;
	for (long i = 0; !button_taps_finished || !rx_finished; i++) {
		sig_neg++;
		vector<Signal> signals;
		Module& broadcaster = *modules.at("broadcaster");
		for (string output : broadcaster.outputs) {
			signals.push_back({"broadcaster", output, false});
		}
		int sig_neg_rx = 0;
		int sig_pos_rx = 0;
		while (!signals.empty()) {
			vector<Signal> out_signals_all;
			for (Signal signal : signals) {
				if (signal.signal)
					sig_pos++;
				else
					sig_neg++;
				if (signal.to == "rx") {
					if (signal.signal)
						sig_pos_rx++;
					else
						sig_neg_rx++;
				}
				map<string, Module*>::iterator it = modules.find(signal.to);
				if (it != modules.end()) {
				vector<Signal> out_signals = it->second->process_signal(signal);
				out_signals_all.insert(out_signals_all.end(),
				                       out_signals.begin(), out_signals.end());
				}
			}
			signals = out_signals_all;
		}

		// cout << "rx received " << sig_neg_rx << " negative and " << sig_pos_rx << " positive pulses." << endl;

		if (sig_neg_rx == 1 && sig_pos_rx == 0) {
			rx_finished = true;
			cout << "rx triggered after " << i + 1 << " presses of button." << endl;
		}

		// Verify if everything is in its initial state again
		bool everything_initial = true;
		for (const pair<string, Module*>& entry : modules) {
			if (!entry.second->is_in_initial_state()) {
				everything_initial = false;
				break;
			}
		}
		if (!button_taps_finished && i == NUM_BUTTON_TAPS - 1) {
			button_taps_finished = true;
			cout << "Positive: " << sig_pos << ", negative: " << sig_neg << endl;
			cout << "Multiplied: " << sig_pos * sig_neg << endl;
		}
		if (everything_initial) {
			button_taps_finished = true;
			int cycle = i + 1;
			cout << "Cycle (everything initial after): " << cycle << endl;
			int sn = sig_neg * (NUM_BUTTON_TAPS / cycle) + signals_negative[NUM_BUTTON_TAPS % cycle];
			int sp = sig_pos * (NUM_BUTTON_TAPS / cycle) + signals_positive[NUM_BUTTON_TAPS % cycle];
			cout << "Positive: " << sp << ", negative: " << sn << endl;
			cout << "Multiplied: " << sp * sn << endl;
		}
		signals_positive.push_back(sig_pos);
		signals_negative.push_back(sig_neg);
	}
}
