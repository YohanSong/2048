#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

void gotoxy(int x, int y);
void game_start(int map[][17]);
void game_print(int map[][17]);
void number_print(int arr[][4], int score);
void create(int arr[][4]);
int create_check(int arr[][4]);
int dmove(int arr[][4], int d, int *score);
int *arr_ts(int arr[][4], int i, int j, int d);
void game_over(int *score);

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c) { //커서 표시를 바꿔주는 함수  
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void main() {
	setcursortype(NOCURSOR);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
	gotoxy(17, 6); printf("■■■■■    ■■■■■    ■      ■    ■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	gotoxy(17, 7); printf("        ■    ■      ■    ■      ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	gotoxy(17, 8); printf("        ■    ■      ■    ■      ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	gotoxy(17, 9); printf("        ■    ■      ■    ■      ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	gotoxy(17, 10); printf("■■■■■    ■      ■    ■■■■■    ■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	gotoxy(17, 11); printf("■            ■      ■            ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	gotoxy(17, 12); printf("■            ■      ■            ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	gotoxy(17, 13); printf("■            ■      ■            ■    ■      ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	gotoxy(17, 14); printf("■■■■■    ■■■■■            ■    ■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(27, 20); printf("PLEASE PRESS KEY"); _getch();
	int map[17][17];
	game_start(map);//map초기화
	while (1) {//게임 종료시 다시 시작
		int score = 100, input;
		int arr[4][4] = { 0, 2, 2 , };
		system("cls");//map을 새로그림
		game_print(map);//map을 draw함
		while (1) {//조작
			if (_kbhit()) {
				if (_getch() == 224) {//특수키
					switch (getch()) {//방향키를 입력받음
					case LEFT:  input = 1; break;
					case UP:    input = 2; break;
					case RIGHT: input = 3; break;
					case DOWN:  input = 4; break;
					}
					//더이상 움직일 수없고 배열이 꽉참 -> 게임 오버 -> 게임 재시작
					if (dmove(arr, input, &score)) break;
				}
				number_print(arr, score);//프린트 갱신
			}
		}
	}
}


//프린트할 위치
void gotoxy(int x, int y) {
	COORD Pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//맵을 초기화함
void game_start(int map[][17]) {
	int arr_temp[17][17] = {
		{ 11, 0, 0, 0, 22, 0, 0, 0, 22, 0, 0, 0, 22, 0, 0, 0, 12 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 21, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 23 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 21, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 23 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 21, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 23 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2, 5, 4, 4, 2 },
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2 },
		{ 14, 0, 0, 0, 24, 0, 0, 0, 24, 0, 0, 0, 24, 0, 0, 0, 13 }
	};
	int i, j;
	for (i = 0; i < 17; i++)
		for (j = 0; j < 17; j++)
			map[i][j] = arr_temp[i][j];
}

//맵을 프린트함
void game_print(int map[][17]) {
	int i = 0, j = 0;
	printf("\n    score : %d\n\n", 100);
	for (int i = 0; i < 17; i++) {
		printf("        ");
		for (int j = 0; j < 17; j++) {
			switch (map[i][j]) {
			case 0: printf("──"); break;
			case 1: printf("  "); break;
			case 2: printf("│"); break;
			case 3: printf("┼"); break;
			case 5: printf("      "); break;
			case 11: printf("┌"); break;
			case 12: printf("┐"); break;
			case 13: printf("┘"); break;
			case 14: printf("└"); break;
			case 21: printf("├"); break;
			case 22: printf("┬"); break;
			case 23: printf("┤"); break;
			case 24: printf("┴"); break;
			}
		}
		printf("\n");
	}
}

//숫자를 프린트함
void number_print(int arr[][4], int score) {
	int i, j;
	gotoxy(6, 1);
	printf("%d", score);

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			gotoxy(5 + j * 3, 5 + i * 4);
			if (arr[i][j] != 0) {
				printf("%5d", arr[i][j]);
			}
			else {
				printf("      ");
			}
		}
	}
}

//새로운 숫자 생성
void create(int arr[][4]) {
	int locate, number;
	srand((unsigned)time(NULL));//rand함수 랜덤설정
	if (rand() % 10 < 8) number = 2;//80퍼로 2생성
	else number = 4;// 20퍼로 4생성
	do {
		locate = rand() % 16;
	} while (arr[locate / 4][locate % 4] != 0);//빈칸나올때까지 랜덤
	arr[locate / 4][locate % 4] = number;
}

//create가 가능한지 체크
int create_check(int arr[][4]) {
	int i, cnt = 0;
	for (i = 0; i < 16; i++) {
		if (arr[i / 4][i % 4] != 0) //배열 int arr[0~3][0~3]
			cnt++;
	}
	if (cnt == 16) return 0;//배열이 꽉참 -> create불가
	else return 1;//creeate가능
}

//방향키이동
int dmove(int arr[][4], int d, int *score) {
	int i, j, s, check = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (*arr_ts(arr, i, j, d) != 0) {
				for (s = j + 1; s < 4; s++) {//뒤와 합치기
					if (*arr_ts(arr, i, j, d) == *arr_ts(arr, i, s, d)) {
						*arr_ts(arr, i, j, d) *= 2;//
						*arr_ts(arr, i, s, d) = 0;
						*score += *arr_ts(arr, i, j, d);//점수 추가
						check++;
						break;
					}
					else if (*arr_ts(arr, i, s, d) != 0) break;//뒤에 합치지 못하면 종료
				}


				for (s = j - 1; s >= 0; s--) {//앞에 이동할수 있나 체크
					if (*arr_ts(arr, i, s, d) != 0) {
						break;
					}
				}
				s++;
				if (j != s) {//앞에 이동할수 있으면 정렬
					*arr_ts(arr, i, s, d) = *arr_ts(arr, i, j, d);
					*arr_ts(arr, i, j, d) = 0;
					check++;
				}
			}

		}
	}
	// check == 0 -> 더 이상 움직일 수 없음 -> create 안함
	if (create_check(arr)) {
		if (check != 0) {
			create(arr);
		}
	}
	else//더이상 create 불가 시
	{
		game_over(score);
		return 1;//다시시작
	}
	return 0;
}

//배열을 돌림 -> 방향키가 달라도 같은 코드로 조작 가능
int *arr_ts(int arr[][4], int i, int j, int d) {
	int ti, tj;
	switch (d) {
	case 1: ti = i; tj = j;   break;
	case 2: ti = j; tj = 3 - i; break;
	case 3: ti = i; tj = 3 - j; break;
	case 4: ti = 3 - j; tj = 3 - i; break;
	}
	return &arr[ti][tj];
}

//게임 오버
void game_over(int *score) {
	system("cls");
	gotoxy(10, 5); printf("Game Over");
	gotoxy(10, 6); printf("Score : %d", *score);
	gotoxy(10, 7); printf("PLEASE PRESS KEY");
	_getch();
	
}