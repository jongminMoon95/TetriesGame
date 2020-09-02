/*
�� �ҽ��� ���α׷������� ȭ���� ��°� ������� �Է¿� ���� ó���� �ϴ� Class�� �����ϱ� ����
*/
// X�� �� 80�� Y�� 36��
#include"Client.h"
#include"Summative.h"
#include<windows.h>
#include<conio.h>

//------------------------------------------��Ʈ������ ���Ǵ� ����------------------------------------------------------------
#define LEFT 75 //�·� �̵� 
#define RIGHT 77 //��� �̵� 
#define UP 72 //ȸ�� 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define ESC 27 //��������
#define ACTIVE_BLOCK 1 // �����ǹ迭�� ����� ������ �� �ִ� ��� 
#define EMPTY 0         // ��� �ִ� ����
#define INACTIVE_BLOCK 2 // ���� �̵��� ��������.
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

//�簢�� �׸��� �Լ�
void rect(){
	cls;
	gotoxy(0, 0);
	for (int i = 0; i < 36; i++){
		if (i == 0 || i == 35)
			for (int j = 0; j < 78; j++){
				cout << "��";
			}
		cout << "��";
		gotoxy(156, i);;
		cout << "��\n";
	}
};
//��ü���� �⺻Ʋ���� Draw ���Ŭ����
class Start :public Game{
public:
	virtual void draw() = 0;
	virtual void option() = 0;
};
// *******************************************************************************************************************************************************************************************************
//��Ʈ�������� �⺻Ʋ���� Draw ���Ŭ����
class Tetris :public Game{
protected:
	int space_on; //spaceŰ�� üũ�ϱ����� ����
	int crush_on; //���� �ٴڿ� ��Ҵ��� üũ�ϱ����� ����
	int newblock_on; //���ο� ���� �ʿ��� üũ
	int gameovercheck; // ���ӿ��� üũ
	int b_type; //��� ������ ���� 
	int b_rotation; //��� ȸ���� ���� 
	int b_type_next; //���� ��ϰ� ����
	int bx; //�̵����� ���� X��ǥ���� ��ġ�� ���ϱ����� �ʿ��� ����
	int by; //�̵����� ���� Y��ǥ���� ��ġ�� ���ϱ����� �ʿ��� ����
	int key; // ������� Ű�Է°��� �޾� �����ϱ����� ����
	int copyarray[Y_size][X_size]; // �̱��÷��� �Ǵ� ��Ƽ���� �ڱ��ڽ��� �������� �����ϱ����� �迭 �������� �����ϱ� ���� copy������ ���.
	int copyonearray[Y_size][X_size]; //��Ƽ�÷��̿��� ����� �÷��̾�1�� ������������ �����ϱ� ���� copy������ ���.
	int copytwoarray[Y_size][X_size]; //��Ƽ�÷��̿��� ����� �÷��̾�2�� ���� �������� �����ϱ� ���� copy������ ���.
	int copythreearray[Y_size][X_size]; //��Ƽ�÷��̿��� ����� �÷��̾�3�� ���� �������� �����ϱ� ���� copy������ ���.

	void inputkey(); // ������� Ű�Է�
	bool checkcrush(int bx, int by, int b_rotation); // ���� �浹�� �ִ��� �̸��˻�.
	void moveblock(int key); // �Է°��� ���� �� ������.
	void newblock(); // ���ο� �� ����.
	void dropblock(); // ���� Y�� ��ĭ�̵�.
	void checkline(); // �� ���� üũ.
	void gameover(); // ���ӿ��� üũ
	void info();
	void drawcheck(); // �������� �����ϱ����� copy�� ��

public:
	virtual void draw() = 0;
	virtual void option() = 0;
	void reset(){ // �����ڿ� �����ϰ� �ʱ�ȭ�� �����ϴ� �޼ҵ�
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

static int blocks[7][4][4][4] = { //[ ���� ���� ][ ���� ���º���][�� x��][ �� y��]
	{ { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } } }, // �׸��
	{ { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 } }, { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 } }, { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // ���ں�
	{ { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } } }, // z�� ��
	{ { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // z�� ��Ī��
	{ { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 1, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 } } }, // �� ���� ��
	{ { { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 1, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } } }, // �� �� ��Ī��
	{ { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 } },
	{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 1, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 } } } };  // ���� ��

