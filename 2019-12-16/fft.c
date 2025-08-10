#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

unsigned long str_to_buf(int* buf, char string[]) {
    unsigned long i = 0;
    while (1) {
        char c = string[i];
        if (c == EOF || c == '\n') {
            break;
        }
        assert(c >= '0' && c <= '9');
        buf[i] = c - '0';
        i++;
    }
    return i;
}

void print_buffer(int* buf, unsigned long bufsize, unsigned long start) {
    for (unsigned long i = 0; i < bufsize; i++) {
        printf("%d", buf[start + i]);
    }
    puts("");
}

long gcd(long a, long b) {
    return (b == 0) ? a : gcd(b, a % b);
}

long lcm(long a, long b) {
    return (a * b) / gcd(a, b);
}

void process_method1(int* input_buffer, int* output_buffer, unsigned long bufsize,
                     int repeat_cycle, int offset, int num_phases) {
    int base_pattern[] = {0, 1, 0, -1};
    for (int phase = 0; phase <= num_phases; phase++) {
        for (unsigned long digit = offset; digit < bufsize; digit++) {
            long sum = 0L;
            long sum_rest = 0L;
            long cycle = lcm(repeat_cycle, (digit + 1) * sizeof(base_pattern) / sizeof(int));
            long digits_to_evaluate = MIN(cycle, bufsize);
            long rest_size = digits_to_evaluate % cycle;
            // We can start evaluating from digit <offset>, the ones to the left will be multiplied
            // by zero anyway
            for (unsigned long i = offset; i < digits_to_evaluate; i++) {
                if (i == rest_size) {
                    sum_rest = sum;
                }
                int base_pattern_index = ((i + 1) / (digit + 1)) % (sizeof(base_pattern) / sizeof(int));
                sum += input_buffer[i] * base_pattern[base_pattern_index];
            }
            sum *= bufsize / cycle + 1;
            sum += sum_rest;
            output_buffer[digit] = (int) ((sum >= 0 ? sum : -sum) % 10L);
        }
        int* swap = input_buffer;
        input_buffer = output_buffer;
        output_buffer = swap;
    }
}

void process_method2(int* input_buffer, int* output_buffer, unsigned long bufsize,
                     int repeat_cycle, int offset, int num_phases) {
    for (int phase = 0; phase <= num_phases; phase++) {
        long sum = 0;
        for (unsigned long digit = bufsize - 1; digit >= offset; digit--) {
            sum += input_buffer[digit];
            output_buffer[digit] = (int) ((sum >= 0 ? sum : -sum) % 10L);
        }
        int* swap = input_buffer;
        input_buffer = output_buffer;
        output_buffer = swap;
    }
}
                     
void process(int* input_buffer, int* output_buffer, unsigned long bufsize, int repeat_cycle,
             int offset, int num_phases) {
    if (offset < bufsize / 2) {
        process_method1(input_buffer, output_buffer, bufsize, repeat_cycle, offset, num_phases);
    } else {
        process_method2(input_buffer, output_buffer, bufsize, repeat_cycle, offset, num_phases);
    }
}

int main() {
    FILE *fp;
    fp = fopen("input.txt", "r");
    char str[10000];
    fgets(str, 10000, fp);
    fclose(fp);

    // char str[] = "12345678";

    int buffer1[10000];
    unsigned long bufsize = str_to_buf(buffer1, str);
    int buffer2[bufsize];
    int* input_buffer = buffer1;
    int* output_buffer = buffer2;

    int orig_input[bufsize];
    memcpy(orig_input, input_buffer, bufsize * sizeof(int));

    process(input_buffer, output_buffer, bufsize, bufsize, 0, 100);
    print_buffer(input_buffer, 8, 0);

    // Part 2

    long offset = 0;
    for (int i = 0; i < 7; i++) {
        offset = offset * 10 + orig_input[i];
    }

    int* long_input = malloc(bufsize * 10000 * sizeof(int));
    for (unsigned long i = 0; i < 10000; i++) {
        memcpy(long_input + i * bufsize, orig_input, bufsize * sizeof(int));
    }
    int* long_buffer2 = malloc(bufsize * 10000 * sizeof(int));

    process(long_input, long_buffer2, bufsize * 10000, bufsize, offset, 100);
    print_buffer(long_input, 8, (unsigned long) offset);
}
