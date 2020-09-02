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
	6. ��Ʈ���� �̱��÷��� ���ȭ��
	7. ��Ʈ���� ��Ƽ�÷��� ��� ȭ��
	8.
*/

#include<iostream>
#include<string>
#include<time.h>

using namespace std;

#define X_size 27
#define Y_size 15

// Client Class - ID,��й�ȣ,�� �̸�,����ȭ��,�� �ִ��ο�,�� ��й�ȣ, ���������ο�, User �г��� ���� Public Data����.
class Client
{
public:
	int scenenumber; // ���� ȭ���� Scene�� üũ�ϱ����� ����.
	string userid; // User ID�� �����ϱ� ���� ����.
	int userpassword; // User Password�� �����ϱ� ���� ����.
	string usernickname; // User Nickname�� �����ϱ� ���� ����.
	string roomname; // ���� �̸��� �����ϱ� ���� ����
	int roommaxsize; // �� �ִ� �ο� �����ϱ� ���� ����.
	int roompassword; // �濡 �ɸ� Password�� �����ϱ� ���� ����
	bool roomrootaccess; // �濡�� �������� �ƴ����� Ȯ���ϱ� ���� ����
	int score; // �������ϰ� �ֱ��Һκ�
	unsigned long int serveraccesscount; // ���� ���� �ο� üũ!
	int drawarray[X_size][Y_size]; // ���ӳ����� ���� �ִ� ���� �׸��� ���� �迭
	int testnumber;
	void format(); // �����ڿ� �Ȱ��� ��Ȱ�� �ϴ� �޼ҵ�
	Client();	//Data�� �ʱ�ȭ�ϱ� ���� ������.
};

Client::Client(){	//Data�� �ʱ�ȭ�ϱ� ���� ������.
	scenenumber = 1;
	for (int i = X_size; i > 0; i--){
		for (int j = Y_size; j > 0; j--){
			drawarray[i - 1][j - 1] = 1;
		}
	}//drawmap�� ��絥���Ϳ� 0���� �ʱ�ȭ�ϱ�����.
	userid = " ";
	userpassword = 0;
	usernickname = " ";
	roomname = " ";
	roommaxsize = 0;
	roompassword = 0;
	roomrootaccess = false;
	serveraccesscount = 0;
	score = 0;
};

void Client::format(){
	scenenumber = 1;
	for (int i = X_size; i > 0; i--){
		for (int j = Y_size; j > 0; j--){
			drawarray[i - 1][j - 1] = 1;
		}
	}//drawmap�� ��絥���Ϳ� 0���� �ʱ�ȭ�ϱ�����.
	userid = " ";
	userpassword = 0;
	usernickname = " ";
	roomname = " ";
	roommaxsize = 0;
	roompassword = 0;
	roomrootaccess = false;
	serveraccesscount = 0;
}

Client client; // Client ��ü����.