#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;


long go_into(Json::Value val, long total, const char* ignore) {
	long subtotal = 0L;
	switch (val.type()) {
		case Json::ValueType::intValue:
			total += val.asInt();
			break;

		case Json::ValueType::uintValue:
			total += val.asUInt();
			break;

		case Json::ValueType::arrayValue:
			for (int i = 0; i < val.size(); i++) {
				total = go_into(val[i], total, ignore);
			}
			break;

		case Json::ValueType::objectValue:
			for (string key : val.getMemberNames()) {
				Json::Value thisvalue = val[key];
				if (ignore == nullptr || !thisvalue.isString() || thisvalue.asString().compare(ignore) != 0) {
					subtotal = go_into(val[key], subtotal, ignore);
				}
				else {
					subtotal = 0;
					break;
				}
			}
			total += subtotal;
			break;

		case Json::ValueType::stringValue:
			// Do nothing
			break;

		default:
			throw runtime_error("unexpected type" + val.type());
	}

	return total;
}

int main() {
	ifstream file("input.txt");

	string line;
	long total = 0L;
	while (getline(file, line)) {
		const char* cstr = line.c_str();
		char* str_end;
		while (*cstr != '\0') {
			long val = strtol(cstr, &str_end, 10);
			if (val != 0) {
				total += val;
				cstr = str_end;
			}
			else {
				cstr++;
			}
		}
	}

	cout << total << endl;
	file.close();


	ifstream file2("input.txt");

	Json::Value json;
	file2 >> json;

	cout << go_into(json, 0, nullptr) << endl;
	cout << go_into(json, 0, "red") << endl;
}
