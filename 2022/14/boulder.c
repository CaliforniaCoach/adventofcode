#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

const int EMPTY = 0;
const int ROCK = 1;
const int SAND = 2;
const int SANDSOURCE = 3;

const char SYMBOL[] = {'.', '#', 'o', '+'};

typedef struct {
    int min_x, max_x, min_y, max_y;
} Boundaries;

Boundaries get_boundaries(int map[][1000]) {
    Boundaries b;
    b.min_x = 1000;
    b.min_y = 1000;
    b.max_x = 0;
    b.max_y = 0;
    for (int y = 0; y < 1000; y++) {
        for (int x = 0; x < 1000; x++) {
            if (map[y][x] != EMPTY) {
                b.min_x = MIN(b.min_x, x);
                b.min_y = MIN(b.min_y, y);
                b.max_x = MAX(b.max_x, x);
                b.max_y = MAX(b.max_y, y);
            }
        }
    }
    return b;
}

void print_field(int map[][1000], Boundaries b) {
    for (int y = MAX(b.min_y - 1, 0); y <= MIN(b.max_y + 1, 1000 - 1); y++) {
        for (int x = MAX(b.min_x - 1, 0); x <= MIN(b.max_x + 1, 1000 -1); x++) {
            printf("%c", SYMBOL[map[y][x]]);
            // printf("%d", map[y][x]);
        }
        puts("");
    }
    puts("");
}

int simulate_sand_dripping1(int map[][1000], Boundaries boundaries) {
    int num_sand = 0;
    int sand_dripping_into_abyss = 0;
    while (!sand_dripping_into_abyss) {
        int x = 500;
        int y = 0;
        while (1) {
            if (y > boundaries.max_y) {
                sand_dripping_into_abyss = 1;
                break;
            }
            else if (map[y + 1][x] == EMPTY) {
                y++;
            }
            else if (map[y + 1][x - 1] == EMPTY) {
                y++;
                x--;
            }
            else if (map[y + 1][x + 1] == EMPTY) {
                y++;
                x++;
            }
            else {
                map[y][x] = SAND;
                num_sand++;
                break;
            }
        }
    }
    return num_sand;
}

int simulate_sand_dripping2(int map[][1000], Boundaries boundaries) {
    int num_sand = 0;
    do {
        int x = 500;
        int y = 0;
        while (1) {
            if (map[y + 1][x] == EMPTY && y < boundaries.max_y) {
                y++;
            }
            else if (map[y + 1][x - 1] == EMPTY && y < boundaries.max_y) {
                y++;
                x--;
            }
            else if (map[y + 1][x + 1] == EMPTY && y < boundaries.max_y) {
                y++;
                x++;
            }
            else {
                map[y][x] = SAND;
                num_sand++;
                break;
            }
        }
    } while (map[0][500] != SAND);
    return num_sand;
}

Boundaries read_map(char filename[], int map[][1000]) {
    char line[1000];
    FILE* file = fopen(filename, "r");
    int from_x = -1, from_y = -1, to_x, to_y;
    while (!feof(file)) {
        if (fscanf(file, "%d,%d", &to_x, &to_y) == 2) {
            if (from_x >= 0 && from_y >= 0) {
                if (from_x == to_x) {
                    for (int y = MIN(from_y, to_y); y <= MAX(from_y, to_y); y++) {
                        map[y][from_x] = ROCK;
                    }
                }
                else {
                    for (int x = MIN(from_x, to_x); x <= MAX(from_x, to_x); x++) {
                        map[from_y][x] = ROCK;
                    }
                }
            }
            from_x = to_x;
            from_y = to_y;
        }
        char ch = fgetc(file);
        if (ch == '\n') {
            from_x = -1;
            to_x = -1;
        }
        else if (ch == ' ') {
            fread(line, strlen("-> "), sizeof(char), file);
        }
    }
    fclose(file);
    map[0][500] = SANDSOURCE;
    return get_boundaries(map);
}

void clear_map_from_sand(int map[][1000]) {
    for (int y = 0; y < 1000; y++)
        for (int x = 0; x < 1000; x++)
            if (map[y][x] == SAND)
                map[y][x] = EMPTY;
}

int main(int argc, char* argv[]) {

    int map[1000][1000];
    for (int y = 0; y < 1000; y++)
        for (int x = 0; x < 1000; x++)
            map[y][x] = EMPTY;

    Boundaries boundaries = read_map("input.txt", map);
    print_field(map, boundaries);

    int num_sand1 = simulate_sand_dripping1(map, boundaries);
    print_field(map, boundaries);

    clear_map_from_sand(map);
    boundaries.max_y++;

    int num_sand2 = simulate_sand_dripping2(map, boundaries);
    boundaries = get_boundaries(map);
    boundaries.min_x -= 2;
    boundaries.max_x += 2;
    boundaries.max_y++;
    for (int x = boundaries.min_x; x <= boundaries.max_x; x++)
        map[boundaries.max_y][x] = ROCK;
    print_field(map, boundaries);

    printf("\nNumber of sand fields part 1: %d\n", num_sand1);
    printf("\nNumber of sand fields part 2: %d\n", num_sand2);
}
