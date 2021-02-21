#include "Renderer.h"

#include <Windows.h>

//#include <cstdlib>
//#include <cstring>
//#include <string>
#include <iostream>
#include <sstream>

Renderer::Renderer(int16_t row, int16_t column, const char *title)
{
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	this->row = row;
	this->column = column;
	init(row, column, title);
}

// ��տ���̨����
//void Renderer::clearScreen()
//{
//	std::system("cls");
//}

// ��տ���̨����
//void Renderer::clearScreen()
//{
//	// ��ȡ����̨��������Ϣ
//	CONSOLE_SCREEN_BUFFER_INFO info;
//	GetConsoleScreenBufferInfo(handle, &info);
//
//	DWORD size = info.dwSize.X * info.dwSize.Y; // ���������ַ�������
//	DWORD number;
//	COORD coord = { 0, 0 };
//	// �Կո������������̨
//	FillConsoleOutputCharacter(handle, \
//		static_cast<TCHAR>(' '), size, coord, &number);
//
//	// �ٴλ�ȡ��������Ϣ
//	GetConsoleScreenBufferInfo(handle, &info);
//	// ��Ĭ����ɫ������̨
//	FillConsoleOutputAttribute(handle, \
//		info.wAttributes, size, coord, &number);
//	// �ƶ������(0, 0)
//	SetConsoleCursorPosition(handle, coord);
//}

// ��տ���̨����
void Renderer::clearScreen()
{
	std::cout << "\033[2J" << "\033[0;0H";
}

// ���ô�����ɫ
//void Renderer::setScreenColor(uint16_t background)
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (background >> 4 & 0xF) << 'f';
//	std::system(output.str().c_str());
//}

// ���ô�����ɫ
//void Renderer::setScreenColor(uint16_t background)
//{
//	// ��ȡ��������Ϣ
//	CONSOLE_SCREEN_BUFFER_INFO info;
//	GetConsoleScreenBufferInfo(handle, &info);
//
//	DWORD size = info.dwSize.X * info.dwSize.Y; // ���������ַ�������
//	DWORD number;
//	COORD coord = { 0, 0 };
//	// ��Ĭ����ɫ������̨
//	FillConsoleOutputAttribute(handle, \
//		background, size, coord, &number);
//	// �ƶ������(0, 0)
//	SetConsoleCursorPosition(handle, coord);
//}

// ���ô�����ɫ
void Renderer::setScreenColor(uint16_t background)
{
	std::ostringstream output;
	output << "\033[" << (background >> 7 & 0x1) << ";4" << (background >> 4 & 0x7) << 'm';
	std::cout << output.str();
	clearScreen();
	std::cout << "\033[0m";
}

/*
 * �����ı���ɫ
 * ������
 *     foreground    ǰ��ɫ
 *     background    ����ɫ
 */
//void Renderer::setTextColor(uint16_t foreground, uint16_t background)
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (background >> 4 & 0xF) << (foreground & 0xF);
//	system(output.str().c_str());
//}

// �����ı���ɫ
//void Renderer::setTextColor(uint16_t foreground, uint16_t background)
//{
//	SetConsoleTextAttribute(handle, foreground & 0x0F | background & 0xF0);
//}

// �����ı���ɫ
void Renderer::setTextColor(uint16_t foreground, uint16_t background)
{
	std::ostringstream output;
	output << "\033[" << (background >> 7 & 0x1) << ";4" << (background >> 4 & 0x7) << 'm'
		<< "\033[" << (foreground >> 3 & 0x1) << ";3" << (foreground & 0x7) << 'm';
	std::cout << output.str();
}

// ����ı�
void Renderer::outText(int16_t x, int16_t y, const char *text)
{
	moveCursor(x, y);
	std::cout << text;
}

// ����ı���ָ����Χ
void Renderer::outTextRange(int16_t left, int16_t right, int16_t up, int16_t down, const char *text)
{
	auto length = static_cast<decltype(left)>(std::strlen(text));
	moveCursor((left + right - length) / 2, (up + down - 1) / 2);
	std::cout << text;
}

// ��ʼ������̨����
//void Renderer::init(int16_t row, int16_t column, const char *title)
//{
//	// ���ô��ڳ���
//	std::string command = "mode con lines=" + std::to_string(row) + " cols=" + std::to_string(column);
//	std::system(command.c_str());
//	// ���ô��ڱ���
//	if (title)
//	{
//		command.assign("title ").append(title);
//		std::system(command.c_str());
//	}
//	// ���ع��
//	hideCursor();
//}

// ��ʼ������̨����
void Renderer::init(int16_t row, int16_t column, const char *title)
{
	// ���ô��ڳ���
	SMALL_RECT rect = { 0, 0, column - 1, row - 1 };
	SetConsoleWindowInfo(handle, TRUE, &rect);
	// ����Ļ�����������봰�ڳ���һ��ʱ�����غ��ݹ�����
	SetConsoleScreenBufferSize(handle, COORD{ column, row });
	// �̶����ڴ�С
	SetWindowLongPtr(
		GetConsoleWindow(), // ��ȡ����̨���ھ��
		GWL_STYLE, // ָ���޸Ĵ�����ʽ
		GetWindowLongPtr(GetConsoleWindow(), GWL_STYLE) // ��ȡ����ԭ��ʽ
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX); // ���ڴ�С��Ϊ�޷��ı䣬��󻯰�ť��Ϊ����Ծ
	// ���ع��
	hideCursor();
	// ���ô��ڱ���
	if (title)
	{
#if defined UNICODE
		//int length = static_cast<int>(sizeof(char) * std::strlen(title));
		//int number = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_USEGLYPHCHARS, title, length, NULL, 0) + 1;
		//TCHAR *buffer = new TCHAR[number];
		//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_USEGLYPHCHARS, title, length, buffer, number);
		//buffer[number - 1] = TEXT('\0');
		//SetConsoleTitle(buffer);
		//delete buffer;
		SetConsoleTitleA(title);
#else
		SetConsoleTitle(title);
#endif
	}
}

// ���ع��
//void Renderer::hideCursor()
//{
//	CONSOLE_CURSOR_INFO info;
//	GetConsoleCursorInfo(handle, &info);
//	info.bVisible = FALSE;
//	SetConsoleCursorInfo(handle, &info);
//}

// ���ع��
void Renderer::hideCursor()
{
	std::cout << "\033[?25l";
}

// ��ʾ���
//void Renderer::showCursor()
//{
//	CONSOLE_CURSOR_INFO info;
//	GetConsoleCursorInfo(handle, &info);
//	info.bVisible = TRUE;
//	SetConsoleCursorInfo(handle, &info);
//}

// ��ʾ���
void Renderer::showCursor()
{
	std::cout << "\33[?25h";
}

// �ƶ����
//void Renderer::moveCursor(int16_t x, int16_t y)
//{
//	COORD coord = { x, y };
//	SetConsoleCursorPosition(handle, coord);
//}

// �ƶ����
void Renderer::moveCursor(int16_t x, int16_t y)
{
	std::cout << "\033[" << y << ';' << x << 'H';
	// \033[0m ������������
	// \033[1m ����
	// \033[4m �»���
	// \033[7m ���ԣ�����ǰ��ɫ�뱳��ɫ��
	// \033[nA �������n��
	// \033[nB �������n��
	// \033[nC �������n��
	// \033[nD �������n��
	// \033[K ����������β�����ݣ������ڵ�ǰ�������������β
	// \033[s ������λ��
	// \033[u �ָ����λ��
}
