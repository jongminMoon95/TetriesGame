#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>

#define X_size 27
#define Y_size 15

using namespace std;


class MasterServerData
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

	MasterServerData();
};

static MasterServerData masterserverdata;




MasterServerData::MasterServerData()
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


MasterServerData RecvData[4];
MasterServerData SendData[4];


int check = 0;


static void __cdecl RecvThread(void * p)
{

	SOCKET sock = (SOCKET)p;

	int checkTh = 0;
	checkTh = check -1 ;

	printf("������ �ѹ� : %d\n", checkTh);

	while (1)
	{
		int recvsize = recv(sock, (char *)&RecvData[checkTh - 1], sizeof(MasterServerData), 0);
		if (recvsize == -1)
		{
			printf("���Ž��� or Ŭ���̾�Ʈ�κ��� ������ ������\n");
			check--;
			break;
		}

		//printf("drawarray >> %d\n", RecvData[0].drawarray[0][1]);
		printf("%d \n",checkTh);
		printf("number >> %d\n", RecvData[0].scenenumber);
		printf("number >> %d\n", RecvData[1].scenenumber);
		printf("number >> %d\n", RecvData[2].scenenumber);
		printf("number >> %d\n", RecvData[3].scenenumber);


		//----------Ŭ���̾�Ʈ���� ����------------------
		// ������ ������ ���� 
		// MasterServerData�� ���� �ְ�
		// ����� Ŭ���̾�Ʈ���� �����͸� �Ѱ��ش�.

		//SendData.scenenumber = 100;


		if (checkTh == 1)
		{
			for (int i = 0; i < Y_size; i++)
			{
				for (int j = 0; j < X_size; j++)
				{
					SendData[0].serveraccess = true;
					SendData[0].roomrootaccess = true;
					strcpy(SendData[0].roomname[0], "1:1 ���Ǿ!!");
					strcpy(SendData[0].usernickname[0], "����1");
					strcpy(SendData[0].usernickname[1], SendData[1].usernickname[0]);
					strcpy(SendData[0].usernickname[2], SendData[2].usernickname[0]);
					strcpy(SendData[0].usernickname[3], SendData[3].usernickname[0]);
					SendData[0].serveraccesscount = checkTh;
					SendData[0].drawarray[i][j] = RecvData[checkTh-1].drawarray[i][j];
					SendData[1].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[2].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[3].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					if (RecvData[0].roomnumber == 0){
						SendData[0].roomminsize = checkTh;
						SendData[0].roomrootaccess = true;
					}
				}
			}
			SendData[1].scenenumber = RecvData[0].scenenumber;
		}

		
		if (checkTh == 2)
		{
			for (int i = 0; i < Y_size; i++)
			{
				for (int j = 0; j < X_size; j++)
				{
					SendData[1].serveraccess = true;
					SendData[1].roomrootaccess = true;
					strcpy(SendData[1].roomname[0], "1:1 ��������!!");
					strcpy(SendData[1].usernickname[0], "����2");
					strcpy(SendData[1].usernickname[1], SendData[0].usernickname[0]);
					strcpy(SendData[1].usernickname[2], SendData[2].usernickname[0]);
					strcpy(SendData[1].usernickname[3], SendData[3].usernickname[0]);
					SendData[1].serveraccesscount = checkTh;
					SendData[1].drawarray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[0].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[2].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[3].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					if (RecvData[0].roomnumber == 0){
						SendData[0].roomminsize = checkTh;
						SendData[0].roomrootaccess = true;
					}
				}
			}
		}

		if (checkTh == 3)
		{
			for (int i = 0; i < Y_size; i++)
			{
				for (int j = 0; j < X_size; j++)
				{
					SendData[2].serveraccess = true;
					SendData[2].roomrootaccess = true;
					strcpy(SendData[2].roomname[0], "1:1 �����ض�!!");
					strcpy(SendData[2].usernickname[0], "����3");
					strcpy(SendData[2].usernickname[1], SendData[0].usernickname[0]);
					strcpy(SendData[2].usernickname[2], SendData[1].usernickname[0]);
					strcpy(SendData[2].usernickname[3], SendData[3].usernickname[0]);
					SendData[2].serveraccesscount = checkTh;
					SendData[2].drawarray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[0].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[1].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[3].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					if (RecvData[0].roomnumber == 0){
						SendData[0].roomminsize = checkTh;
						SendData[0].roomrootaccess = true;
					}
				}
			}
		}

		if (checkTh == 4)
		{
			for (int i = 0; i < Y_size; i++)
			{
				for (int j = 0; j < X_size; j++)
				{
					SendData[3].serveraccess = true;
					SendData[3].roomrootaccess = true;
					strcpy(SendData[3].roomname[0], "1:1 ���Ǹ�!!");
					strcpy(SendData[3].usernickname[0], "����4");
					strcpy(SendData[3].usernickname[1], SendData[0].usernickname[0]);
					strcpy(SendData[3].usernickname[2], SendData[1].usernickname[0]);
					strcpy(SendData[3].usernickname[3], SendData[2].usernickname[0]);
					SendData[3].serveraccesscount = checkTh;
					SendData[3].drawarray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[0].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[2].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					SendData[1].playeronearray[i][j] = RecvData[checkTh - 1].drawarray[i][j];
					if (RecvData[0].roomnumber == 0){
						SendData[0].roomminsize = checkTh;
						SendData[0].roomrootaccess = true;
					}
				}
			}
		}
		

		int sending = send(sock, (char *)&SendData[checkTh-1], sizeof(MasterServerData), 0);
		
		if (sending == -1)
		{
			printf("�߽Ž���\n");
			break;

		}

	}

	//-----------���� �ݱ�---------------
	closesocket(sock);
	//------------------------------------

}
	


	



