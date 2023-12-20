#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;


#define MAXIMIZE(LHS, RHS) (LHS = ((LHS) > (RHS)) ? (LHS) : (RHS))
#define MINIMIZE(LHS, RHS) (LHS = ((LHS) < (RHS)) ? (LHS) : (RHS))

struct Condition {
	char category;
	bool greater_than;
	int value;
	string target;
};

struct Range {
	int min;
	int max;
};

bool start_workflow(const map<string, vector<Condition>>& workflows, const string& workflow_name,
                    const map<char, int>& part) {

	if (workflow_name == "A") {
		return true;
	} else if (workflow_name == "R") {
		return false;
	}

	const vector<Condition>& workflow = workflows.at(workflow_name);
	while (true) {
		for (const Condition& cond : workflow) {
			if (cond.value < 0) {
				return start_workflow(workflows, cond.target, part);
			} else {
				if (cond.greater_than) {
					if (part.at(cond.category) > cond.value) {
						return start_workflow(workflows, cond.target, part);
					}
				} else if (part.at(cond.category) < cond.value) {
					return start_workflow(workflows, cond.target, part);
				}
			}
		}
	}
	throw;
}


long analyze_workflow(const map<string, vector<Condition>>& workflows, string workflow_name,
                       map<char, Range> ranges, long combinations) {

	
	if (workflow_name == "A") {
		long comb = 1L;
		for (const pair<char, Range>& range : ranges) {
			comb *= range.second.max - range.second.min + 1;
		}
		return combinations + comb;
	}
	if (workflow_name == "R") {
		return combinations;
	}

	for (const Condition& cond : workflows.at(workflow_name)) {
		if (cond.value < 0) {
			return analyze_workflow(workflows, cond.target, ranges, combinations);
		} else {
			map<char, Range> ranges2(ranges);
			if (cond.greater_than) {
				MAXIMIZE(ranges2.at(cond.category).min, cond.value + 1);
				MINIMIZE(ranges.at(cond.category).max, cond.value);
			} else {
				MINIMIZE(ranges2.at(cond.category).max, cond.value - 1);
				MAXIMIZE(ranges.at(cond.category).min, cond.value);
			}
			combinations = analyze_workflow(workflows, cond.target, ranges2, combinations);
		}
	}
	throw;
}

int main() {

	map<string, vector<Condition>> workflows;

	ifstream file("input.txt");
	string line;
	while (getline(file, line) && line != "") {
		vector<Condition> workflow;
		string name;
		size_t pos = line.find('{');
		name = line.substr(0, pos);
		while (pos != string::npos) {
			Condition cond;
			cond.category = line.substr(pos + 1, 1)[0];
			string comparison_char = line.substr(pos + 2, 1);
			if (comparison_char != ">" && comparison_char != "<") {
				cond.value = -1;
				cond.target = line.substr(pos + 1, line.find('}', pos + 1) - pos - 1);
				pos = string::npos;
			} else {
				cond.greater_than = line.substr(pos + 2, 1) == ">";
				cond.value = stoi(line.substr(pos + 3));
				size_t pos2 = line.find(':', pos + 1);
				pos = line.find(',', pos + 1);
				cond.target = line.substr(pos2 + 1, pos - pos2 - 1);
			}
			workflow.push_back(cond);
		}
		workflows.insert(make_pair(name, workflow));
	}

	vector<map<char, int>> parts;
	while (getline(file, line)) {
		size_t pos = 0;
		map<char, int> part;
		while (pos != string::npos) {
			part.insert(make_pair(line.substr(pos + 1, 1)[0], stoi(line.substr(pos + 3))));
			pos = line.find(',', pos + 1);
		}
		parts.push_back(part);
	}
	file.close();

	int sum = 0;
	for (map<char, int> part : parts) {
		if (start_workflow(workflows, "in", part)) {
			sum += part.at('x') + part.at('m') + part.at('a') + part.at('s');
		}
	}
	cout << sum << endl;

	map<char, Range> ranges = {
		{'x', {1, 4000}},
		{'m', {1, 4000}},
		{'a', {1, 4000}},
		{'s', {1, 4000}}
	};
	long combinations = analyze_workflow(workflows, "in", ranges, 0L);
	cout << combinations << endl;
}
