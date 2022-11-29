#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define SIZE 255
#define MALLOC_EXC 1
#define DEBUG

typedef struct string {
	char* str;
	int c_sz;
	int r_sz;
} string;

typedef struct Node_t {
	char data;
	struct Node_t* next;

} Node_t;

typedef struct Node_int {
	int data;
	struct Node_int* next;
} Node_int;

int chislo(char symbol) {
	return (symbol < '0' || symbol > '9') ? 0 : 1;
	// isdigit нервно курит в сторонке (=
}

int operation(char symbol) {
	return (symbol == '-' || symbol == '+' || symbol == '*' ||
			symbol == '/' || symbol == '%' || symbol == '^');
}

int prior(char symbol) {
	if (symbol == '+' || symbol == '-') {
		return 2;
	} else if (symbol == '*' || symbol == '/' || symbol == '%') {
		return 3;
	} else if (symbol == '^') {
		return 4;
	} else {
		// тут всё правильно
		// скорее всего просто конец строки хендлится непральна
		// кстати с учётом того что у Тебя for, а не while,
		// до \0 дело могло даже не дойти... ._.
		return -1;
	}
}

int calculate(int first, int second, char operation, int* result) {
	if (operation == '+') {
		*result = first + second;
		return 1;
	} else if (operation == '-') {
		*result = first - second;
		return 1;
	} else if (operation == '*') {
		*result = first * second;
		return 1;
	} else if (operation == '/') {
		if (second == 0) {
			return -1;
		}
		*result = first / second;
		return 1;
	} else if (operation == '%') {
		if (second == 0) {
			return -2;
		}
		*result = first % second;
		return 1;
	} else if (operation == '^') {
		*result = pow(first, second);
		return 1;
	}
	return 0;
}

void cr_str(string* str) {
	str->c_sz = SIZE;
	str->r_sz = 0;
	str->str = (char*)malloc(sizeof(char) * SIZE);
}

int check_size(string* str) {
	if (str->r_sz == str->c_sz - 1) {
		char* p_str = (char*)realloc(str->str, sizeof(char) * 2 * str->c_sz);
		if (p_str == NULL) {
			return MALLOC_EXC;
		}
		str->str = p_str;
		str->c_sz = 2 * str->c_sz;
	}
	return 0;
}

int read_str(string* str) {
	char symb;
	while ((symb = getchar()) != '\n') {
		if (symb == EOF) {
			return 2;
		}
		check_size(str);
		str->str[str->r_sz++] = symb;
	}
	str->str[str->r_sz++] = '\0';
	return 0;
}

int push(Node_t** head, char elem) {
	Node_t* tmp = malloc(sizeof(Node_t));
	if (tmp == NULL) {
		return 1;
	}
	tmp->next = *head;
	tmp->data = elem;
	*head = tmp;
	return 0;
}

int pop(Node_t** head, char* elem) {
	Node_t* old;
	if (*head == NULL) {
		return 2;
	}
	old = *head;
	*elem = (*head)->data;
	*head = (*head)->next;
	free(old);
	return 0;
}

int push_int(Node_int** head, int elem) {
	Node_int* tmp = malloc(sizeof(Node_t));
	if (tmp == NULL) {
		return 1;
	}
	tmp->next = *head;
	tmp->data = elem;
	*head = tmp;
	return 0;
}

int pop_int(Node_int** head, int* elem) {
	Node_int* old;
	if (*head == NULL) {
		return 2;
	}
	old = *head;
	*elem = (*head)->data;
	*head = (*head)->next;
	free(old);
	return 0;
}

int check_str(string* str) {
	int count_brc = 0;
	if ((!(chislo(str->str[0]))) && (str->str[0] != '(')) {
		return 0;
	}
	if ((!(chislo(str->str[(str->r_sz)-2]))) && (str->str[(str->r_sz)-2] != ')')) {
		return 0;
	}
	for (int i = 0; i < (str->r_sz)-1; i++) {
		if (str->str[i] == '(') {
			if ((!chislo(str->str[i+1]) || chislo(str->str[i-1])) && (str->str[i+1] != '(' && str->str[i-1] != '(')) {
				return 0;
			}
			count_brc++;
		} else if (str->str[i] == ')') {
			if (count_brc == 0) {
				return 2;
			}
			if ((chislo(str->str[i+1]) || !chislo(str->str[i-1])) && (str->str[i+1] != ')' && str->str[i-1] != ')')) {
				return 0;
			}
			count_brc--;
		} else if (operation(str->str[i])) {
			if ((!(chislo(str->str[i+1]))) && (str->str[i+1] != '(')) {
				return 0;
			}
		} else if (!chislo(str->str[i])) {
			return 0;
		}
	}
	if (count_brc == 0) {
		return 1;
	} else {
		return 2;
	}
}

