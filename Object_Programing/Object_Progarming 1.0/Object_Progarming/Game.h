/*
이 소스는 프로그램내에서 화면의 출력과 사용자의 입력에 대한 처리를 하는 Class를 정의하기 위함
*/
// X축 □ 80개 Y축 36개
#include"Client.h"
#include"Summative.h"
#include<windows.h>
#include<conio.h>

//------------------------------------------테트리스에 사용되는 정의------------------------------------------------------------
#define LEFT 75 //좌로 이동 
#define RIGHT 77 //우로 이동 
#define UP 72 //회전 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define ESC 27 //게임종료
#define ACTIVE_BLOCK 1 // 게임판배열에 저장된 움직일 수 있는 블록 
#define EMPTY 0         // 비어 있는 공간
#define INACTIVE_BLOCK 2 // 블럭의 이동이 끝났을때.
//------------------------------------------------------------------------------------------------------------------------------

class Game{
public:
	Client *pclient=&client;
	friend void gotoxy(short x, short y);
	friend void rect();
	Game();
};

Game::Game(){
	Title;
	Map_max_size;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(console, &CURSOR);
}

void gotoxy(short x, short y){
	COORD pos = { x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
};

//사각형 그리기 함수
void rect(){
	cls;
	gotoxy(0, 0);
	for (int i = 0; i < 36; i++){
		if (i == 0 || i == 35)
			for (int j = 0; j < 78; j++){
				cout << "▨";
			}
		cout << "▨";
		gotoxy(156, i);;
		cout << "▨\n";
	}
};
//전체적인 기본틀관련 Draw 기반클래스
class Start :public Game{
public:
	virtual void draw() = 0;
	virtual void option() = 0;
};
// *******************************************************************************************************************************************************************************************************
//테트리스게임 기본틀관련 Draw 기반클래스
class Tetris :public Game{
protected:
	int space_on; //space키를 체크하기위한 변수
	int crush_on; //블럭이 바닥에 닿았는지 체크하기위한 변수
	int newblock_on; //새로운 블럭이 필요함 체크
	int gameovercheck; // 게임오버 체크
	int b_type; //블록 종류를 저장 
	int b_rotation; //블록 회전값 저장 
	int b_type_next; //다음 블록값 저장
	int bx; //이동중인 블럭의 X좌표값의 위치를 구하기위해 필요한 변수
	int by; //이동중인 블럭의 Y좌표값의 위치를 구하기위해 필요한 변수
	int key; // 사용자의 키입력값을 받아 저장하기위한 변수
	int copyarray[Y_size][X_size]; // 싱글플레이 또는 멀티에서 자기자신의 블럭정보를 저장하기위한 배열 깜빡임을 방지하기 위해 copy용으로 사용.
	int copyonearray[Y_size][X_size]; //멀티플레이에서 사용할 플레이어1의 정보깜빡임을 방지하기 위해 copy용으로 사용.
	int copytwoarray[Y_size][X_size]; //멀티플레이에서 사용할 플레이어2의 정보 깜빡임을 방지하기 위해 copy용으로 사용.
	int copythreearray[Y_size][X_size]; //멀티플레이에서 사용할 플레이어3의 정보 깜빡임을 방지하기 위해 copy용으로 사용.

	void inputkey(); // 사용자의 키입력
	bool checkcrush(int bx, int by, int b_rotation); // 블럭의 충돌이 있는지 미리검사.
	void moveblock(int key); // 입력값에 따른 블럭 움직임.
	void newblock(); // 새로운 블럭 생성.
	void dropblock(); // 블럭을 Y축 한칸이동.
	void checkline(); // 블럭 라인 체크.
	void gameover(); // 게임오버 체크
	void info();
	void drawcheck(); // 깜빡임을 방지하기위해 copy와 비교

public:
	virtual void draw() = 0;
	virtual void option() = 0;
	void reset(){ // 생성자와 동일하게 초기화를 실행하는 메소드
		for (int i = 0; i < Y_size; i++){
			for (int j = 0; j < X_size; j++){
				pclient->drawarray[i][j] = 0;
				copyarray[i][j] = 0;
				copyonearray[i][j] = 0;
				copytwoarray[i][j] = 0;
				copythreearray[i][j] = 0;
			}
		}
		space_on = 0;
		crush_on = 0;
		newblock_on = 1;
		gameovercheck = 0;
		key = 0;
		bx = 0;
		by = 0;
		b_type = 0;
		srand((unsigned)time(NULL));
		b_type_next = rand() & 7;
		b_rotation = 0;
	};
	Tetris(){
		for (int i = 0; i < Y_size; i++){
			for (int j = 0; j < X_size; j++){
				pclient->drawarray[i][j] = 0;
				copyarray[i][j] = 0;
				copyonearray[i][j] = 0;
				copytwoarray[i][j] = 0;
				copythreearray[i][j] = 0;
			}
		}
		space_on = 0;
		crush_on = 0;
		newblock_on = 1;
		gameovercheck = 0;
		key = 0;
		bx = 0;
		by = 0;
		b_type = 0;
		srand((unsigned)time(NULL));
		b_type_next = rand()&7;
		b_rotation = 0;
	};
};

static int blocks[7][4][4][4] = { //[ 블럭의 종류 ][ 블럭의 형태변경][블럭 x축][ 블럭 y축]
	{ { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } } }, // 네모블럭
	{ { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 } }, { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 } }, { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // 일자블럭
	{ { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } } }, // z자 블럭
	{ { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // z자 대칭블럭
	{ { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 1, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 } } }, // 긴 ㄴ자 블럭
	{ { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 1, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // 긴 ㄴ 대칭블럭
	{ { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 1, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } } } };  // ㅗ자 블럭

void Tetris::inputkey(){// 사용자의 키입력
	key = 0;

	if (_kbhit()){ //키입력이 있는 경우  
		key = _getch(); //키값을 받음
		if (key == 224){ //방향키인경우 
			do{ key = _getch(); } while (key == 224);//방향키지시값을 버림 
			switch (key){
			case LEFT: //왼쪽키 눌렀을때  
				if (checkcrush(bx - 1, by, b_rotation) == true)
					moveblock(LEFT);
				break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
			case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 
				if (checkcrush(bx + 1, by, b_rotation) == true)
					moveblock(RIGHT);
				break;
			case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨 
				if (checkcrush(bx, by + 1, b_rotation) == true)
					moveblock(DOWN);
				break;
			case UP: //위쪽 방향키 눌렀을때 
				if (checkcrush(bx, by, (b_rotation + 1) % 4) == true)
					moveblock(UP);//회전할 수 있는지 체크 후 가능하면 회전
				else if (crush_on == 1 && checkcrush(bx, by - 1, (b_rotation + 1) % 4) == true)
					moveblock(777);//바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
			}
		}
		else{ //방향키가 아닌경우 
			switch (key){
			case SPACE: //스페이스키 눌렀을때 
				space_on = 1; //스페이스키 체크 
				while (crush_on == 0){ //바닥에 닿을때까지 이동시킴 
					dropblock();
					pclient->score += 1; // drop 보너스 1점씩 space누름 지점으로부터 추가
				}
				break;
				/*
			case P: 
			case p:  
				
				break;
			case ESC: 
				system("cls"); 
				exit(0); 
				*/
			}
		}
	}
	while (_kbhit())
		_getch();
}

//블럭의 충돌이 있는지 미리검사. 지금 여기서 문제야!! if문지우면 잘작동됨
bool Tetris::checkcrush(int bx, int by, int b_rotation){ //지정된 좌표와 회전값으로 충돌이 있는지 검사 

	for (int i = 0; i<4; i++){
		for (int j = 0; j < 4; j++){ //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
			if (blocks[b_type][b_rotation][i][j] == ACTIVE_BLOCK && pclient->drawarray[by + i][bx + j + ((X_size / 2) - 2)] == INACTIVE_BLOCK)
				return false;
			if (by + i < 1 || by + i >Y_size || bx + j + ((X_size / 2) - 2) < -1 || bx + j + ((X_size / 2) - 2) > X_size + 1){
				crush_on = 1;
				return false;
			}
		}
	}
	return true; //하나도 안겹치면 true리턴 
}

void Tetris::moveblock(int key){// 입력값에 따른 블럭 움직임.
	switch (key){
	case LEFT: //왼쪽방향 
		for (int i = 0; i < 4; i++){ //현재좌표의 블럭을 지움 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		for (int i = 0; i < 4; i++){ //왼쪽으로 한칸가서 active block을 찍음 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--; //좌표값 이동 
		break;

	case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP: //키보드 위쪽 눌렀을때 회전시킴. 
		for (int i = 0; i < 4; i++){ //현재좌표의 블럭을 지움  
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시키고 블럭의종류는 4개이므로 4보다 클경우 0으로 다시돌린다
		for (int i = 0; i < 4; i++){ //회전된 블록을 찍음 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 777: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
		//이를 동작시키는 특수동작 
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}

void Tetris::newblock(){
	if (newblock_on == 1){
		b_type = 0;
		b_rotation = 0;
		bx = 0;
		by = 0;
		b_type = b_type_next; //다음블럭값을 가져옴 
		b_type_next = rand() % 7; //다음 블럭을 만듦
		for (int i = 0; i < 4; i++){
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (pclient->drawarray[i][j] == EMPTY)
					pclient->drawarray[i][j] = blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)];
				else if (pclient->drawarray[i][j] != EMPTY){
					gameovercheck = 1;
				}
			}
		}
	}
	newblock_on = 0;
	info();
}

void Tetris::drawcheck(){
	for (int i = 0; i < Y_size; i++){
		for (int j = 0; j < X_size; j++){
			if (pclient->scenenumber == 7){
				if (copyarray[i][j] != pclient->drawarray[i][j]){
					if (pclient->drawarray[i][j] == ACTIVE_BLOCK){
						switch (b_type){ //블럭에 색상을 입히기위한 문장.
						case 0:RED break;
						case 1:BLUE break;
						case 2:YELLOW break;
						case 3:PLUM break;
						case 4:SKY_BLUE break;
						case 5:WHITE break;
						case 6:GREEN break;
						}
						gotoxy((j * 2) + 2, i + 1); cout << "■";
						ORIGINAL
					}
					if (pclient->drawarray[i][j] == EMPTY){
						gotoxy((j * 2) + 2, i + 1); cout << "  ";
					}
					if (pclient->drawarray[i][j] == INACTIVE_BLOCK){
						gotoxy((j * 2) + 2, i + 1); cout << "■";
					}
					copyarray[i][j] = pclient->drawarray[i][j];
				}
			}

			if (pclient->scenenumber == 8)
			{
				//자기 판을 비교
				if (copyarray[i][j] != pclient->drawarray[i][j]){
					if (pclient->drawarray[i][j] == ACTIVE_BLOCK){
						switch (b_type){ //블럭에 색상을 입히기위한 문장.
						case 0:RED break;
						case 1:BLUE break;
						case 2:YELLOW break;
						case 3:PLUM break;
						case 4:SKY_BLUE break;
						case 5:WHITE break;
						case 6:GREEN break;
						}
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "■";
						ORIGINAL
					}
					if (pclient->drawarray[i][j] == EMPTY){
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->drawarray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "■";
					}
					copyarray[i][j] = pclient->drawarray[i][j];
				}
				//player1 비교
				if (copyonearray[i][j] != pclient->playeronearray[i][j]){
					ORIGINAL
					if (pclient->playeronearray[i][j] == ACTIVE_BLOCK){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "■";
					}
					if (pclient->playeronearray[i][j] == EMPTY){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "  ";
					}
					if (pclient->playeronearray[i][j] == INACTIVE_BLOCK){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "■";
					}
					copyonearray[i][j] = pclient->playeronearray[i][j];
				}

				//player2 비교
				if (copytwoarray[i][j] != pclient->playertwoarray[i][j]){
					ORIGINAL
					if (pclient->playertwoarray[i][j] == ACTIVE_BLOCK){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "■";
					}
					if (pclient->playertwoarray[i][j] == EMPTY){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->playertwoarray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "■";
					}
					copytwoarray[i][j] = pclient->playertwoarray[i][j];
				}
				//player3 비교
				if (copythreearray[i][j] != pclient->playerthreearray[i][j]){
					ORIGINAL
					if (pclient->playerthreearray[i][j] == ACTIVE_BLOCK){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "■";
					}
					if (pclient->playerthreearray[i][j] == EMPTY){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->playerthreearray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "■";
					}
					copythreearray[i][j] = pclient->playerthreearray[i][j];
				}
			}
		}
	}
}

//줄완성시 위에줄전부삭제되는점 해결할것.
void Tetris::checkline(){
	int checkline = 0;
	for (int i = 0; i < Y_size; i++){
		for (int j = 0; j < X_size; j++){
			if (pclient->drawarray[i][j] == 2)
				checkline++;
		}
		if (checkline == X_size){
			for (int j = 0; j < X_size; j++)
				pclient->drawarray[i][j] = 0;
			for (int a = i; a > 0; a--){
				for (int k = 0; k < X_size; k++){
					pclient->drawarray[a][k] = pclient->drawarray[a - 1][k];
			}
		}
			pclient->score += 50; // 라인클리어. 50점 추가
		}
		checkline = 0;
	}
	info();
}

void Tetris::dropblock(){

	if (crush_on&&checkcrush(bx, by + 1, b_rotation) == true) crush_on = 0; //밑이 비어있으면 crush flag 끔 
	if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false){ //밑이 비어있지않고 crush flag가 켜져있으면
		for (int i = 0; i < Y_size; i++){ //현재 조작중인 블럭을 굳힘 
			for (int j = 0; j < X_size; j++){
				if (pclient->drawarray[i][j] == ACTIVE_BLOCK) pclient->drawarray[i][j] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0; //flag를 끔 
		checkline(); //라인체크를 함 
		newblock_on = 1; //새로운 블럭생성 flag를 켬    
		return; //함수 종료 
	}
	if (checkcrush(bx, by + 1, b_rotation) == true) moveblock(DOWN); //밑이 비어있으면 밑으로 한칸 이동 
	if (checkcrush(bx, by + 1, b_rotation) == false) crush_on = 1; //밑으로 이동이 안되면  crush flag를 켬
}

void Tetris::gameover(){
	int esccheck = 0;
	if (pclient->scenenumber == 7){
		gotoxy(2, 10); cout << "┌─────────────┐";
		gotoxy(2, 11); cout << "│ ☆ Game Over!! ☆        │";
		gotoxy(2, 12); cout << "│점수는 " << pclient->score << "      점 입니다!│";
		gotoxy(2, 13); cout << "│--------------------------│";
		gotoxy(2, 14); cout << "│ 진행하시려면             │";
		gotoxy(2, 15); cout << "│   ESC키를 누르세요!      │";
		gotoxy(2, 16); cout << "└─────────────┘";
	}
	if (pclient->scenenumber == 8){
		gotoxy(98, 10); cout << "┌─────────────┐";
		gotoxy(98, 11); cout << "│ ☆ Game Over!! ☆        │";
		gotoxy(98, 12); cout << "│점수는 " << pclient->score << "      점 입니다!│";
		gotoxy(98, 13); cout << "│--------------------------│";
		gotoxy(98, 14); cout << "│ 진행하시려면             │";
		gotoxy(98, 15); cout << "│   ESC키를 누르세요!      │";
		gotoxy(98, 16); cout << "└─────────────┘";
	}
	while (true)
	{
			esccheck = _getch(); //키값을 받음
			if (esccheck == 27) //esc키를 누를시에 종료
				break;
	}
}

void Tetris::info(){
	if (pclient->scenenumber == 7){
		gotoxy(48, 7); cout << pclient->score;
		gotoxy(48, 6); cout << (pclient->score / 1000) + 1;
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				gotoxy(46 + (j * 2), 11 + i);
				switch (b_type_next){ //블럭에 색상을 입히기위한 문장.
				case 0:RED break;
				case 1:BLUE break;
				case 2:YELLOW break;
				case 3:PLUM break;
				case 4:SKY_BLUE break;
				case 5:WHITE break;
				case 6:GREEN break;
				}
				if (blocks[b_type_next][0][i][j] == 1)
					cout << "■";
				else
					cout << "  ";
			}
		}
		ORIGINAL
	}
	if (pclient->scenenumber == 8){

		gotoxy(16, 30); cout << pclient->usernickname[1];
		gotoxy(16, 31); cout << (pclient->scoreone / 1000) + 1;
		gotoxy(16, 32); cout << pclient->scoreone;

		gotoxy(46, 30); cout << pclient->usernickname[2];
		gotoxy(46, 31); cout << (pclient->scoretwo / 1000) + 1;
		gotoxy(46, 32); cout << pclient->scoretwo;

		gotoxy(78, 30); cout << pclient->usernickname[3];
		gotoxy(78, 31); cout << (pclient->scorethree / 1000) + 1;
		gotoxy(78, 32); cout << pclient->scorethree;

		WHITE
		gotoxy(110, 30); cout << pclient->usernickname[0];
		gotoxy(110, 31); cout << (pclient->score / 1000) + 1;
		gotoxy(110, 32); cout << pclient->score;
		ORIGINAL
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				gotoxy(140 + (j * 2), 19 + i);
				switch (b_type_next){ //블럭에 색상을 입히기위한 문장.
				case 0:RED break;
				case 1:BLUE break;
				case 2:YELLOW break;
				case 3:PLUM break;
				case 4:SKY_BLUE break;
				case 5:WHITE break;
				case 6:GREEN break;
				}
				if (blocks[b_type_next][0][i][j] == 1)
					cout << "■";
				else
					cout << "  ";
			}
		}
		ORIGINAL
	}
}


