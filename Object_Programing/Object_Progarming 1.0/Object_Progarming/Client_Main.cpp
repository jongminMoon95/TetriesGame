/*
	********************************************************* Object_Oriented Programing _ Team Project *********************************************************

	������Ʈ�� : c++�� ����� ����
	���� : Ver 0.0.1
	���������� ������ ��¥ : 2015 - 10 - 31

	�� �� �ҽ����Ͽ��� Thread�� Class �ܿ��� ���� �� �ۼ����� �ʴ´�.

	************************************************************************************************************************************************************
*/

//#include"Game.h"
#include"Sub_Server.h"
#include"Sound.h"


HANDLE frist = CreateThread(NULL, 0, soundoption, NULL, NULL, NULL);


//�ؾߵ��� ��Ʈ���� �̱۸�� �˰��� �߰��ϱ�
void main()
{
	t.SettingConnect();

	_beginthread(ClientThread, NULL, NULL);
Promiss:
	Start *start;
	Tetris *tetris;
	switch (game.pclient->scenenumber){
	case 1:
		start = new One_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 2:
		start = new Two_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 3:
		start = new Three_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 4:
		start = new Four_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 5:
		start = new Five_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 6:
		start = new Six_Scence;
		start->draw();
		start->option();
		delete start;
		break;

	case 7:
		tetris = new Seven_Scence;
		tetris->draw();
		tetris->option();
		delete tetris;
		break;

	case 8:
		tetris = new Eight_Scence;
		tetris->draw();
		tetris->option();
		delete tetris;
		break;

	default:
		client.format();
		break;
	}
	goto Promiss;
}