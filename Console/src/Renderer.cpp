#include "Renderer.hpp"

#include <Windows.h>

//#include <cstdlib>
//#include <cstring>
//#include <string>
#include <iostream>
#include <sstream>

// ��ʼ������
//void Renderer::init(const char *_title) const
//{
//	// ���ô��ڳ���
//	std::string command = "mode con lines=" + std::to_string(_row) + " cols=" + std::to_string(_column);
//	std::system(command.c_str());
//
//	// ���ô��ڱ���
//	if (_title)
//	{
//		command.assign("title ").append(_title);
//		std::system(command.c_str());
//	}
//
//	// ���ع��
//	hideCursor();
//}

// ��ʼ������
void Renderer::init(const char* _title) const
{
	// ���ô��ڳ���
	SMALL_RECT rect = { 0, 0, _column - 1, _row - 1 };
	SetConsoleWindowInfo(_handle, TRUE, &rect);

	// ����Ļ�����������봰�ڳ���һ��ʱ�����غ��ݹ�����
	SetConsoleScreenBufferSize(_handle, COORD{ _column, _row });

	// �̶����ڴ�С
	SetWindowLongPtr(
		GetConsoleWindow(), // ��ȡ����̨���ھ��
		GWL_STYLE, // ָ���޸Ĵ�����ʽ
		GetWindowLongPtr(GetConsoleWindow(), GWL_STYLE) // ��ȡ����ԭ��ʽ
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX); // ���ڴ�С��Ϊ�޷��ı䣬��󻯰�ť��Ϊ����Ծ

	// ���ô��ڱ���
	if (_title)
	{
#if defined UNICODE
		//int length = static_cast<int>(sizeof(char) * std::strlen(_title));
		//int number = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_USEGLYPHCHARS, _title, length, NULL, 0) + 1;
		//TCHAR *buffer = new TCHAR[number];
		//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_USEGLYPHCHARS, _title, length, buffer, number);
		//buffer[number - 1] = TEXT('\0');
		//SetConsoleTitle(buffer);
		//delete buffer;
		SetConsoleTitleA(_title);
#else
		SetConsoleTitle(_title);
#endif
	}

	// ���ع��
	hideCursor();
}

// ���ع��
void Renderer::hideCursor() const
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(_handle, &info);
	info.bVisible = FALSE;
	SetConsoleCursorInfo(_handle, &info);
}

// ���ع�꣨�����ݾɰ����̨��
//void Renderer::hideCursor() const
//{
//	std::cout << "\033[?25l";
//}

// ��ʾ���
void Renderer::showCursor() const
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(_handle, &info);
	info.bVisible = TRUE;
	SetConsoleCursorInfo(_handle, &info);
}

// ��ʾ��꣨�����ݾɰ����̨��
//void Renderer::showCursor() const
//{
//	std::cout << "\033[?25h";
//}

// �ƶ����
void Renderer::moveCursor(int16_t _x, int16_t _y) const
{
	COORD coord = { _x, _y };
	SetConsoleCursorPosition(_handle, coord);
}

// �ƶ���꣨�����ݾɰ����̨��
//void Renderer::moveCursor(int16_t _x, int16_t _y) const
//{
//	std::cout << "\033[" << _y << ';' << _x << 'H';
//	// \033[0m ������������
//	// \033[1m ����
//	// \033[4m �»���
//	// \033[7m ���ԣ�����ǰ��ɫ�뱳��ɫ��
//	// \033[nA �������n��
//	// \033[nB �������n��
//	// \033[nC �������n��
//	// \033[nD �������n��
//	// \033[K ����������β�����ݣ������ڵ�ǰ�������������β
//	// \033[s ������λ��
//	// \033[u �ָ����λ��
//}

Renderer::Renderer(int16_t _row, int16_t _column, const char *_title)
	: _handle(GetStdHandle(STD_OUTPUT_HANDLE)), _row(_row), _column(_column)
{
	init(_title);
}

// ��տ���̨����
//void Renderer::clearScreen() const
//{
//	std::system("cls");
//}