// ******************************************************************************************************************************************************************************************************
//캐치마인드 기본틀관련 Draw 기반클래스 - 아직 미구현
class CatchMind :public Game{
public:
	virtual void draw() = 0;
	virtual void option() = 0;
};

// *******************************************************************************************************************************************************************************************************
//시작화면
class  One_Scence:public Start{
public:
		virtual void draw(){
			rect();

			BLUE;
			gotoxy(25, 5); cout << "■        ■  ■■■  ■     ■    ■■■  ";
			gotoxy(25, 6); cout << "■■    ■■    ■    ■■   ■      ■    ";
			gotoxy(25, 7); cout << "■  ■■  ■    ■    ■ ■  ■      ■    ";
			gotoxy(25, 8); cout << "■        ■    ■    ■   ■■      ■    ";
			gotoxy(25, 9); cout << "■        ■  ■■■  ■     ■    ■■■  ";
			GREEN;
			gotoxy(75, 5); cout << "  ■■■■         ■     ■  ■ ■  ■■■■■  ";//GAME
			gotoxy(75, 6); cout << " ■              ■  ■   ■■ ■■  ■          ";
			gotoxy(75, 7); cout << " ■    ■■■   ■■■■  ■     ■  ■■■■■  ";
			gotoxy(75, 8); cout << " ■      ■     ■    ■  ■     ■  ■          ";
			gotoxy(75, 9); cout << "  ■■■■      ■    ■  ■     ■  ■■■■■  ";
			ORIGINAL;
			gotoxy(58, 19); cout << "원하시는 플레이를 선택해주세요!!";
			gotoxy(60, 21); cout << "1. 싱글 플레이< Single Type Play >";
			gotoxy(60, 23); cout << "2. 멀티 플레이< Multi Type Play >";
			gotoxy(60, 25); cout << "3. 만든 사람  < Created by ~ >";
			gotoxy(59, 26); cout << "┌────────────────┐";
			gotoxy(59, 27); cout << "│↑↓위아래 이동 Space Key = 선택│";
			gotoxy(59, 28); cout << "└────────────────┘";
		};
	virtual void option(){
		int startoption = 1;
		bool startoptionactive = false;
		while (!GetAsyncKeyState(VK_SPACE)){
			if (GetAsyncKeyState(VK_UP)){
				startoption++;
				startoptionactive = true;
			}
			if (GetAsyncKeyState(VK_DOWN)){
				startoption--;
				startoptionactive = true;

			}
			if (startoption > 6) startoption = 4;
			if (startoption < 4) startoption = 6;
			if (startoptionactive == true){
				switch (startoption){
				case 4:
					gotoxy(57, 25); cout << " ";
					gotoxy(57, 23); cout << " ";
					gotoxy(57, 21); cout << "→";
					startoptionactive = false;
					break;

				case 5:
					gotoxy(57, 21); cout << " ";
					gotoxy(57, 25); cout << " ";
					gotoxy(57, 23); cout << "→";
					startoptionactive = false;
					break;

				case 6:
					gotoxy(57, 21); cout << " ";
					gotoxy(57, 23); cout << " ";
					gotoxy(57, 25); cout << "→";
					startoptionactive = false;
					break;
				}
			}
		}
		if (startoption == 4)
			pclient->scenenumber = 7;
		if (startoption == 5)
			pclient->scenenumber = 2;
		if (startoption == 6)
			pclient->scenenumber = 5;
	}
};

