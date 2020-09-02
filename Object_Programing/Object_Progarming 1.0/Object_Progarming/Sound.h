/*
이 소스는 프로그램에서 Sound 관련 옵션을 가진 Class를 정의하기 위함.
*/
#include<mmsystem.h>
#pragma comment (lib, "winmm.lib") //MCI

DWORD WINAPI soundoption(LPVOID){
	Client* pclient=&client;
BGMLoop:
	TCHAR strCmd[MAX_PATH], strRet[MAX_PATH];
	TCHAR strID[80];
	BOOL  bPlay = TRUE;
	switch (pclient->scenenumber)
	{
	case 1:
		wsprintf(strID, TEXT("mymusic%d"), 1);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("start.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 1){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;

	case 2:
		wsprintf(strID, TEXT("mymusic%d"), 2);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("login.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 2){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;

	case 3:
		wsprintf(strID, TEXT("mymusic%d"), 3);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("server.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 3){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;

	case 4:
		wsprintf(strID, TEXT("mymusic%d"), 4);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("room.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 4){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;
	case 5:
		wsprintf(strID, TEXT("mymusic%d"), 5);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("createdit.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 5){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;
	case 6:
		Sleep(4000); //4 초정도의 딜레이
		wsprintf(strID, TEXT("mymusic%d"), 6);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("tetris.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 6){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;
	case 7:
		Sleep(4000); //4 초정도의 딜레이
		wsprintf(strID, TEXT("mymusic%d"), 7);
		wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("tetris.mp3"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		wsprintf(strCmd, TEXT("play %s from 0"), strID);
		mciSendString(strCmd, strRet, sizeof(strRet), NULL);
		while (true){
			if (pclient->scenenumber != 7){
				wsprintf(strCmd, TEXT("stop %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				wsprintf(strCmd, TEXT("close %s"), strID);
				mciSendString(strCmd, strRet, sizeof(strRet), NULL);
				goto BGMLoop;
			}
		}
		break;
	}
	return 0;
}