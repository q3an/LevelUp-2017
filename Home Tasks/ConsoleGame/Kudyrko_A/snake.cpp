/*Classic snake ver1.0 made by q3an in 28.10.2017
To compile you should have all libraries.
*/
#include <stdio.h>
#include <locale.h>
#include <conio.h> // см. ниже (это тоже)(ну почти)
#include <windows.h> /*Да, это костыль. 
Если пояснишь за удобную задержку и отчистку экрана,
не зависящие от платформ и без windows.h, то ты молодец.*/
// MAIN SETTINGS
#define X_SIZE 55
#define Y_SIZE 25// size of GUI
#define GAME_DELAY 300 //delay in msec
#define START_X 27
#define START_Y 10// start position of snake's head
#define START_DIRECTION 's'// s - down, w - up, a - left, d - right
#define START_SIZE 4
#define SNAKE_VIEW '0'
#define FOOD_VIEW '$'
#define START_FOOD_N 1
#define FOOD_TIMER 20
struct dot { 
	int x, y;
	char view;
};
int encounter_with_tail(dot*, int);
void swap(dot*, dot*);
void grow_snake(dot**, int*);
void delete_food(dot**, dot* , int*, int, int);
int encounter_with_food(dot, dot*, int);
void gen_new_food(dot**, int*);
void print_screen(dot*, int, dot*, int, int); //"graphics engine ver1.0"
void delay(int); 
char control(char); // fun returns chosen dimension of a snake
void add_vector(dot*, char); // fun moves snake_piece
int main() {
	setlocale(LC_ALL,"Russian");
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN);
	// ещё один костыль... Помогает изменить текст консольки (love classics!) 
	// ( я не втыкаю, что написано в этой части кода)
	// creating a snake
	int score = 0;
	char is_alive = 1, swap;
	int size = START_SIZE;
	dot* snake;
	char vector = START_DIRECTION;
	if (!(snake = (dot*)malloc(START_SIZE * sizeof(dot)))) {
		printf("\nMEMORY ERROR\n");
		return 1;
	}
	for(int i = 0; i < START_SIZE; i++) {
		snake[i].x = START_X;
		snake[i].y = START_Y - i;
		snake[i].view = SNAKE_VIEW;
	}
	// creating food dots
	dot* food;
	int food_n = START_FOOD_N;
	int food_timer = 0;
	if (!(food = (dot*)malloc(START_FOOD_N * sizeof(dot*)))) {
		free(snake);
		printf("\nMEMORY ERROR\n");
		return 1;
	}
	for (int i = 0; i < START_FOOD_N; i++) {
		food[i].view = FOOD_VIEW;
		food[i].x = rand() % X_SIZE;
		food[i].y = rand() % Y_SIZE;
	}
	// complete
	// game process
	while (is_alive) {
		print_screen(snake, size, food, food_n, score);
		if (food_timer == FOOD_TIMER) {
			food_timer = 0;
			gen_new_food(&food, &food_n);
		}
		else {
			food_timer++;
		}
		vector = _kbhit() && (swap = control(vector)) != '0' ? swap : vector;
		for (int i = size - 1; i > 0; i--) {
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		add_vector(snake, vector);
		if (encounter_with_food(snake[0], food, food_n)) {
			grow_snake(&snake, &size);
			delete_food(&food, food, &food_n, snake[0].x, snake[0].y);
			score++;
			printf("\a");
		}
		if (encounter_with_tail(snake, size)) {
			printf("\a\a\a");
			is_alive = 0;
		}
		delay(1);
		rewind(stdin);
		system("CLS");
	}
	printf("\n\t\t\tG a M e    O v E r\n");
	printf("\n\t\t\tSCORE: %d\n", score);
	free(food);
	free(snake);
	_getch();
	return 0;
}
int encounter_with_tail(dot* snake, int size) {
	for (int i = 1; i < size; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			return 1;
		}
	}
	return 0;
}
void grow_snake(dot** snake_p, int *size) {
	dot* temp;
	do {
		temp = (dot*)malloc((*size) * sizeof(dot));
	} while (!temp);
	for (int i = 0; i < *size; i++) {
		temp[i] = *(*snake_p + i);
	}
	dot* temp1;
	do {
		temp1 = (dot*)realloc(*snake_p, sizeof(dot) * (*size + 1));
	} while (!temp1);
	for (int i = 0; i < *size; i++) {
		temp1[i] = temp[i];
	}
	temp1[*size] = temp1[*size - 1];
	*size += 1;
	*snake_p = temp1;
	free(temp);
}
void delete_food(dot** foodp, dot* food, int* food_n, int x, int y) {
	int i;
	for (i = 0; i < *food_n; i++) {
		if (food[i].x == x && food[i].y == y) {
			break;
		}
	}
	*food_n -= 1;
	swap(food + i, food + *food_n);
	dot* temp;
	do {
		temp = (dot*)malloc(sizeof(dot) * *food_n);
	} while (!temp);
	for (int j = 0; j < *food_n; j++) {
		temp[j] = food[j];
	}
	dot* temp1;
	do {
		temp1 = (dot*)realloc(food, sizeof(dot) * *food_n);
	} while (!temp1);
	for (int j = 0; j < *food_n; j++)
	{
		temp1[j] = temp[j];
	}
	*foodp = temp1;
	free(temp);
}
void swap(dot* one, dot* two) {
	dot temp = *one;
	*one = *two;
	*two = temp;
}
int encounter_with_food(dot snake_head, dot* food, int food_n) {
	for (int i = 0; i < food_n; i++) {
		if (snake_head.x == food[i].x && snake_head.y == food[i].y) {
			return 1;
		}
	}
	return 0;
}
void gen_new_food(dot** foodp, int* food_n) {
	dot* temp;
	do {
		temp = (dot*)malloc((*food_n) * sizeof(dot));
	} while (!temp);
	for (int i = 0; i < *food_n; i++) {
		temp[i] = *(*foodp + i);
	}
	dot* temp1;
	do {
		temp1 = (dot*)realloc(*foodp, sizeof(dot) * (*food_n + 1));
	} while (!temp1);
	temp1[*food_n].view = FOOD_VIEW;
	temp1[*food_n].x = rand() % X_SIZE;
	temp1[*food_n].y = rand() % Y_SIZE;
	for (int i = 0; i < *food_n; i++) {
		temp1[i] = temp[i];
	}
	*food_n += 1;
	*foodp = temp1;
	free(temp);
}
void print_screen(dot* snake, int snake_size, dot* food, int food_n, int score) {
	printf("/////////////////////////////////////SCORE: %d////////////\n", score);
	char matrix[Y_SIZE][X_SIZE] = {};
	for (int i = 0; i < food_n; i++) {
		matrix[food[i].y][food[i].x] = food[i].view;
	}
	for (int i = 0; i < snake_size; i++) {
		matrix[snake[i].y][snake[i].x] = snake[i].view;
	}
	for (int j = 0; j < Y_SIZE; j++) {
		for (int i = 0; i < X_SIZE; i++) {
			if (matrix[j][i] == 0) {
				putc(' ', stdout);
				continue;
			}
			putc(matrix[j][i], stdout);
		}
		printf("/\n");
	}
	printf("/////////////////////////////////////SCORE: %d////////////\n", score);
}
void add_vector(dot* snake, char vector) {
	dot temp = *snake;
	char direction = vector;
	switch (direction) {
	case 'w':
		temp.y--;
		temp.y = temp.y == -1 ? Y_SIZE - 1 : temp.y;
		break;
	case 's':
		temp.y++;
		temp.y = temp.y == Y_SIZE ? 0 : temp.y;
		break;
	case 'd':
		temp.x++;
		temp.x = temp.x == X_SIZE ? 0 : temp.x;
		break;
	case 'a':
		temp.x--;
		temp.x = temp.x == -1 ? X_SIZE - 1 : temp.x;
		break;
	}
	*snake = temp;
}
void delay(int sec) {
	Sleep(sec * GAME_DELAY);
}
char control(char last_vector){
	switch (_getch()) {
	case 72:
	case 'Ц':
	case 'ц':
	case 'W':
	case 'w':
		if (last_vector == 'w' || last_vector == 's') {
			return '0';
		}
		else {
			return 'w';
		}
	case 80:
	case 'Ы':
	case 'ы':
	case 'S':
	case 's':
		if (last_vector == 'w' || last_vector == 's') {
			return '0';
		}
		else {
			return 's';
		}
	case 75:
	case 'Ф':
	case 'ф':
	case 'A':
	case 'a':
		if (last_vector == 'a' || last_vector == 'd') {
			return '0';
		}
		else {
			return 'a';
		}
	case 77:
	case 'В':
	case 'в':
	case 'D':
	case 'd':
		if (last_vector == 'a' || last_vector == 'd') {
			return '0';
		}
		else {
			return 'd';
		}
	default:
		return '0';
	}
}
