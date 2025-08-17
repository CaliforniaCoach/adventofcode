#include <stdio.h>
#include <string.h>
#include <assert.h>


#define MAX_LINES 10000
#define MAX_LINE_LENGTH 100

int val(char digit) {
    if (digit >= '0' && digit <= '2')
        return digit - '0';
    else if (digit == '-')
        return -1;
    else if (digit == '=')
        return -2;
    else
        assert(0);
}

char digit(int val) {
    if (val >= 0 && val <= 2)
        return '0' + val;
    else if (val == -1)
        return '-';
    else
        return '=';
}

void trim(char string[]) {
    int i = strlen(string) - 1;
    while (string[i] == '\n')
        string[i--] = '\0';
}

int main(int argc, char* argv[]) {
    int line = 0;
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    FILE* f = fopen(argv[1], "r");
    while(fgets(lines[line], MAX_LINE_LENGTH, f)) {
        trim(lines[line]);
        line++;
    }
    fclose(f);

    char number[MAX_LINE_LENGTH + 1];
    for (int i = 0; i < MAX_LINE_LENGTH; i++)
        number[i] = '0';
    number[MAX_LINE_LENGTH] = '\0';

    for (int i = 0; i < line; i++) {
        int place = 0;
        int carryover = 0;
        int len = strlen(lines[i]);
        while (place <= len || carryover != 0) {
            char digit1 = number[MAX_LINE_LENGTH - 1 - place];
            char digit2 = '0';
            if (len - place - 1 >= 0)
                digit2 = lines[i][len - place - 1];
            int sum = val(digit1) + val(digit2) + carryover;
            if (sum > 2) {
                carryover = 1;
                sum -= 5;
            }
            else if (sum < -2) {
                carryover = -1;
                sum += 5;
            }
            else {
                carryover = 0;
            }
            number[MAX_LINE_LENGTH - 1 - place] = digit(sum);
            place++;
        }
    }

    puts(number);
}