// *******************************************************************************************************************************************************************************************************

//서버 로그인 화면
class Two_Scence :public Start
{
public:
	virtual void draw(){
		rect();
		gotoxy(50, 23);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
		gotoxy(50, 24);  cout << "▨- 정보가 없을 경우 ID나 Password에 new를 입력해주세요.▨";
		gotoxy(50, 25);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
		gotoxy(50, 26);  cout << "▨ ID :                                                 ▨";
		gotoxy(50, 27);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
		gotoxy(50, 28);  cout << "▨ Password :                                           ▨";
		gotoxy(50, 29);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
		gotoxy(50, 30);  cout << "▨          로그인[Enter]        취소[ESC]              ▨";
		gotoxy(50, 31);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";

	};
	virtual void option(){
		int enter = 0;
		gotoxy(58, 26); cin >> pclient->userid;
		if (strcmp(pclient->userid.c_str(), "new") == 0){
			pclient->scenenumber = 6;
			return;
		}
		gotoxy(64, 28); cin >> pclient->userpassword;
		if (strcmp(pclient->userpassword.c_str(), "new")==0){
			pclient->scenenumber = 6;
			return;
		}
		while (true){
			if (GetAsyncKeyState(VK_RETURN) && pclient->serveraccess == true){ //엔터키
				pclient->scenenumber = 3;
				break;
			}
			if (GetAsyncKeyState(VK_ESCAPE)){ // ESC키
				pclient->scenenumber = 1;
				break;
			}
		}
	};
};