int count_expr(string str, int* result) {
	Node_int* head = NULL;
	int i = 0;
	int tmp_result = 0;
	int first = 0;
	int second = 0;
	int calculate_result;
	while (str.str[i] != '\0') {
		if (chislo(str.str[i])) {
			tmp_result = tmp_result*10+((str.str[i]) - '0');
		} else if (str.str[i] == ' ') {
			push_int(&head, tmp_result);
			tmp_result = 0;
		} else if (operation(str.str[i])) {
			pop_int(&head, &second);
			pop_int(&head, &first);
			calculate_result = calculate(first, second, str.str[i++], result);
			switch(calculate_result) {
				case 1:
					break;
				case -1:
					return -1;
				case -2:
					return -2;
				default:
					return 1;
			}
			push_int(&head, *result);
		}
		i++;
	}
	return 0;
}

int read_str_file(string* str, FILE* fp) {
	char c;
	while (!feof(fp)) {
		c = fgetc(fp);
		if (c == EOF) {
			check_size(str);
			str->str[str->r_sz++] = '\0';
			return 2;
		}
		if (c == '\n' || c == '\v' || c == '\r') {
			check_size(str);
			str->str[str->r_sz++] = '\0';
			return 0;
		}
		check_size(str);
		str->str[str->r_sz++] = c;
	}
	str->str[str->r_sz++] = '\0';
	return 0;
}

int main(int argc, char* argv[]) {
	string result;
	char rubbish;
	char rubbish_stack;
	cr_str(&result);
	int status_code_1 = 0;
	char elem;
	Node_t* head = NULL;
	

	string str;
	FILE* fp;
	FILE* fin;
	int status_code = 0;
	int count_expression = 0;
	fin = fopen("mistakes.txt", "w");
	if (fin == NULL) {
		printf("Can't open file");
	}
	for (int i = 1; i < argc; i++) {
		if ((fp = fopen(argv[i], "r")) == NULL) {
			printf("Can't open file %s\n", argv[i]);
			continue;
		}
		int index_str = 0;
		fprintf(fin, "%s\n", argv[i]);
		printf("FileName: %s\n\n", argv[i]);

		while (!feof(fp)) {
			push(&head, '(');
			cr_str(&str);
			cr_str(&result);
			read_str_file(&str, fp);

			status_code = check_str(&str);
			if (status_code == 0) {
				fprintf(fin, "%d: Wrong input str: %s\n", index_str, str.str);
			} else if (status_code == 2) {
				fprintf(fin, "%d: Problem with bracket\n", index_str);
			} else if (status_code == 1) {
				for (int i = 0; i < str.r_sz; i++) {
					if (chislo(str.str[i])) {
						check_size(&result);
						result.str[result.r_sz++] = str.str[i];
					} else {
						if (i != 0 && chislo(str.str[i - 1])) {
							result.str[result.r_sz++] = ' ';
						}
						if (str.str[i] == '(') {
							push(&head, '(');
						} else if (str.str[i] == ')' || str.str[i] == '\0') {
							while (head->data != '(') {

								check_size(&result);
								pop(&head, &result.str[result.r_sz++]);
								check_size(&result);
								result.str[result.r_sz++] = ' ';
							}
							pop(&head, &rubbish);
						} else if (operation(str.str[i])) {
							while (prior(str.str[i]) <= prior(head->data)) {
								check_size(&result);
								pop(&head, &result.str[result.r_sz++]);
								check_size(&result);
								result.str[result.r_sz++] = ' ';
							}
							push(&head, str.str[i]);
						}
					}
				}
				pop(&head, &rubbish_stack);
				result.str[result.r_sz++] = '\0';

				int* calculated = (int*)malloc(sizeof(int));
				if (calculated == NULL) {
					printf("Problem with malloc");
				}
				count_expression = count_expr(result, calculated);
				if (count_expression == -1) {
					fprintf(fin, "%d: Problem with /0\n", index_str);
				} else if (count_expression == -2) {
					fprintf(fin, "%d: Problem with %%0\n", index_str);
				} else if (count_expression == 1) {
					fprintf(fin, "%d: Some problem\n", index_str);
				} else {
					printf("String: %s\n", str.str);
					printf("Reverse polish notation: %s\n", result.str);
					printf("Result: %d\n", *calculated);
				}
			}
			free(str.str);
			free(result.str);
			index_str++;
		}
		fprintf(fin, "\n\n");
		printf("\n\n");
		fclose(fp);
	}
	fclose(fin);
	return 0;
}