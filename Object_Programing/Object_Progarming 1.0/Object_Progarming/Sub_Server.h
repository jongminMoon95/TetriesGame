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

	/*
	int scenenumber; //게임 화면의 scene을 체크하기위한 변수.
	char userpassword[256]; // user id를 저장하기 위한 변수.
	char userid[256];
	char usernickname[256]; //user password를 저장하기 위한 변수.
	char roomname[256]; // 방 이름을 저장하기 위한 변수.
	char test[2]; //발신
	char moon[2]; //수신
	int roommaxsize; //방 최대 인원 저장하기 위한 변수.
	int roompassword; //방에 걸린 password를 저장하기 위한 변수.
	bool roomrootaccess; //방에서 방장인지 아닌지를 확인하기 위한 변수
	unsigned long int serveraccesscount; //서버 인원체크 변수.
	int drawarray[X_size][Y_size]; // 게임내에서 블럭이 있는 곳을 그리기 위한 배열.
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


static void __cdecl RecvThread(void * p)
{

	SOCKET sock = (SOCKET)p;


	while (1)
	{


		MasterClient RecvData;
		MasterClient SendData;
		// 클라이언트로부터 데이터를 받는다.
		// sizeof (MssterServerData) 길이 만큼 RecvData에 데이터를 받는다.
		// (char *)&RecvData    : RecvData의 주소값(&)을 넣었다는건 RecvData의 메모리 주소에 넘겨받은 데이터 값을 넣어주는 것과 같다.
		int recvsize = recv(sock, (char *)&RecvData, sizeof(MasterClient), 0);

		if (recvsize == -1)
		{
			printf("수신실패 or 클라이언트로부터 연결이 끊어짐\n");
			break;
		}

				// 데이터 받기에 성공했다면은
		// 클라이언트에서 넘겨준 데이터를 확인해준다.

		printf("client >> %s\n", RecvData.userid);
		//printf("client >> %s\n", RecvData.userpassword);



		//----------클라이언트에게 전송------------------
		// 전송할 데이터 세팅 
		// MasterServerData에 값을 넣고
		// 연결된 클라이언트에게 데이터를 넘겨준다.

		//SendData.scenenumber = 100;

		int sending = send(sock, (char *)&SendData, sizeof(MasterClient), 0);
		if (sending == -1)
		{
			printf("발신실패\n");
			break;
		}

		printf("보낸거 : %d\n", SendData.scenenumber);

	}
	//-----------소켓 닫기---------------
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
		printf("초기화 실패\n");
	}

	MasterClientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (MasterClientsock == INVALID_SOCKET)
	{
		printf("소켓 생성 실패\n");
	}

	struct sockaddr_in sockinfo;
	memset(&sockinfo, 0, sizeof(sockinfo)); // 주소 구조체 함수 초기화



	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(4000); // 포트를 두 번째로 입력 받음
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP를 첫 번째로 입력 받음

	if (connect(MasterClientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) // 서버 측에 연결 요청
	{
		cout << " 서버 접속 실패 ";
	}

	else
	{
		cout << "상대방과 연결이 성공했습니다.\n\n";
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
		printf("발신실패\n");
	}


	MasterClient RecvData;
	Strlen = recv(MasterClientsock, (char *)&RecvData, sizeof(MasterClient), 0);// recv 함수 반환 값 저장

	if (Strlen == -1) // recv 함수 반환 값이 -1이면 메시지 수신 실패
	{
		cout << " 메세지 수신 실패 ";
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
			Strlen = recv(MasterClientsock, message, sizeof(message), 0);// recv 함수 반환 값 저장

			if (Strlen == -1) // recv 함수 반환 값이 -1이면 메시지 수신 실패
			{
				cout << " 메세지 수신 실패 ";
			}

			printf("receive : %c", message[0]); // 수신 된 메시지 표시
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