// *******************************************************************************************************************************************************************************************************

//서버 메인화면
class Three_Scence :public Start{
public:
	virtual void draw(){
		cls;
		rect();
		gotoxy(10, 2); cout << "              생성되어 있는 방                                                                                         현재 접속자들            \n";
		gotoxy(10, 3); cout << "                ( Room List)                                                                                           ( User List )            \n";
		gotoxy(0, 5); for (int i = 0; i < 78; i++)  cout << "▨";
		gotoxy(8, 10); cout << client.usernickname;
		gotoxy(0, 10); for (int i = 0; i < 58; i++)  cout << "▨";
		gotoxy(0, 15); for (int i = 0; i < 58; i++)  cout << "▨";
		gotoxy(0, 20); for (int i = 0; i < 58; i++)  cout << "▨";
		gotoxy(0, 25); for (int i = 0; i < 58; i++)  cout << "▨";
		gotoxy(0, 30); for (int i = 0; i < 78; i++) cout << "▨";
		for (int i = 0; i < 35; i++){
			gotoxy(114, i);
			cout << "▨";
		}
	};
	virtual void option(){
		int arrow = 0;
		gotoxy(120, 33); cout << "총 접속자 수 :";
		gotoxy(12, 6); cout << pclient->roomname;
		gotoxy(122, 6); cout << pclient->usernickname[0];
		gotoxy(122, 7); cout << pclient->usernickname[1];
		gotoxy(122, 8); cout << pclient->usernickname[2];
		gotoxy(122, 9); cout << pclient->usernickname[3];
		gotoxy(122, 10); cout << pclient->usernickname[4];
		gotoxy(138, 33); cout << pclient->serveraccesscount;
		while (true)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
				break;
			if (GetAsyncKeyState(VK_UP))
				arrow++;
			if (GetAsyncKeyState(VK_DOWN))
				arrow--;
			if (arrow > 4)
				arrow = 0;
			if (arrow < 0)
				arrow = 4;
			switch (arrow)
			{
			case 0:
				gotoxy(8, 6); cout << "→";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 1:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "→";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 2:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "→";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 3:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "→";
				gotoxy(8, 26); cout << "  ";
				break;
			case 4:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "→";
				break;
			default:
				break;
			}
			if (GetAsyncKeyState(VK_RETURN)){
				client.roomnumber = arrow;
				if (client.roomnumber == 0)
					client.scenenumber = 4;
				break;
			}
		}
	};
};
// *******************************************************************************************************************************************************************************************************

