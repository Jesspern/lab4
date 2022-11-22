#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 255
#define MALLOC_EXC 1

typedef struct string {
	char* str;
	int c_sz;
	int r_sz;
} string;

typedef struct Node_t {
	char data;
	struct Node_t* next;

} Node_t;

typedef struct Stuck {
	int length;
	Node_t* head;

} Stuck;

int chislo(char symbol) {
	return (symbol < '0' || symbol > '9') ? 0 : 1;
}

int operation(char symbol) {
	return (symbol == '-' || symbol == '+' || symbol == '*' || symbol == '/' || symbol == '%') ? 1 : 0;
}

int prior(char symbol) {
	if (symbol == '(') {
		return 1;
	} else if (symbol == '+' || symbol == '-') {
		return 2;
	} else if (symbol == '*' || symbol == '/') {
		return 3;
	} else if (symbol == '^') {
		return 4;
	} else if (symbol == ')') {
		return 0;
	} else {
		return -1;
	}
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

int check_str(string* str) {
	int count_brc = 0;
	if ((!(chislo(str->str[0]))) && (str->str[0] != '(')) {
		return 0;
	}
	if ((!(chislo(str->str[(str->r_sz)-2]))) && (str->str[(str->r_sz)-2] != ')')) {
		return 0;
	}
	for (int i = 1; i < (str->r_sz)-2; i++) {
		if (str->str[i] == '^') {
			if (str->str[i+1] == '-') {
				return 0;
			} else if (!(chislo(str->str[i+1]))) {
				return 0;
			}
		} else if (str->str[i] == '(') {
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

int main(void) {
	string str;
	string result;
	cr_str(&result);
	int status_code = 0;
	char elem;
	Node_t* head = NULL;
	cr_str(&str);
	read_str(&str);
	printf("%d\n", check_str(&str));
	/*for(int i = 0; i < 3; i++) {
		cr_str(&str);
		read_str(&str);
		if (check_str(&str) == 1) {
			break;
		}
		free(str.str);
		printf("Please, try again\n");
	}*/
	for (int i = 0; i<str.r_sz-1; i++) {
		if (chislo(str.str[i])) {
			check_size(&result);
			result.str[result.r_sz++] = str.str[i];
		} else {
			if (head == NULL) {
				push(&head, str.str[i]);
			} else {
				if (str.str[i] == '(') {
					push(&head, str.str[i]);
				} else if (str.str[i] == ')') {
					while (head->data == '(') {
						check_size(&result);
						pop(&head, &result.str[result.r_sz++]);
					}
				} else if (prior(str.str[i]) > prior(head->data)) {
					push(&head, str.str[i]);
				} else if (prior(str.str[i]) <= prior(head->data)) {
					int j = 2;
					char a;
					while (prior(str.str[i]) <= prior(head->data) && j != 0) {
						check_size(&result);
						printf("%c\n", head->data);
						pop(&head, &result.str[result.r_sz++]);
						j--;
					}
				} else if (str.str[i] == '\0') {
					while (head != NULL) {
						check_size(&result);
						pop(&head, &result.str[result.r_sz++]);
					}
				}
			}
		}
	}
	printf("%s\n", result.str);
	printf("%c\n", head->data);

	return 0;
}