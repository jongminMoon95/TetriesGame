/*
�� �ҽ��� ���α׷����� ����� Color �Ǵ� Mapsize�� ��ũ�� �����ϱ� ����.
*/
#define cls system("cls")
#define Map_max_size system("mode con: cols=1000 lines=1000")

#define COL GetStdHandle(STD_OUTPUT_HANDLE) // �������ǿ� �ʿ��� �⺻ �ڵ�����
#define BLACK             SetConsoleTextAttribute(COL, 0x0000);        // ������
#define DARK_BLUE         SetConsoleTextAttribute(COL, 0x0001);        // �Ķ���
#define GREEN             SetConsoleTextAttribute(COL, 0x0002);        // ���
#define BLUE_GREEN        SetConsoleTextAttribute(COL, 0x0003);        // û���
#define BLOOD             SetConsoleTextAttribute(COL, 0x0004);        // �˺�����
#define PURPLE            SetConsoleTextAttribute(COL, 0x0005);        // �����
#define GOLD              SetConsoleTextAttribute(COL, 0x0006);        // �ݻ�
#define ORIGINAL          SetConsoleTextAttribute(COL, 0x0007);        // ���� ȸ��- �ܼ�â�⺻��.
#define GRAY              SetConsoleTextAttribute(COL, 0x0008);        // ȸ��
#define BLUE		      SetConsoleTextAttribute(COL, 0x0009);        // �Ķ���
#define HIGH_GREEN		  SetConsoleTextAttribute(COL, 0x000a);        // ���λ�
#define SKY_BLUE		  SetConsoleTextAttribute(COL, 0x000b);        // �ϴû�
#define RED				  SetConsoleTextAttribute(COL, 0x000c);        // ������
#define PLUM			  SetConsoleTextAttribute(COL, 0x000d);        // ���ֻ�
#define YELLOW			  SetConsoleTextAttribute(COL, 0x000e);        // �����
#define WHITE			  SetConsoleTextAttribute(COL, 0x000f);        // ���
#define Title			  system("title Tetris_Game - Object_Oriented Programing Report !!!"); // Ÿ��Ʋ �̸�����