//서버 방접속화면
class Four_Scence :public Start{
public:
	virtual void draw(){
		rect();
		for (int i = 0; i < 35; i++){
			gotoxy(114, i);
			cout << "▨";
		}
		gotoxy(0, 9); for (int i = 0; i < 57; i++)  cout << "▨";
		gotoxy(0, 17); for (int i = 0; i < 57; i++)  cout << "▨";
		gotoxy(0, 26); for (int i = 0; i < 57; i++)  cout << "▨";
		gotoxy(114, 24); for (int i = 0; i < 21; i++) cout << "▨";

		gotoxy(118, 2); cout << "방 이름 : "<<pclient->roomname;
		gotoxy(118, 5); cout << "Server - 1.0 ver";
		gotoxy(118, 7); cout << "Client - 1.0 ver";
		gotoxy(118, 9); cout << "Game Source - Prototype";
		gotoxy(118, 11); cout << "Server & Client - Moon Jong Min";
		gotoxy(118, 13); cout << "Game & Bind - Hwamg In Tae";
		gotoxy(118, 18); cout << "Copyright ⓒ 2015 Developed";
		gotoxy(126, 20); cout << "by the team";
		gotoxy(118, 22); cout << "- Critical section All Right Reserued";

		gotoxy(118, 26); cout << "                              ■■";
		gotoxy(118, 27); cout << "  ■■■■        ■■■■    ■■";
		gotoxy(118, 28); cout << " ■              ■       ■  ■■";
		gotoxy(118, 29); cout << " ■    ■■■    ■       ■  ■■";
		gotoxy(118, 30); cout << " ■      ■      ■       ■      ";
		gotoxy(118, 31); cout << "  ■■■■        ■■■■    ■■";
		gotoxy(120, 33); cout << "방장이 F5 Key를 눌러야 시작됩니다 ";
	};
	virtual void option(){
		while (true){
			gotoxy(10, 3); cout << pclient->usernickname[0];
			if (GetAsyncKeyState(VK_F5)){
				gotoxy(20, 3); cout << pclient->roomrootaccess<<"  "<< pclient->roomminsize;
				if (pclient->roomrootaccess == true && pclient->roomminsize > 1){ // 테스이후 1이상으로 수정하여 방에 2명이상 있을경우로 수정할것. 
					pclient->scenenumber = 8;
					break;
				}
			}
		}
	};
};