// ��տ���̨����
void Renderer::clearScreen() const
{
	// ��ȡ����̨��������Ϣ
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(_handle, &info);

	// �Կո������������̨
	DWORD size = info.dwSize.X * info.dwSize.Y; // ���������ַ�������
	DWORD number;
	COORD coord = { 0, 0 };
	FillConsoleOutputCharacter(_handle, \
		static_cast<TCHAR>(' '), size, coord, &number);

	// �ٴλ�ȡ��������Ϣ
	GetConsoleScreenBufferInfo(_handle, &info);

	// ��Ĭ����ɫ������̨
	FillConsoleOutputAttribute(_handle, \
		info.wAttributes, size, coord, &number);

	// �ƶ������(0, 0)
	SetConsoleCursorPosition(_handle, coord);
}

// ��տ���̨���ڣ������ݾɰ����̨��
//void Renderer::clearScreen() const
//{
//	std::cout << "\033[2J" << "\033[0;0H";
//}

/*
 * ���ô�����ɫ
 * ������
 *     _background    ����ɫ
 *     _foreground    ǰ��ɫ
 */
//void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (_background >> 4 & 0x0F) << (_foreground & 0x0F);
//	system(output.str().c_str());
//}

// ���ô�����ɫ
void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
{
	// ��ȡ��������Ϣ
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(_handle, &info);

	// ��Ĭ����ɫ������̨
	DWORD size = info.dwSize.X * info.dwSize.Y; // ���������ַ�������
	DWORD number;
	COORD coord = { 0, 0 };
	FillConsoleOutputAttribute(_handle, \
		_background, size, coord, &number);

	// �ƶ������(0, 0)
	SetConsoleCursorPosition(_handle, coord);

	// ����������ɫ
	setTextColor(_foreground, _background);
}

/*
 * ������ɫλ����ɫλ
 * ������
 *     _color    ��ɫ
 *     _offset   ����ƫ��
 */
//static uint16_t swapRedAndBlue(uint16_t _color, uint16_t _offset) noexcept
//{
//	// ��ȡ��ɫλ����ɫλ
//	uint16_t temp = _color & 0x1 << _offset;
//
//	// ����ɫλ����ֵΪ��һɫλ
//	_color = _color & ~(0x1 << _offset) | (_color & 0x4 << _offset) >> 2;
//	return _color & ~(0x4 << _offset) | temp << 2;
//}

// ���ô�����ɫ�������ݾɰ����̨��
//void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
//{
//	// ����ɫλ
//	uint16_t color = swapRedAndBlue(_background, 4);
//
//	// ���ñ���ɫ
//	std::ostringstream output;
//	output << "\033[" << (color >> 7 & 0x01) << ";4" << (color >> 4 & 0x07) << 'm';
//	std::cout << output.str();
//
//	// ��䴰��
//	clearScreen();
//
//	// �������
//	std::cout << "\033[0m";
//
//	// ����������ɫ
//	setTextColor(_foreground, _background);
//}

// �����ı���ɫ
void Renderer::setTextColor(uint16_t _foreground, uint16_t _background) const
{
	SetConsoleTextAttribute(_handle, _foreground & 0x0F | _background & 0xF0);
}

// �����ı���ɫ�������ݾɰ����̨��
//void Renderer::setTextColor(uint16_t _foreground, uint16_t _background) const
//{
//	// ����ɫλ
//	_foreground = swapRedAndBlue(_foreground, 0);
//	_background = swapRedAndBlue(_background, 4);
//
//	// ���ñ���ɫ��ǰ��ɫ
//	std::ostringstream output;
//	output << "\033[" << (_background >> 7 & 0x01) << ";4" << (_background >> 4 & 0x07) << 'm'
//		<< "\033[" << (_foreground >> 3 & 0x01) << ";3" << (_foreground & 0x07) << 'm';
//	std::cout << output.str();
//}

// ����ı�
void Renderer::outText(int16_t _x, int16_t _y, const char *_text) const
{
	moveCursor(_x, _y);
	std::cout << _text;
}

// ����ı���ָ����Χ
void Renderer::outText(int16_t _left, int16_t _right, int16_t _up, int16_t _down, const char *_text) const
{
	auto length = static_cast<decltype(_left)>(std::strlen(_text));
	moveCursor((_left + _right - length) / 2, (_up + _down - 1) / 2);
	std::cout << _text;
}
