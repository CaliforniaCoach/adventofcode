#include <iostream>
#include <cmath>

using namespace std;


const long MIN = 29000000;

int main() {

    // max_presents = 10 * (house / 2) * (house + 1) = 5 * house^2 + 5 * house
    // house = -b +/- sqrt(b^2 - 4 * a * c) / 2 * a; a = 5, b = 5, c = -max_presents
    // house = -5 + sqrt(25 + 20 * max_presents) / 10

    long min_house = sqrt(25 + 20 * MIN) / 10 - 5;

    // Part 1
    for (long house = min_house; true; house++) {
        long presents = 0;
        for (long elve = 1; elve <= house; elve++) {
            if (house % elve == 0) {
                presents += elve * 10;
            }
        }
        if (presents >= MIN) {
            cout << "House " << house << " got " << presents << " presents." << endl;
            break;
        }
    }

    // Part 2
    for (long house = min_house; true; house++) {
        long presents = 0;
        for (long elve = (house - 1) / 50 + 1; elve <= house / 2 + 1; elve++) {
            if (house % elve == 0) {
                presents += elve * 11;
            }
        }
        presents += house * 11;
        if (presents >= MIN) {
            cout << "House " << house << " got " << presents << " presents." << endl;
            break;
        }
    }

    return 0;
}