// *******************************************************************************************************************************************************************************************************
//created by~ 화면
class Five_Scence :public Start{
public:
	virtual void draw(){
		system("mode con: cols=20 lines=20");
		cls;
		cout << "   - 만든 사람 -\n\n";
		cout << " 컴퓨터학부 \n 2014218089 황인태\n\n";
		cout << " 컴퓨터학부 \n 2014218030 문종민\n";
		gotoxy(0, 18); cout << " - Press Esc key -";
	};
	virtual void option(){
		while (true){
			if (GetAsyncKeyState(VK_ESCAPE)){
				Map_max_size;
				pclient->scenenumber = 1;
				break;
			}
		}
	};
};

// *******************************************************************************************************************************************************************************************************
//create info 화면
class Six_Scence :public Start{
public:
	virtual void draw(){
			cls;
			rect();
			gotoxy(40, 25);  cout << "▨▨▨▨▨▨▨▨-Creat Information- ▨▨▨▨▨▨▨▨▨▨▨";
			gotoxy(40, 26);  cout << "▨ ID       :                                           ▨";
			gotoxy(40, 27);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
			gotoxy(40, 28);  cout << "▨ password :                                           ▨";
			gotoxy(40, 29);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
			gotoxy(40, 30);  cout << "▨            생성[Enter]        취소[ESC]              ▨";
			gotoxy(40, 31);  cout << "▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨▨";
	};
	
