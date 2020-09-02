/*
********************************************************* Object_Oriented Programing _ Team Project *********************************************************

이 소스는 최상위 Class인 Client를 정의하기 위함.

※ 코드 작성시 유의사항
1. Class 의 경우만 첫 글자를 대문자로 정의하며, 그 외의 모든 이름은 소문자로 사용할 것
2. Method를 정의할 경우 해당 Method가 어떠한 기능을 작동하는지 간략하게 주석처리 할 것.
3. goto문의 경우 필요한 경우에만 사용하며, 사용할 경우 해당 Label마다 주석처리 할것.
4. 프로그램 내에서 사용되는 모든 data는 가능한 은닉화할 것.
5. Method 나 변수 등을 선언할 경우 사용 용도와 기능에 관련된 이름으로 선언할 것.
6. 공통적으로 사용되는 최상위 Client에서의 수정이 필요할 경우 의논하고 수정할 것.
7. 표준 라이브러리외에는 헤더를 사용하지 않을 것.
8. Method의 경우에만 단어의 어절마다 _를 사용할 것. Data의 경우는 사용하지 않는다.
   ex) Method - void roomsize() → void room_size() 형식으로 사용할 것.
9. Class내의 소멸자는 사용하지 않는다. 필요할 경우 임의의 Method를 선언하여 사용한다.
   단, 생성자는 예외.
10. Thread 읙 경우 Main이 있는 소스에서 선언하도록 한다.
************************************************************************************************************************************************************
*/

/*
	※ scenenumber가 가리키는 각각의 화면 - 추가 및 변동시 수정할것.
	1. 시작화면 - 싱글,멀티,Creatdit 선택화면
	2. 멀티플레이 로그인 화면
	3. 멀티플레이 서버화면
	4. 방접속 대기화면
	5. Creatdit 화면
	6. 테트리스 싱글플레이 모드화면
	7. 테트리스 멀티플레이 모드 화면
	8.
*/

#include<iostream>
#include<string>
#include<time.h>

using namespace std;

#define X_size 27
#define Y_size 15

// Client Class - ID,비밀번호,방 이름,게임화면,방 최대인원,방 비밀번호, 서버접속인원, User 닉네임 등의 Public Data정의.
class Client
{
public:
	int scenenumber; // 게임 화면의 Scene를 체크하기위한 변수.
	string userid; // User ID를 저장하기 위한 변수.
	int userpassword; // User Password를 저장하기 위한 변수.
	string usernickname; // User Nickname을 저장하기 위한 변수.
	string roomname; // 방의 이름을 저장하기 위한 변수
	int roommaxsize; // 방 최대 인원 저장하기 위한 변수.
	int roompassword; // 방에 걸린 Password를 저장하기 위한 변수
	bool roomrootaccess; // 방에서 방장인지 아닌지를 확인하기 위한 변수
	int score; // 문종민하고 애기할부분
	unsigned long int serveraccesscount; // 서버 접속 인원 체크!
	int drawarray[X_size][Y_size]; // 게임내에서 블럭이 있는 곳을 그리기 위한 배열
	int testnumber;
	void format(); // 생성자와 똑같은 역활을 하는 메소드
	Client();	//Data를 초기화하기 위한 생성자.
};

Client::Client(){	//Data를 초기화하기 위한 생성자.
	scenenumber = 1;
	for (int i = X_size; i > 0; i--){
		for (int j = Y_size; j > 0; j--){
			drawarray[i - 1][j - 1] = 1;
		}
	}//drawmap에 모든데이터에 0으로 초기화하기위함.
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
	}//drawmap에 모든데이터에 0으로 초기화하기위함.
	userid = " ";
	userpassword = 0;
	usernickname = " ";
	roomname = " ";
	roommaxsize = 0;
	roompassword = 0;
	roomrootaccess = false;
	serveraccesscount = 0;
}

Client client; // Client 객체생성.