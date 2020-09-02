#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include"Game.h"
#include<process.h>

#define X_size 15
#define Y_size 27

class MasterClient
{

public:
	int scenenumber; // ���� ȭ���� Scene�� üũ�ϱ����� ����.
	char userid[256]; // User ID�� �����ϱ� ���� ����.
	char userpassword[256]; // User Password�� �����ϱ� ���� ����.
	char usernickname[5][256]; // User Nickname�� �����ϱ� ���� ����.
	char roomname[5][256]; // ���� �̸��� �����ϱ� ���� ����
	int roomminsize;
	int roommaxsize; // �� �ִ� �ο� �����ϱ� ���� ����.
	int roompassword; // �濡 �ɸ� Password�� �����ϱ� ���� ����
	int roomnumber;
	bool roomrootaccess; // �濡�� �������� �ƴ����� Ȯ���ϱ� ���� ����
	bool serveraccess;
	int score; // ��������
	int scoreone; // player 1�� ����
	int scoretwo; // player 2�� ����
	int scorethree; // player 3�� ����
	unsigned long int serveraccesscount; // ���� ���� �ο� üũ!
	int drawarray[Y_size][X_size]; // ���ӳ����� ���� �ִ� ���� �׸��� ���� �迭
	int playeronearray[Y_size][X_size]; // �÷��̾� 1�� ������ �������
	int playertwoarray[Y_size][X_size]; // �÷��̾� 2�� ������ �������
	int playerthreearray[Y_size][X_size]; // �÷��̾� 3�� ������ �������

	bool startcheck;

	/*
	int scenenumber; //���� ȭ���� scene�� üũ�ϱ����� ����.
	char userpassword[256]; // user id�� �����ϱ� ���� ����.
	char userid[256];
	char usernickname[256]; //user password�� �����ϱ� ���� ����.
	char roomname[256]; // �� �̸��� �����ϱ� ���� ����.
	char test[2]; //�߽�
	char moon[2]; //����
	int roommaxsize; //�� �ִ� �ο� �����ϱ� ���� ����.
	int roompassword; //�濡 �ɸ� password�� �����ϱ� ���� ����.
	bool roomrootaccess; //�濡�� �������� �ƴ����� Ȯ���ϱ� ���� ����
	unsigned long int serveraccesscount; //���� �ο�üũ ����.
	int drawarray[X_size][Y_size]; // ���ӳ����� ���� �ִ� ���� �׸��� ���� �迭.
	*/
	MasterClient();
};

MasterClient::MasterClient()
{
	serveraccess = false;
	roomnumber = -1;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 256; j++){
			usernickname[i][j] = ' ';
			roomname[i][j] = ' ';
		}
	}

	startcheck = false;
	roomminsize = 0;
	scenenumber = 0; //���� ȭ���� scene�� üũ�ϱ����� ����.
	memset(userpassword, 0, 256); // user id�� �����ϱ� ���� ����.
	memset(usernickname, 0, 256);//user password�� �����ϱ� ���� ����.
	roommaxsize = 0;//�� �ִ� �ο� �����ϱ� ���� ����.
	roompassword = 0;//�濡 �ɸ� password�� �����ϱ� ���� ����.
	roomrootaccess = false;//�濡�� �������� �ƴ����� Ȯ���ϱ� ���� ����
	serveraccesscount = 0; //���� �ο�üũ ����.
	score = 0; // ��������
	scoreone = 0; // player 1�� ����
	scoretwo = 0; // player 2�� ����
	scorethree = 0; // player 3�� ����

	for (int i = 0; i < Y_size; i++)
	{
		for (int z = 0; z < X_size; z++)
		{
			drawarray[i][z] = 0;
			playeronearray[i][z] = 0;
			playertwoarray[i][z] = 0;
			playerthreearray[i][z] = 0;
		}
	}

	//memset(test, 0, 2);
	//memset(moon, 0, 2);
}


static void __cdecl RecvThread(void * p)
{

	SOCKET sock = (SOCKET)p;


	while (1)
	{


		MasterClient RecvData;
		MasterClient SendData;
		// Ŭ���̾�Ʈ�κ��� �����͸� �޴´�.
		// sizeof (MssterServerData) ���� ��ŭ RecvData�� �����͸� �޴´�.
		// (char *)&RecvData    : RecvData�� �ּҰ�(&)�� �־��ٴ°� RecvData�� �޸� �ּҿ� �Ѱܹ��� ������ ���� �־��ִ� �Ͱ� ����.
		int recvsize = recv(sock, (char *)&RecvData, sizeof(MasterClient), 0);

		if (recvsize == -1)
		{
			printf("���Ž��� or Ŭ���̾�Ʈ�κ��� ������ ������\n");
			break;
		}

				// ������ �ޱ⿡ �����ߴٸ���
		// Ŭ���̾�Ʈ���� �Ѱ��� �����͸� Ȯ�����ش�.

		printf("client >> %s\n", RecvData.userid);
		//printf("client >> %s\n", RecvData.userpassword);



		//----------Ŭ���̾�Ʈ���� ����------------------
		// ������ ������ ���� 
		// MasterServerData�� ���� �ְ�
		// ����� Ŭ���̾�Ʈ���� �����͸� �Ѱ��ش�.

		//SendData.scenenumber = 100;

		int sending = send(sock, (char *)&SendData, sizeof(MasterClient), 0);
		if (sending == -1)
		{
			printf("�߽Ž���\n");
			break;
		}

		printf("������ : %d\n", SendData.scenenumber);

	}
	//-----------���� �ݱ�---------------
	closesocket(sock);
	//------------------------------------
}