	virtual void option(){
		gotoxy(58, 26); cin >> pclient->userid;
		gotoxy(64, 28); cin >> pclient->userpassword;
		pclient->scenenumber = 2;
	};
};


// *******************************************************************************************************************************************************************************************************

//싱글모드 화면
class  Seven_Scence :public Tetris{
public:
	virtual void draw(){
		cls;
		cout << "\n 싱글플레이 모드입니다.\n";
		cout << " 잠시 후 게임이 시작됩니다.";
		Sleep(3000);
		cls;
		for (int i = 0; i < Y_size + 2; i++){
			for (int j = 0; j < X_size + 2; j++){
				if (i == 0 || i == Y_size + 1 || j == 0 || j == X_size + 1)
					cout << "▨";
				else{
					cout << "  ";
				}
			}
			cout << "\n";
		}
		gotoxy(39, 3); cout << "Player : User";
		gotoxy(39, 5); cout << "Mode  : Single Mode";
		gotoxy(39, 6); cout << "Level : " << "    단계";
		gotoxy(39, 7); cout << "Score : ";
		gotoxy(39, 10); cout << "▨▨ Next Block ▨▨";
		for (int i = 11; i < 16; i++){
			gotoxy(39, i); cout << "▨                ▨";
		}
		gotoxy(39, 16); cout << "▨▨▨▨▨▨▨▨▨▨";
		gotoxy(39, 18); cout << "P = 일시정지";
		gotoxy(39, 18); cout << "Space bar = 블록 드랍";
		gotoxy(39, 20); cout << "   ↑     블록 변경";
		gotoxy(39, 21); cout << "←    →  좌우 이동";
		gotoxy(39, 22); cout << "   ↓     아래 이동";
		SKY_BLUE
			gotoxy(39, 27); cout << "재미있게 하세요~~"; ORIGINAL
	};

