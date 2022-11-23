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
		*result = first / second;
		return 1;
	} else if (operation == '%') {
		*result = first & second;
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
				return 0;
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
		return 0;
	}
}

int count_expr(string str1, int* result) {
	Node_int* head = NULL;
	int i = 0;
	int tmp_result = 0;
	int first = 0;
	int second = 0;
	while (str1.str[i] != '\0') {
		if (chislo(str1.str[i])) {
			tmp_result = tmp_result*10+((str1.str[i]) - '0');
		} else if (str1.str[i] == ' ') {
			push_int(&head, tmp_result);
			tmp_result = 0;
		} else if (operation(str1.str[i])) {
			pop_int(&head, &second);
			pop_int(&head, &first);
			calculate(first, second, str1.str[i++], result);
			push_int(&head, *result);
			if (str1.str[i] == '\0') {
				return 0;
			}
		}
		i++;
	}
	pop_int(&head, &tmp_result);
	return 0;
}

int main(void) {
	
	char rubbish;
	char rubbish_stack;
	Node_t* head = NULL;
	int status_code = 0;
	char elem;
	for( int i = 0; i < 2; i++) {
		string str;
		string result;
		push(&head, '(');
		cr_str(&result);
		cr_str(&str);
		read_str(&str);
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
			//printf("%c - Stack data\n", head->data);
		}	
		pop(&head, &rubbish_stack);
		result.str[result.r_sz++] = '\0';

		int* calculated;
		printf("%s\n", result.str);
		count_expr(result, calculated);
		printf("Check str: %c\n", str.str[1]);
		printf("Check str: %c\n", result.str[0]);
		printf("\n%d\n", *calculated);
		free(result.str);
		//free(str.str);
	}

	return 0;
}