class MasterServer
{
public:
	//SOCKET sock, client;
	//WSADATA wsa; //���� ��� �Լ����� ����ϱ����ؼ� ����ü�� �������
	//struct sockaddr_in serverinfo, clientinfo;
	int serversize;
	int Strlen;

	SOCKET serv_sock;


	int size;


	MasterServer();
	~MasterServer();

	void server_setting();
	void server_connect();
	//void server_send();
	//void server_receive();
	//void server_accesslist();
	void server_cloese();
};

MasterServer::MasterServer()
{
	//clnt_addr = { 0 };
	size = sizeof(SOCKADDR_IN);
}

MasterServer::~MasterServer()
{

}


void MasterServer::server_setting()
{
	//-------���� ���̺귯�� �ҷ�����--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (retval != 0)
	{
		printf("WSAStartup() Error\n");
		//return 0;
	}
	//-------------------------------------------

	//----------���� ����--------------

	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serv_sock == SOCKET_ERROR)
	{
		printf("socket() Error\n");

	}
	//-----------------------------------

	//--------����(�ڽ�)�� ���� ���� �Է�------------
	SOCKADDR_IN serv_addr = { 0 };					// �ʱ�ȭ
	serv_addr.sin_family = AF_INET;					// IP ���
	serv_addr.sin_port = htons(4000);				// ��Ʈ 4000��
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// �ּҴ� �˾Ƽ� ã��
	//------------------------------------------------

	//-----------���ͳݿ� ����---------------------
	retval = bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
	{
		printf("bind() Error\n");
		//return 0;
	}
	//--------------------------------------------

	//-----------����ο� ����-----------------
	listen(serv_sock, 5);		// 5������� ����� �� �ְ� ��...
	//-------------------------------------------


}



void MasterServer::server_connect()
{
	while (1)
	{
		SOCKADDR_IN clnt_addr;
		SOCKET clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &size);
		
		if (clnt_sock == SOCKET_ERROR)
		{
			printf("accept() Error\n");
			continue;
		}

		check++;

		//----------------------------------------------------------------------
		printf("Ŭ���̾�Ʈ ����\n");
		printf("IP : %s, Port : %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
		
		//-----------���� ������ ����-------------
		_beginthread(RecvThread, NULL, (void*)clnt_sock);
	}
}


void MasterServer::server_cloese()
{
	closesocket(serv_sock);
}

int main()
{
	
	
	MasterServer t;

	t.server_setting();
	t.server_connect();
	//t.server_connect();

	t.server_cloese();
	WSACleanup();
	
	return 0;
}

//Recv�Լ� �ٸ� ����

// �����͸� �ѹ��� ������ ��� ©�� �޴´�.
void MyRecv(SOCKET socket)
{
	int  nRecvSize = 0;
	bool bRecvDone = false;

	char pDataBuff[1024];	// �����͸� ���� ���� 
	int nDataBuffSize = 0;	// ���� �޾ƾ��� ������ ������

	nDataBuffSize = sizeof(MasterServerData);
	memset(pDataBuff, 0, 1024);

	while (!bRecvDone)
	{
		// �����͸� �޴´�
		int nRet = recv(socket, &pDataBuff[nRecvSize], nDataBuffSize, 0);// �����͸� �޴´�.
		if (nRet)
		{
			nRecvSize += nRet;// recv�Լ��� ���ؼ� ���� ������ 

			if (nRecvSize == nDataBuffSize)// recv���ؼ� ���� ������ ���̰� �޾ƾ��� ������ ���̿� �ٸ��ٸ�.. ��� �����͸� �޴´�.
				bRecvDone = true;
		}
	}

	if (bRecvDone == true)// �����͸� �� �޾Ҵٸ�
	{
		MasterServerData data;
		memcpy(&data, pDataBuff, nDataBuffSize);// ���� ���̸�ŭ �޸� ī��
		// �����ϰ� ���
	}

}