void Tetris::inputkey(){// ������� Ű�Է�
	key = 0;

	if (_kbhit()){ //Ű�Է��� �ִ� ���  
		key = _getch(); //Ű���� ����
		if (key == 224){ //����Ű�ΰ�� 
			do{ key = _getch(); } while (key == 224);//����Ű���ð��� ���� 
			switch (key){
			case LEFT: //����Ű ��������  
				if (checkcrush(bx - 1, by, b_rotation) == true)
					moveblock(LEFT);
				break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
			case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
				if (checkcrush(bx + 1, by, b_rotation) == true)
					moveblock(RIGHT);
				break;
			case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
				if (checkcrush(bx, by + 1, b_rotation) == true)
					moveblock(DOWN);
				break;
			case UP: //���� ����Ű �������� 
				if (checkcrush(bx, by, (b_rotation + 1) % 4) == true)
					moveblock(UP);//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
				else if (crush_on == 1 && checkcrush(bx, by - 1, (b_rotation + 1) % 4) == true)
					moveblock(777);//�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
			}
		}
		else{ //����Ű�� �ƴѰ�� 
			switch (key){
			case SPACE: //�����̽�Ű �������� 
				space_on = 1; //�����̽�Ű üũ 
				while (crush_on == 0){ //�ٴڿ� ���������� �̵���Ŵ 
					dropblock();
					pclient->score += 1; // drop ���ʽ� 1���� space���� �������κ��� �߰�
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

//���� �浹�� �ִ��� �̸��˻�. ���� ���⼭ ������!! if������� ���۵���
bool Tetris::checkcrush(int bx, int by, int b_rotation){ //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 

	for (int i = 0; i<4; i++){
		for (int j = 0; j < 4; j++){ //������ ��ġ�� �����ǰ� ������� ���ؼ� ��ġ�� false�� ���� 
			if (blocks[b_type][b_rotation][i][j] == ACTIVE_BLOCK && pclient->drawarray[by + i][bx + j + ((X_size / 2) - 2)] == INACTIVE_BLOCK)
				return false;
			if (by + i < 1 || by + i >Y_size || bx + j + ((X_size / 2) - 2) < -1 || bx + j + ((X_size / 2) - 2) > X_size + 1){
				crush_on = 1;
				return false;
			}
		}
	}
	return true; //�ϳ��� �Ȱ�ġ�� true���� 
}

void Tetris::moveblock(int key){// �Է°��� ���� �� ������.
	switch (key){
	case LEFT: //���ʹ��� 
		for (int i = 0; i < 4; i++){ //������ǥ�� ���� ���� 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		for (int i = 0; i < 4; i++){ //�������� ��ĭ���� active block�� ���� 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--; //��ǥ�� �̵� 
		break;

	case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
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

	case DOWN:    //�Ʒ��� ����. ���ʹ����̶� ���� ������ ����
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

	case UP: //Ű���� ���� �������� ȸ����Ŵ. 
		for (int i = 0; i < 4; i++){ //������ǥ�� ���� ����  
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //ȸ������ 1������Ű�� ���������� 4���̹Ƿ� 4���� Ŭ��� 0���� �ٽõ�����
		for (int i = 0; i < 4; i++){ //ȸ���� ����� ���� 
			for (int j = (X_size / 2) - 2; j < (X_size / 2) + 2; j++){
				if (blocks[b_type][b_rotation][i][j - ((X_size / 2) - 2)] == ACTIVE_BLOCK) pclient->drawarray[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 777: //����� �ٴ�, Ȥ�� �ٸ� ��ϰ� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
		//�̸� ���۽�Ű�� Ư������ 
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
		b_type = b_type_next; //���������� ������ 
		b_type_next = rand() % 7; //���� ���� ����
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
						switch (b_type){ //���� ������ ���������� ����.
						case 0:RED break;
						case 1:BLUE break;
						case 2:YELLOW break;
						case 3:PLUM break;
						case 4:SKY_BLUE break;
						case 5:WHITE break;
						case 6:GREEN break;
						}
						gotoxy((j * 2) + 2, i + 1); cout << "��";
						ORIGINAL
					}
					if (pclient->drawarray[i][j] == EMPTY){
						gotoxy((j * 2) + 2, i + 1); cout << "  ";
					}
					if (pclient->drawarray[i][j] == INACTIVE_BLOCK){
						gotoxy((j * 2) + 2, i + 1); cout << "��";
					}
					copyarray[i][j] = pclient->drawarray[i][j];
				}
			}

			if (pclient->scenenumber == 8)
			{
				//�ڱ� ���� ��
				if (copyarray[i][j] != pclient->drawarray[i][j]){
					if (pclient->drawarray[i][j] == ACTIVE_BLOCK){
						switch (b_type){ //���� ������ ���������� ����.
						case 0:RED break;
						case 1:BLUE break;
						case 2:YELLOW break;
						case 3:PLUM break;
						case 4:SKY_BLUE break;
						case 5:WHITE break;
						case 6:GREEN break;
						}
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "��";
						ORIGINAL
					}
					if (pclient->drawarray[i][j] == EMPTY){
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->drawarray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 7) - 7) + (j * 2), i + 1); cout << "��";
					}
					copyarray[i][j] = pclient->drawarray[i][j];
				}
				//player1 ��
				if (copyonearray[i][j] != pclient->playeronearray[i][j]){
					ORIGINAL
					if (pclient->playeronearray[i][j] == ACTIVE_BLOCK){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "��";
					}
					if (pclient->playeronearray[i][j] == EMPTY){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "  ";
					}
					if (pclient->playeronearray[i][j] == INACTIVE_BLOCK){
						gotoxy(((j * 2) + 2, i + 1), i + 1); cout << "��";
					}
					copyonearray[i][j] = pclient->playeronearray[i][j];
				}

				//player2 ��
				if (copytwoarray[i][j] != pclient->playertwoarray[i][j]){
					ORIGINAL
					if (pclient->playertwoarray[i][j] == ACTIVE_BLOCK){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "��";
					}
					if (pclient->playertwoarray[i][j] == EMPTY){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->playertwoarray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 3) - 11) + (j * 2), i + 1); cout << "��";
					}
					copytwoarray[i][j] = pclient->playertwoarray[i][j];
				}
				//player3 ��
				if (copythreearray[i][j] != pclient->playerthreearray[i][j]){
					ORIGINAL
					if (pclient->playerthreearray[i][j] == ACTIVE_BLOCK){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "��";
					}
					if (pclient->playerthreearray[i][j] == EMPTY){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "  ";
					}
					if (pclient->playerthreearray[i][j] == INACTIVE_BLOCK){
						gotoxy(((X_size * 5) - 9) + (j * 2), i + 1); cout << "��";
					}
					copythreearray[i][j] = pclient->playerthreearray[i][j];
				}
			}
		}
	}
}