	virtual void option(){
		reset();
		while (true){
			if (newblock_on == 1)
				newblock(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성
			for (int i = 0; i < 10; i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
				inputkey(); //키입력확인 
				drawcheck(); //화면을 그림 
				Sleep(100); //게임속도조절
				if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false)
					Sleep(100); //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
				if (space_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
					space_on = 0;
					break;
				}
			}
			dropblock(); // 블록을 한칸 내림 
			drawcheck();
			if (gameovercheck == 1){
				gameover();
				pclient->scenenumber = 1;
				return;
			}
		}
	};

};

// *******************************************************************************************************************************************************************************************************

//서버 게임 화면
class Eight_Scence :public Tetris{
public:
	virtual void draw(){
		gotoxy(0, 0);
		rect();
		for (int i = 0; i < 35; i++){
			gotoxy(32, i);
			cout << "▨";
			gotoxy(64, i);
			cout << "▨";
			gotoxy(96, i);
			cout << "▨";
			gotoxy(128, i);
			cout << "▨";
		}
		gotoxy(0, 28); for (int j = 0; j < 65; j++)cout << "▨";

		gotoxy(6, 30); cout << "닉네임 :";
		gotoxy(6, 31); cout << "Level  :";
		gotoxy(6, 32); cout << "Score  :";

		gotoxy(36, 30); cout << "닉네임 :";
		gotoxy(36, 31); cout << "Level  :";
		gotoxy(36, 32); cout << "Score  :";

		gotoxy(68, 30); cout << "닉네임 :";
		gotoxy(68, 31); cout << "Level  :";
		gotoxy(68, 32); cout << "Score  :";

		GOLD
		gotoxy(100, 30); cout << "닉네임 :";
		gotoxy(100, 31); cout << "Level  :";
		gotoxy(100, 32); cout << "Score  :";
		ORIGINAL

		gotoxy(132, 3); cout << "현재시간 : ";
		gotoxy(132, 6); cout << "방이름 : ";
		gotoxy(132, 7); cout << "방번호 : ";
		gotoxy(132, 8); cout << "최대인원 : ";
		gotoxy(132, 9); cout << "비밀번호 : ";

		gotoxy(132, 11); cout << "P = 일시정지";
		gotoxy(132, 13); cout << "Space bar = 블록 드랍";
		gotoxy(132, 14); cout << "   ↑     블록 변경";
		gotoxy(132, 15); cout << "←    →  좌우 이동";
		gotoxy(132, 16); cout << "   ↓     아래 이동";

		gotoxy(132, 18); cout << "▨▨ Next Block ▨▨";
		for (int i = 19; i < 24; i++){
			gotoxy(132, i); cout << "▨                ▨";
		}
		gotoxy(132, 24); cout << "▨▨▨▨▨▨▨▨▨▨";

		gotoxy(132, 34); cout << "재미있게 하세요~~";

	};
	virtual void option(){
		
		gotoxy(142, 3); cout << "??시??분";
		gotoxy(142, 6); cout << client.roomname;
		gotoxy(142, 7); cout << client.roomnumber;
		gotoxy(142, 8); cout << client.roommaxsize;
		gotoxy(142, 9); cout << client.roompassword;
		
		//Player 자기의 배열판위치
		for (int i = 0; i < Y_size; i++){
			for (int j = 0; j < X_size; j++){
				gotoxy(((X_size * 7) - 7) + (j * 2), i + 1);
				reset();
				while (true){
					if (newblock_on == 1)
						newblock(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성
					for (int i = 0; i < 10; i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
						inputkey(); //키입력확인 
						drawcheck(); //화면을 그림 
						Sleep(100); //게임속도조절
						if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false)
							Sleep(100); //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
						if (space_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
							space_on = 0;
							break;
						}
					}
					dropblock(); // 블록을 한칸 내림 
					drawcheck();
					if (gameovercheck == 1){
						gameover();
						pclient->scenenumber = 4;
						return;
					}
				}
			}
		}
	};
};

Game game; // Game class 객체생성