class SocketMasterClient
{
public:

	SOCKET MasterClientsock;


	char message[255];
	char chatme[255];
	int Strlen;
	char ip[13];



	SocketMasterClient();
	~SocketMasterClient();

	void intro();
	void SettingConnect();
	void SR();
	void Close();

	void run()
	{
		CreateThread(0, 0, __Thsend, (void*)this, 0, 0);
		CreateThread(0, 0, __Threceive, (void*)this, 0, 0);
	}

protected:

	virtual void Thsend(){}
	virtual void Threceive(){}



	static unsigned long __stdcall __Thsend(void *args)
	{
		SocketMasterClient* self = (SocketMasterClient*)args;
		self->Thsend();
		return 0;
	}

	static unsigned long __stdcall __Threceive(void *args)
	{
		SocketMasterClient* self = (SocketMasterClient*)args;
		self->Threceive();
		return 0;
	}

};

SocketMasterClient::SocketMasterClient()
{
	memset(message, 0, 255);
	memset(chatme, 0, 255);
	memset(ip, 0, 13);


}

SocketMasterClient::~SocketMasterClient()
{

}

void SocketMasterClient::intro()
{
	cout << "==============================================================================\n";
}

void SocketMasterClient::SettingConnect()
{

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("�ʱ�ȭ ����\n");
	}

	MasterClientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (MasterClientsock == INVALID_SOCKET)
	{
		printf("���� ���� ����\n");
	}

	struct sockaddr_in sockinfo;
	memset(&sockinfo, 0, sizeof(sockinfo)); // �ּ� ����ü �Լ� �ʱ�ȭ



	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(4000); // ��Ʈ�� �� ��°�� �Է� ����
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP�� ù ��°�� �Է� ����

	if (connect(MasterClientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) // ���� ���� ���� ��û
	{
		cout << " ���� ���� ���� ";
	}

	else
	{
		cout << "����� ������ �����߽��ϴ�.\n\n";
	}

}

struct Mydata
{
	int data1;

	Mydata()
	{
		data1 = 13;
	}
};

Mydata *save;
Mydata temp;

char a[60];

void SocketMasterClient::SR()
{

	MasterClient SendData;


	SendData.roomnumber = client.roomnumber;
	if (client.startcheck == true)
		SendData.scenenumber = 8;

	int test = send(MasterClientsock, (char *)&SendData, sizeof(MasterClient), 0);

	if (test == -1)
	{
		printf("�߽Ž���\n");
	}


	MasterClient RecvData;
	Strlen = recv(MasterClientsock, (char *)&RecvData, sizeof(MasterClient), 0);// recv �Լ� ��ȯ �� ����

	if (Strlen == -1) // recv �Լ� ��ȯ ���� -1�̸� �޽��� ���� ����
	{
		cout << " �޼��� ���� ���� ";
	}

	client.serveraccess = RecvData.serveraccess;
	client.userid = RecvData.userid;
	client.userpassword = RecvData.userpassword;
	client.usernickname[0] = RecvData.usernickname[0];
	client.roomname[0] = RecvData.roomname[0];
	client.serveraccesscount = RecvData.serveraccesscount;
	client.roomminsize = RecvData.roomminsize;
	client.roomrootaccess = RecvData.roomrootaccess;



}


void SocketMasterClient::Close()
{
	closesocket(MasterClientsock);
	WSACleanup();
}

class Thread : public SocketMasterClient
{
public:

	void Thsend()
	{
		while (1)
		{
			Sleep(1000);
			//cout << "Send : ";
			//fgets(chatme, sizeof(chatme), stdin);

			send(MasterClientsock, chatme, sizeof(chatme), 0);
		}
	}

	void Threceive()
	{
		while (1)
		{
			Sleep(1000);
			Strlen = recv(MasterClientsock, message, sizeof(message), 0);// recv �Լ� ��ȯ �� ����

			if (Strlen == -1) // recv �Լ� ��ȯ ���� -1�̸� �޽��� ���� ����
			{
				cout << " �޼��� ���� ���� ";
			}

			printf("receive : %c", message[0]); // ���� �� �޽��� ǥ��
		}

	}

};


SocketMasterClient t;



static void __cdecl ClientThread(void * p)
{
	t.intro();
	t.SettingConnect();

	while (1)
	{
		t.SR();
	}

}