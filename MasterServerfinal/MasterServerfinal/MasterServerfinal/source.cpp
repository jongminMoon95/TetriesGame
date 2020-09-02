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
	int scenenumber; // 게임 화면의 Scene를 체크하기위한 변수.
	char userid[256]; // User ID를 저장하기 위한 변수.
	char userpassword[256]; // User Password를 저장하기 위한 변수.
	char usernickname[5][256]; // User Nickname을 저장하기 위한 변수.
	char roomname[5][256]; // 방의 이름을 저장하기 위한 변수
	int roomminsize;
	int roommaxsize; // 방 최대 인원 저장하기 위한 변수.
	int roompassword; // 방에 걸린 Password를 저장하기 위한 변수
	int roomnumber;
	bool roomrootaccess; // 방에서 방장인지 아닌지를 확인하기 위한 변수
	bool serveraccess;
	int score; // 점수저장
	int scoreone; // player 1의 점수
	int scoretwo; // player 2의 점수
	int scorethree; // player 3의 점수
	unsigned long int serveraccesscount; // 서버 접속 인원 체크!
	int drawarray[Y_size][X_size]; // 게임내에서 블럭이 있는 곳을 그리기 위한 배열
	int playeronearray[Y_size][X_size]; // 플레이어 1의 데이터 저장장소
	int playertwoarray[Y_size][X_size]; // 플레이어 2의 데이터 저장장소
	int playerthreearray[Y_size][X_size]; // 플레이어 3의 데이터 저장장소

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
	scenenumber = 0; //게임 화면의 scene을 체크하기위한 변수.
	memset(userpassword, 0, 256); // user id를 저장하기 위한 변수.
	memset(usernickname, 0, 256);//user password를 저장하기 위한 변수.
	roommaxsize = 0;//방 최대 인원 저장하기 위한 변수.
	roompassword = 0;//방에 걸린 password를 저장하기 위한 변수.
	roomrootaccess = false;//방에서 방장인지 아닌지를 확인하기 위한 변수
	serveraccesscount = 0; //서버 인원체크 변수.
	score = 0; // 점수저장
	scoreone = 0; // player 1의 점수
	scoretwo = 0; // player 2의 점수
	scorethree = 0; // player 3의 점수

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

	printf("스레드 넘버 : %d\n", checkTh);

	while (1)
	{
		int recvsize = recv(sock, (char *)&RecvData[checkTh - 1], sizeof(MasterServerData), 0);
		if (recvsize == -1)
		{
			printf("수신실패 or 클라이언트로부터 연결이 끊어짐\n");
			check--;
			break;
		}

		//printf("drawarray >> %d\n", RecvData[0].drawarray[0][1]);
		printf("%d \n",checkTh);
		printf("number >> %d\n", RecvData[0].scenenumber);
		printf("number >> %d\n", RecvData[1].scenenumber);
		printf("number >> %d\n", RecvData[2].scenenumber);
		printf("number >> %d\n", RecvData[3].scenenumber);


		//----------클라이언트에게 전송------------------
		// 전송할 데이터 세팅 
		// MasterServerData에 값을 넣고
		// 연결된 클라이언트에게 데이터를 넘겨준다.

		//SendData.scenenumber = 100;


		if (checkTh == 1)
		{
			for (int i = 0; i < Y_size; i++)
			{
				for (int j = 0; j < X_size; j++)
				{
					SendData[0].serveraccess = true;
					SendData[0].roomrootaccess = true;
					strcpy(SendData[0].roomname[0], "1:1 한판어떄!!");
					strcpy(SendData[0].usernickname[0], "유저1");
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
					strcpy(SendData[1].roomname[0], "1:1 한판하자!!");
					strcpy(SendData[1].usernickname[0], "유저2");
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
					strcpy(SendData[2].roomname[0], "1:1 한판해라!!");
					strcpy(SendData[2].usernickname[0], "유저3");
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
					strcpy(SendData[3].roomname[0], "1:1 한판만!!");
					strcpy(SendData[3].usernickname[0], "유저4");
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
			printf("발신실패\n");
			break;

		}

	}

	//-----------소켓 닫기---------------
	closesocket(sock);
	//------------------------------------

}
	


	



class MasterServer
{
public:
	//SOCKET sock, client;
	//WSADATA wsa; //소켓 통신 함수들을 사용하기위해서 구조체를 만들엇음
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
	//-------소켓 라이브러리 불러오기--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (retval != 0)
	{
		printf("WSAStartup() Error\n");
		//return 0;
	}
	//-------------------------------------------

	//----------소켓 생성--------------

	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serv_sock == SOCKET_ERROR)
	{
		printf("socket() Error\n");

	}
	//-----------------------------------

	//--------서버(자신)의 소켓 정보 입력------------
	SOCKADDR_IN serv_addr = { 0 };					// 초기화
	serv_addr.sin_family = AF_INET;					// IP 사용
	serv_addr.sin_port = htons(4000);				// 포트 4000번
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 주소는 알아서 찾기
	//------------------------------------------------

	//-----------인터넷에 연결---------------------
	retval = bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
	{
		printf("bind() Error\n");
		//return 0;
	}
	//--------------------------------------------

	//-----------대기인원 설정-----------------
	listen(serv_sock, 5);		// 5명까지만 대기할 수 있게 함...
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
		printf("클라이언트 접속\n");
		printf("IP : %s, Port : %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
		
		//-----------수신 스레드 생성-------------
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

//Recv함수 다른 샘플

// 데이터를 한번에 못받을 경우 짤라서 받는다.
void MyRecv(SOCKET socket)
{
	int  nRecvSize = 0;
	bool bRecvDone = false;

	char pDataBuff[1024];	// 데이터를 받을 버퍼 
	int nDataBuffSize = 0;	// 실제 받아야할 데이터 사이즈

	nDataBuffSize = sizeof(MasterServerData);
	memset(pDataBuff, 0, 1024);

	while (!bRecvDone)
	{
		// 데이터를 받는다
		int nRet = recv(socket, &pDataBuff[nRecvSize], nDataBuffSize, 0);// 데이터를 받는다.
		if (nRet)
		{
			nRecvSize += nRet;// recv함수를 통해서 받은 데이터 

			if (nRecvSize == nDataBuffSize)// recv통해서 받은 데이터 길이가 받아야할 데이터 길이와 다르다면.. 계속 데이터를 받는다.
				bRecvDone = true;
		}
	}

	if (bRecvDone == true)// 데이터를 다 받았다면
	{
		MasterServerData data;
		memcpy(&data, pDataBuff, nDataBuffSize);// 받은 길이만큼 메모리 카피
		// 적절하게 사용
	}

}