//�ٿϼ��� ���������λ����Ǵ��� �ذ��Ұ�.
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
			pclient->score += 50; // ����Ŭ����. 50�� �߰�
		}
		checkline = 0;
	}
	info();
}

void Tetris::dropblock(){

	if (crush_on&&checkcrush(bx, by + 1, b_rotation) == true) crush_on = 0; //���� ��������� crush flag �� 
	if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false){ //���� ��������ʰ� crush flag�� ����������
		for (int i = 0; i < Y_size; i++){ //���� �������� ���� ���� 
			for (int j = 0; j < X_size; j++){
				if (pclient->drawarray[i][j] == ACTIVE_BLOCK) pclient->drawarray[i][j] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0; //flag�� �� 
		checkline(); //����üũ�� �� 
		newblock_on = 1; //���ο� ������ flag�� ��    
		return; //�Լ� ���� 
	}
	if (checkcrush(bx, by + 1, b_rotation) == true) moveblock(DOWN); //���� ��������� ������ ��ĭ �̵� 
	if (checkcrush(bx, by + 1, b_rotation) == false) crush_on = 1; //������ �̵��� �ȵǸ�  crush flag�� ��
}

void Tetris::gameover(){
	int esccheck = 0;
	if (pclient->scenenumber == 7){
		gotoxy(2, 10); cout << "������������������������������";
		gotoxy(2, 11); cout << "�� �� Game Over!! ��        ��";
		gotoxy(2, 12); cout << "�������� " << pclient->score << "      �� �Դϴ�!��";
		gotoxy(2, 13); cout << "��--------------------------��";
		gotoxy(2, 14); cout << "�� �����Ͻ÷���             ��";
		gotoxy(2, 15); cout << "��   ESCŰ�� ��������!      ��";
		gotoxy(2, 16); cout << "������������������������������";
	}
	if (pclient->scenenumber == 8){
		gotoxy(98, 10); cout << "������������������������������";
		gotoxy(98, 11); cout << "�� �� Game Over!! ��        ��";
		gotoxy(98, 12); cout << "�������� " << pclient->score << "      �� �Դϴ�!��";
		gotoxy(98, 13); cout << "��--------------------------��";
		gotoxy(98, 14); cout << "�� �����Ͻ÷���             ��";
		gotoxy(98, 15); cout << "��   ESCŰ�� ��������!      ��";
		gotoxy(98, 16); cout << "������������������������������";
	}
	while (true)
	{
			esccheck = _getch(); //Ű���� ����
			if (esccheck == 27) //escŰ�� �����ÿ� ����
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
				switch (b_type_next){ //���� ������ ���������� ����.
				case 0:RED break;
				case 1:BLUE break;
				case 2:YELLOW break;
				case 3:PLUM break;
				case 4:SKY_BLUE break;
				case 5:WHITE break;
				case 6:GREEN break;
				}
				if (blocks[b_type_next][0][i][j] == 1)
					cout << "��";
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
				switch (b_type_next){ //���� ������ ���������� ����.
				case 0:RED break;
				case 1:BLUE break;
				case 2:YELLOW break;
				case 3:PLUM break;
				case 4:SKY_BLUE break;
				case 5:WHITE break;
				case 6:GREEN break;
				}
				if (blocks[b_type_next][0][i][j] == 1)
					cout << "��";
				else
					cout << "  ";
			}
		}
		ORIGINAL
	}
}


