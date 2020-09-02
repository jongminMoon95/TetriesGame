/*
********************************************************* Object_Oriented Programing _ Team Project *********************************************************

�� �ҽ��� �ֻ��� Class�� Client�� �����ϱ� ����.

�� �ڵ� �ۼ��� ���ǻ���
1. Class �� ��츸 ù ���ڸ� �빮�ڷ� �����ϸ�, �� ���� ��� �̸��� �ҹ��ڷ� ����� ��
2. Method�� ������ ��� �ش� Method�� ��� ����� �۵��ϴ��� �����ϰ� �ּ�ó�� �� ��.
3. goto���� ��� �ʿ��� ��쿡�� ����ϸ�, ����� ��� �ش� Label���� �ּ�ó�� �Ұ�.
4. ���α׷� ������ ���Ǵ� ��� data�� ������ ����ȭ�� ��.
5. Method �� ���� ���� ������ ��� ��� �뵵�� ��ɿ� ���õ� �̸����� ������ ��.
6. ���������� ���Ǵ� �ֻ��� Client������ ������ �ʿ��� ��� �ǳ��ϰ� ������ ��.
7. ǥ�� ���̺귯���ܿ��� ����� ������� ���� ��.
8. Method�� ��쿡�� �ܾ��� �������� _�� ����� ��. Data�� ���� ������� �ʴ´�.
   ex) Method - void roomsize() �� void room_size() �������� ����� ��.
9. Class���� �Ҹ��ڴ� ������� �ʴ´�. �ʿ��� ��� ������ Method�� �����Ͽ� ����Ѵ�.
   ��, �����ڴ� ����.
10. Thread �� ��� Main�� �ִ� �ҽ����� �����ϵ��� �Ѵ�.
************************************************************************************************************************************************************
*/

/*
	�� scenenumber�� ����Ű�� ������ ȭ�� - �߰� �� ������ �����Ұ�.
	1. ����ȭ�� - �̱�,��Ƽ,Creatdit ����ȭ��
	2. ��Ƽ�÷��� �α��� ȭ��
	3. ��Ƽ�÷��� ����ȭ��
	4. ������ ���ȭ��
	5. Creatdit ȭ��
	6. ���̵� ����
	7. ��Ʈ���� �̱��÷��� ���ȭ��
	8. ��Ʈ���� ��Ƽ�÷��� ��� ȭ��
*/

#include<iostream>
#include<string>
#include<time.h>

using namespace std;

#define X_size 15
#define Y_size 27

// Client Class - ID,��й�ȣ,�� �̸�,����ȭ��,�� �ִ��ο�,�� ��й�ȣ, ���������ο�, User �г��� ���� Public Data����.
class Client
{
public:
	int scenenumber; // ���� ȭ���� Scene�� üũ�ϱ����� ����.
	string userid; // User ID�� �����ϱ� ���� ����.
	string userpassword; // User Password�� �����ϱ� ���� ����.
	string usernickname[5]; // User Nickname�� �����ϱ� ���� ����.
	string roomname[5]; // ���� �̸��� �����ϱ� ���� ����
	int roomminsize; // �濡 ������ �ο��� üũ
	int roommaxsize; // �� �ִ� �ο� �����ϱ� ���� ����.
	int roompassword; // �濡 �ɸ� Password�� �����ϱ� ���� ����
	bool roomrootaccess; // �濡�� �������� �ƴ����� Ȯ���ϱ� ���� ����
	bool serveraccess;
	int score; // ��������
	int scoreone; // player 1�� ����
	int scoretwo; // player 2�� ����
	int scorethree; // player 3�� ����
	int roomnumber;
	unsigned long int serveraccesscount; // ���� ���� �ο� üũ!
	int drawarray[Y_size][X_size]; // ���ӳ����� ���� �ִ� ���� �׸��� ���� �迭
	int playeronearray[Y_size][X_size]; // �÷��̾� 1�� ������ �������
	int playertwoarray[Y_size][X_size]; // �÷��̾� 2�� ������ �������
	int playerthreearray[Y_size][X_size]; // �÷��̾� 3�� ������ �������

	bool startcheck;

	void format(); // �����ڿ� �Ȱ��� ��Ȱ�� �ϴ� �޼ҵ�
	Client();	//Data�� �ʱ�ȭ�ϱ� ���� ������.
};

Client::Client(){	//Data�� �ʱ�ȭ�ϱ� ���� ������.
	scenenumber = 1;
	for (int i = Y_size; i > 0; i--){
		for (int j = X_size; j > 0; j--){
			drawarray[i - 1][j - 1] = 0;
			playeronearray[i - 1][j - 1] = 0;
			playertwoarray[i - 1][j - 1] = 0;
			playerthreearray[i - 1][j - 1] = 0;
		}
	}//drawmap�� ��絥���Ϳ� 0���� �ʱ�ȭ�ϱ�����.
	for (int i = 0; i < 5; i++)
	{
		usernickname[i] = " ";
		roomname[i] = " ";
	}

	userid = " ";
	userpassword = " ";
	roommaxsize = 0;
	roompassword = 0;
	roomrootaccess = false;
	serveraccess = false;
	serveraccesscount = 0;
	roomnumber = -1;
	score = 0;
	scoreone = 0;
	scoretwo = 0;
	scorethree = 0;
	startcheck = false;
};

void Client::format(){
	scenenumber = 1;
	for (int i = 0; i < 5; i++)
	{
		usernickname[i] = " ";
		roomname[i] = " ";
	}

	for (int i = Y_size; i > 0; i--){
		for (int j = X_size; j > 0; j--){
			drawarray[i - 1][j - 1] = 0;
		}
	}//drawmap�� ��絥���Ϳ� 0���� �ʱ�ȭ�ϱ�����.
	userid = " ";
	userpassword = " ";
	roommaxsize = 0;
	roompassword = 0;
	roomrootaccess = false;
	serveraccesscount = 0;
}

Client client; // Client ��ü����.