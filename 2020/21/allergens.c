#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


#define FILENAME "input.txt"


char ingredient_name[1000][100];
char allergen_name[1000][100];
int ingredients[1000][500];
int allergens[1000][500];
int allergen_not_in_ingredients[1000][1000];
int ingredient_contains_allergen[1000];
int num_foods;

struct Allergen {int allergen_index; int ingredient_index;} sorted_allergen_list[1000];

int find_or_add(char* item, char list[][100]) {
	int i;
	for (i = 0; list[i][0] != '\0'; i++) {
		if (!strcmp(item, list[i])) {
			return i;
		}
	}
	strcpy(list[i], item);
	return i;
}

int get_item_index(int list[], int item) {
	for (int i = 0; list[i] >= 0; i++) {
		if (list[i] == item) {
			return i;
		}
	}
	return -1;
}

int get_exactly_one_common_item(int list1[], int list2[]) {
	int common_item = -1;
	for (int i = 0; list1[i] >= 0; i++) {
		int item = list1[i];
		int idx = get_item_index(list2, item);
		if (idx >= 0) {
			if (common_item >= 0) {
				return -1;
			} else {
				common_item = item;
			}
		}
	}
	return common_item;
}

int cmp(const void* p1, const void* p2) {
	return strcmp(
		allergen_name[((struct Allergen*) p1)->allergen_index],
		allergen_name[((struct Allergen*) p2)->allergen_index]
	);
}

int main() {
	memset(ingredient_contains_allergen, -1, sizeof(ingredient_contains_allergen));
	char line[1000];
	FILE* fp = fopen(FILENAME, "r");
	while (1) {
		if (fgets(line, 1000, fp) == NULL) {
			break;
		}
		char (*list_to_add_to)[100] = ingredient_name;
		int (*food_list_to_add_to)[500] = ingredients;
		int i = 0;
		char* tok = strtok(line, " ");
		while (tok != NULL) {
			if (!strcmp(tok, "(contains")) {
				ingredients[num_foods][i] = -1;
				list_to_add_to = allergen_name;
				food_list_to_add_to = allergens;
				i = 0;
			} else {
				switch (tok[strlen(tok) - 1]) {
					case ',':
						tok[strlen(tok) - 1] = '\0';
						break;
					case '\n':
						tok[strlen(tok) - 2] = '\0';
						break;
					case ')':
						tok[strlen(tok) - 1] = '\0';
						break;
				}
				food_list_to_add_to[num_foods][i++] = find_or_add(tok, list_to_add_to);
			}
			tok = strtok(NULL, " ");
		}
		allergens[num_foods][i] = -1;
		num_foods++;
	}
	fclose(fp);

	for (int food = 0; food < num_foods; food++) {
		for (int ingredient = 0; ingredient_name[ingredient][0] != '\0'; ingredient++) {
			int idx = get_item_index(ingredients[food], ingredient);
			if (idx < 0) {
				for (int i = 0; allergens[food][i] >= 0; i++) {
					allergen_not_in_ingredients[allergens[food][i]][ingredient] = 1;
				}
			}
		}
	}
	int all_allergens_known = 0;
	while (!all_allergens_known) {
		all_allergens_known = 1;
		for (int allergen = 0; allergen_name[allergen][0] != '\0'; allergen++) {
			int unique_ingredient = -1;
			for (int ingredient = 0; ingredient_name[ingredient][0] != '\0'; ingredient++) {
				if (!allergen_not_in_ingredients[allergen][ingredient]) {
					if (unique_ingredient < 0) {
						unique_ingredient = ingredient;
					} else {
						unique_ingredient = -1;
						break;
					}
				}
			}
			if (unique_ingredient >= 0) {
				ingredient_contains_allergen[unique_ingredient] = allergen;
			} else {
				all_allergens_known = 0;
			}
		}
		for (int ingredient = 0; ingredient_name[ingredient][0] != '\0'; ingredient++) {
			int allergen = ingredient_contains_allergen[ingredient];
			if (allergen >= 0) {
				printf("Ingredient '%s' contains '%s'\n", ingredient_name[ingredient],
					allergen_name[allergen]);
				for (int a = 0; allergen_name[a][0] != '\0'; a++) {
					if (a != allergen) {
						allergen_not_in_ingredients[a][ingredient] = 1;
					}
				}
			}
		}
	}
	int count = 0;
	for (int ingredient = 0; ingredient_name[ingredient][0] != '\0'; ingredient++) {
		if (ingredient_contains_allergen[ingredient] < 0) {
			for (int food = 0; food < num_foods; food++) {
				for (int i = 0; ingredients[food][i] >= 0; i++) {
					if (ingredients[food][i] == ingredient) {
						count++;
					}
				}
			}
		}
	}
	printf("\nIngredients without allergens appear %d times.\n", count);

	int i;
	for (i = 0; allergen_name[i][0] != '\0'; i++) {
		sorted_allergen_list[i].allergen_index = i;
		int ingredient;
		for (ingredient = 0; ingredient_contains_allergen[ingredient] != i; ingredient++);
		sorted_allergen_list[i].ingredient_index = ingredient;
	}
	qsort(sorted_allergen_list, (size_t) i, sizeof(*sorted_allergen_list), cmp);
	puts("\nCanonical dangerous ingredients list:");
	char* sep = "";
	for (int i = 0; allergen_name[i][0] != '\0'; i++) {
		printf("%s%s", sep, ingredient_name[sorted_allergen_list[i].ingredient_index]);
		sep = ",";
	}
	puts("");
}