// ******************************************************************************************************************************************************************************************************
//ĳġ���ε� �⺻Ʋ���� Draw ���Ŭ���� - ���� �̱���
class CatchMind :public Game{
public:
	virtual void draw() = 0;
	virtual void option() = 0;
};

// *******************************************************************************************************************************************************************************************************
//����ȭ��
class  One_Scence:public Start{
public:
		virtual void draw(){
			rect();

			BLUE;
			gotoxy(25, 5); cout << "��        ��  ����  ��     ��    ����  ";
			gotoxy(25, 6); cout << "���    ���    ��    ���   ��      ��    ";
			gotoxy(25, 7); cout << "��  ���  ��    ��    �� ��  ��      ��    ";
			gotoxy(25, 8); cout << "��        ��    ��    ��   ���      ��    ";
			gotoxy(25, 9); cout << "��        ��  ����  ��     ��    ����  ";
			GREEN;
			gotoxy(75, 5); cout << "  �����         ��     ��  �� ��  ������  ";//GAME
			gotoxy(75, 6); cout << " ��              ��  ��   ��� ���  ��          ";
			gotoxy(75, 7); cout << " ��    ����   �����  ��     ��  ������  ";
			gotoxy(75, 8); cout << " ��      ��     ��    ��  ��     ��  ��          ";
			gotoxy(75, 9); cout << "  �����      ��    ��  ��     ��  ������  ";
			ORIGINAL;
			gotoxy(58, 19); cout << "���Ͻô� �÷��̸� �������ּ���!!";
			gotoxy(60, 21); cout << "1. �̱� �÷���< Single Type Play >";
			gotoxy(60, 23); cout << "2. ��Ƽ �÷���< Multi Type Play >";
			gotoxy(60, 25); cout << "3. ���� ���  < Created by ~ >";
			gotoxy(59, 26); cout << "������������������������������������";
			gotoxy(59, 27); cout << "��������Ʒ� �̵� Space Key = ���æ�";
			gotoxy(59, 28); cout << "������������������������������������";
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
					gotoxy(57, 21); cout << "��";
					startoptionactive = false;
					break;

				case 5:
					gotoxy(57, 21); cout << " ";
					gotoxy(57, 25); cout << " ";
					gotoxy(57, 23); cout << "��";
					startoptionactive = false;
					break;

				case 6:
					gotoxy(57, 21); cout << " ";
					gotoxy(57, 23); cout << " ";
					gotoxy(57, 25); cout << "��";
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

//���� �α��� ȭ��
class Two_Scence :public Start
{
public:
	virtual void draw(){
		rect();
		gotoxy(50, 23);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
		gotoxy(50, 24);  cout << "��- ������ ���� ��� ID�� Password�� new�� �Է����ּ���.��";
		gotoxy(50, 25);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
		gotoxy(50, 26);  cout << "�� ID :                                                 ��";
		gotoxy(50, 27);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
		gotoxy(50, 28);  cout << "�� Password :                                           ��";
		gotoxy(50, 29);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
		gotoxy(50, 30);  cout << "��          �α���[Enter]        ���[ESC]              ��";
		gotoxy(50, 31);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";

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
			if (GetAsyncKeyState(VK_RETURN) && pclient->serveraccess == true){ //����Ű
				pclient->scenenumber = 3;
				break;
			}
			if (GetAsyncKeyState(VK_ESCAPE)){ // ESCŰ
				pclient->scenenumber = 1;
				break;
			}
		}
	};
};

// *******************************************************************************************************************************************************************************************************

//���� ����ȭ��
class Three_Scence :public Start{
public:
	virtual void draw(){
		cls;
		rect();
		gotoxy(10, 2); cout << "              �����Ǿ� �ִ� ��                                                                                         ���� �����ڵ�            \n";
		gotoxy(10, 3); cout << "                ( Room List)                                                                                           ( User List )            \n";
		gotoxy(0, 5); for (int i = 0; i < 78; i++)  cout << "��";
		gotoxy(8, 10); cout << client.usernickname;
		gotoxy(0, 10); for (int i = 0; i < 58; i++)  cout << "��";
		gotoxy(0, 15); for (int i = 0; i < 58; i++)  cout << "��";
		gotoxy(0, 20); for (int i = 0; i < 58; i++)  cout << "��";
		gotoxy(0, 25); for (int i = 0; i < 58; i++)  cout << "��";
		gotoxy(0, 30); for (int i = 0; i < 78; i++) cout << "��";
		for (int i = 0; i < 35; i++){
			gotoxy(114, i);
			cout << "��";
		}
	};
	virtual void option(){
		int arrow = 0;
		gotoxy(120, 33); cout << "�� ������ �� :";
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
				gotoxy(8, 6); cout << "��";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 1:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "��";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 2:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "��";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "  ";
				break;
			case 3:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "��";
				gotoxy(8, 26); cout << "  ";
				break;
			case 4:
				gotoxy(8, 6); cout << "  ";
				gotoxy(8, 11); cout << "  ";
				gotoxy(8, 16); cout << "  ";
				gotoxy(8, 21); cout << "  ";
				gotoxy(8, 26); cout << "��";
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

//���� ������ȭ��
class Four_Scence :public Start{
public:
	virtual void draw(){
		rect();
		for (int i = 0; i < 35; i++){
			gotoxy(114, i);
			cout << "��";
		}
		gotoxy(0, 9); for (int i = 0; i < 57; i++)  cout << "��";
		gotoxy(0, 17); for (int i = 0; i < 57; i++)  cout << "��";
		gotoxy(0, 26); for (int i = 0; i < 57; i++)  cout << "��";
		gotoxy(114, 24); for (int i = 0; i < 21; i++) cout << "��";

		gotoxy(118, 2); cout << "�� �̸� : "<<pclient->roomname;
		gotoxy(118, 5); cout << "Server - 1.0 ver";
		gotoxy(118, 7); cout << "Client - 1.0 ver";
		gotoxy(118, 9); cout << "Game Source - Prototype";
		gotoxy(118, 11); cout << "Server & Client - Moon Jong Min";
		gotoxy(118, 13); cout << "Game & Bind - Hwamg In Tae";
		gotoxy(118, 18); cout << "Copyright �� 2015 Developed";
		gotoxy(126, 20); cout << "by the team";
		gotoxy(118, 22); cout << "- Critical section All Right Reserued";

		gotoxy(118, 26); cout << "                              ���";
		gotoxy(118, 27); cout << "  �����        �����    ���";
		gotoxy(118, 28); cout << " ��              ��       ��  ���";
		gotoxy(118, 29); cout << " ��    ����    ��       ��  ���";
		gotoxy(118, 30); cout << " ��      ��      ��       ��      ";
		gotoxy(118, 31); cout << "  �����        �����    ���";
		gotoxy(120, 33); cout << "������ F5 Key�� ������ ���۵˴ϴ� ";
	};
	virtual void option(){
		while (true){
			gotoxy(10, 3); cout << pclient->usernickname[0];
			if (GetAsyncKeyState(VK_F5)){
				gotoxy(20, 3); cout << pclient->roomrootaccess<<"  "<< pclient->roomminsize;
				if (pclient->roomrootaccess == true && pclient->roomminsize > 1){ // �׽����� 1�̻����� �����Ͽ� �濡 2���̻� �������� �����Ұ�. 
					pclient->scenenumber = 8;
					break;
				}
			}
		}
	};
};

// *******************************************************************************************************************************************************************************************************
//created by~ ȭ��
class Five_Scence :public Start{
public:
	virtual void draw(){
		system("mode con: cols=20 lines=20");
		cls;
		cout << "   - ���� ��� -\n\n";
		cout << " ��ǻ���к� \n 2014218089 Ȳ����\n\n";
		cout << " ��ǻ���к� \n 2014218030 ������\n";
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
//create info ȭ��
class Six_Scence :public Start{
public:
	virtual void draw(){
			cls;
			rect();
			gotoxy(40, 25);  cout << "�ɢɢɢɢɢɢɢ�-Creat Information- �ɢɢɢɢɢɢɢɢɢɢ�";
			gotoxy(40, 26);  cout << "�� ID       :                                           ��";
			gotoxy(40, 27);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
			gotoxy(40, 28);  cout << "�� password :                                           ��";
			gotoxy(40, 29);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
			gotoxy(40, 30);  cout << "��            ����[Enter]        ���[ESC]              ��";
			gotoxy(40, 31);  cout << "�ɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢɢ�";
	};
	
	virtual void option(){
		gotoxy(58, 26); cin >> pclient->userid;
		gotoxy(64, 28); cin >> pclient->userpassword;
		pclient->scenenumber = 2;
	};
};


// *******************************************************************************************************************************************************************************************************

//�̱۸�� ȭ��
class  Seven_Scence :public Tetris{
public:
	virtual void draw(){
		cls;
		cout << "\n �̱��÷��� ����Դϴ�.\n";
		cout << " ��� �� ������ ���۵˴ϴ�.";
		Sleep(3000);
		cls;
		for (int i = 0; i < Y_size + 2; i++){
			for (int j = 0; j < X_size + 2; j++){
				if (i == 0 || i == Y_size + 1 || j == 0 || j == X_size + 1)
					cout << "��";
				else{
					cout << "  ";
				}
			}
			cout << "\n";
		}
		gotoxy(39, 3); cout << "Player : User";
		gotoxy(39, 5); cout << "Mode  : Single Mode";
		gotoxy(39, 6); cout << "Level : " << "    �ܰ�";
		gotoxy(39, 7); cout << "Score : ";
		gotoxy(39, 10); cout << "�ɢ� Next Block �ɢ�";
		for (int i = 11; i < 16; i++){
			gotoxy(39, i); cout << "��                ��";
		}
		gotoxy(39, 16); cout << "�ɢɢɢɢɢɢɢɢɢ�";
		gotoxy(39, 18); cout << "P = �Ͻ�����";
		gotoxy(39, 18); cout << "Space bar = ��� ���";
		gotoxy(39, 20); cout << "   ��     ��� ����";
		gotoxy(39, 21); cout << "��    ��  �¿� �̵�";
		gotoxy(39, 22); cout << "   ��     �Ʒ� �̵�";
		SKY_BLUE
			gotoxy(39, 27); cout << "����ְ� �ϼ���~~"; ORIGINAL
	};

	virtual void option(){
		reset();
		while (true){
			if (newblock_on == 1)
				newblock(); // �� �� flag�� �ִ� ��� ���ο� �� ����
			for (int i = 0; i < 10; i++){ //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
				inputkey(); //Ű�Է�Ȯ�� 
				drawcheck(); //ȭ���� �׸� 
				Sleep(100); //���Ӽӵ�����
				if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false)
					Sleep(100); //����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
				if (space_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
					space_on = 0;
					break;
				}
			}
			dropblock(); // ����� ��ĭ ���� 
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

//���� ���� ȭ��
class Eight_Scence :public Tetris{
public:
	virtual void draw(){
		gotoxy(0, 0);
		rect();
		for (int i = 0; i < 35; i++){
			gotoxy(32, i);
			cout << "��";
			gotoxy(64, i);
			cout << "��";
			gotoxy(96, i);
			cout << "��";
			gotoxy(128, i);
			cout << "��";
		}
		gotoxy(0, 28); for (int j = 0; j < 65; j++)cout << "��";

		gotoxy(6, 30); cout << "�г��� :";
		gotoxy(6, 31); cout << "Level  :";
		gotoxy(6, 32); cout << "Score  :";

		gotoxy(36, 30); cout << "�г��� :";
		gotoxy(36, 31); cout << "Level  :";
		gotoxy(36, 32); cout << "Score  :";

		gotoxy(68, 30); cout << "�г��� :";
		gotoxy(68, 31); cout << "Level  :";
		gotoxy(68, 32); cout << "Score  :";

		GOLD
		gotoxy(100, 30); cout << "�г��� :";
		gotoxy(100, 31); cout << "Level  :";
		gotoxy(100, 32); cout << "Score  :";
		ORIGINAL

		gotoxy(132, 3); cout << "����ð� : ";
		gotoxy(132, 6); cout << "���̸� : ";
		gotoxy(132, 7); cout << "���ȣ : ";
		gotoxy(132, 8); cout << "�ִ��ο� : ";
		gotoxy(132, 9); cout << "��й�ȣ : ";

		gotoxy(132, 11); cout << "P = �Ͻ�����";
		gotoxy(132, 13); cout << "Space bar = ��� ���";
		gotoxy(132, 14); cout << "   ��     ��� ����";
		gotoxy(132, 15); cout << "��    ��  �¿� �̵�";
		gotoxy(132, 16); cout << "   ��     �Ʒ� �̵�";

		gotoxy(132, 18); cout << "�ɢ� Next Block �ɢ�";
		for (int i = 19; i < 24; i++){
			gotoxy(132, i); cout << "��                ��";
		}
		gotoxy(132, 24); cout << "�ɢɢɢɢɢɢɢɢɢ�";

		gotoxy(132, 34); cout << "����ְ� �ϼ���~~";

	};
	virtual void option(){
		
		gotoxy(142, 3); cout << "??��??��";
		gotoxy(142, 6); cout << client.roomname;
		gotoxy(142, 7); cout << client.roomnumber;
		gotoxy(142, 8); cout << client.roommaxsize;
		gotoxy(142, 9); cout << client.roompassword;
		
		//Player �ڱ��� �迭����ġ
		for (int i = 0; i < Y_size; i++){
			for (int j = 0; j < X_size; j++){
				gotoxy(((X_size * 7) - 7) + (j * 2), i + 1);
				reset();
				while (true){
					if (newblock_on == 1)
						newblock(); // �� �� flag�� �ִ� ��� ���ο� �� ����
					for (int i = 0; i < 10; i++){ //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
						inputkey(); //Ű�Է�Ȯ�� 
						drawcheck(); //ȭ���� �׸� 
						Sleep(100); //���Ӽӵ�����
						if (crush_on&&checkcrush(bx, by + 1, b_rotation) == false)
							Sleep(100); //����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
						if (space_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
							space_on = 0;
							break;
						}
					}
					dropblock(); // ����� ��ĭ ���� 
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

Game game; // Game class ��ü����