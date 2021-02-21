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

// 清空控制台窗口
//void Renderer::clearScreen()
//{
//	std::system("cls");
//}

// 清空控制台窗口
//void Renderer::clearScreen()
//{
//	// 获取控制台缓冲区信息
//	CONSOLE_SCREEN_BUFFER_INFO info;
//	GetConsoleScreenBufferInfo(handle, &info);
//
//	DWORD size = info.dwSize.X * info.dwSize.Y; // 缓冲区的字符容纳量
//	DWORD number;
//	COORD coord = { 0, 0 };
//	// 以空格填充整个控制台
//	FillConsoleOutputCharacter(handle, \
//		static_cast<TCHAR>(' '), size, coord, &number);
//
//	// 再次获取缓冲区信息
//	GetConsoleScreenBufferInfo(handle, &info);
//	// 以默认颜色填充控制台
//	FillConsoleOutputAttribute(handle, \
//		info.wAttributes, size, coord, &number);
//	// 移动光标至(0, 0)
//	SetConsoleCursorPosition(handle, coord);
//}

// 清空控制台窗口
void Renderer::clearScreen()
{
	std::cout << "\033[2J" << "\033[0;0H";
}

// 设置窗口颜色
//void Renderer::setScreenColor(uint16_t background)
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (background >> 4 & 0xF) << 'f';
//	std::system(output.str().c_str());
//}

// 设置窗口颜色
//void Renderer::setScreenColor(uint16_t background)
//{
//	// 获取缓冲区信息
//	CONSOLE_SCREEN_BUFFER_INFO info;
//	GetConsoleScreenBufferInfo(handle, &info);
//
//	DWORD size = info.dwSize.X * info.dwSize.Y; // 缓冲区的字符容纳量
//	DWORD number;
//	COORD coord = { 0, 0 };
//	// 以默认颜色填充控制台
//	FillConsoleOutputAttribute(handle, \
//		background, size, coord, &number);
//	// 移动光标至(0, 0)
//	SetConsoleCursorPosition(handle, coord);
//}

// 设置窗口颜色
void Renderer::setScreenColor(uint16_t background)
{
	std::ostringstream output;
	output << "\033[" << (background >> 7 & 0x1) << ";4" << (background >> 4 & 0x7) << 'm';
	std::cout << output.str();
	clearScreen();
	std::cout << "\033[0m";
}

/*
 * 设置文本颜色
 * 参数：
 *     foreground    前景色
 *     background    背景色
 */
//void Renderer::setTextColor(uint16_t foreground, uint16_t background)
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (background >> 4 & 0xF) << (foreground & 0xF);
//	system(output.str().c_str());
//}

// 设置文本颜色
//void Renderer::setTextColor(uint16_t foreground, uint16_t background)
//{
//	SetConsoleTextAttribute(handle, foreground & 0x0F | background & 0xF0);
//}

// 设置文本颜色
void Renderer::setTextColor(uint16_t foreground, uint16_t background)
{
	std::ostringstream output;
	output << "\033[" << (background >> 7 & 0x1) << ";4" << (background >> 4 & 0x7) << 'm'
		<< "\033[" << (foreground >> 3 & 0x1) << ";3" << (foreground & 0x7) << 'm';
	std::cout << output.str();
}

// 输出文本
void Renderer::outText(int16_t x, int16_t y, const char *text)
{
	moveCursor(x, y);
	std::cout << text;
}

// 输出文本至指定范围
void Renderer::outTextRange(int16_t left, int16_t right, int16_t up, int16_t down, const char *text)
{
	auto length = static_cast<decltype(left)>(std::strlen(text));
	moveCursor((left + right - length) / 2, (up + down - 1) / 2);
	std::cout << text;
}

// 初始化控制台窗口
//void Renderer::init(int16_t row, int16_t column, const char *title)
//{
//	// 设置窗口长宽
//	std::string command = "mode con lines=" + std::to_string(row) + " cols=" + std::to_string(column);
//	std::system(command.c_str());
//	// 设置窗口标题
//	if (title)
//	{
//		command.assign("title ").append(title);
//		std::system(command.c_str());
//	}
//	// 隐藏光标
//	hideCursor();
//}

// 初始化控制台窗口
void Renderer::init(int16_t row, int16_t column, const char *title)
{
	// 设置窗口长宽
	SMALL_RECT rect = { 0, 0, column - 1, row - 1 };
	SetConsoleWindowInfo(handle, TRUE, &rect);
	// 当屏幕缓冲区长宽与窗口长宽一致时，隐藏横纵滚动条
	SetConsoleScreenBufferSize(handle, COORD{ column, row });
	// 固定窗口大小
	SetWindowLongPtr(
		GetConsoleWindow(), // 获取控制台窗口句柄
		GWL_STYLE, // 指定修改窗口样式
		GetWindowLongPtr(GetConsoleWindow(), GWL_STYLE) // 获取窗口原样式
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX); // 窗口大小设为无法改变，最大化按钮设为不活跃
	// 隐藏光标
	hideCursor();
	// 设置窗口标题
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

// 隐藏光标
//void Renderer::hideCursor()
//{
//	CONSOLE_CURSOR_INFO info;
//	GetConsoleCursorInfo(handle, &info);
//	info.bVisible = FALSE;
//	SetConsoleCursorInfo(handle, &info);
//}

// 隐藏光标
void Renderer::hideCursor()
{
	std::cout << "\033[?25l";
}

// 显示光标
//void Renderer::showCursor()
//{
//	CONSOLE_CURSOR_INFO info;
//	GetConsoleCursorInfo(handle, &info);
//	info.bVisible = TRUE;
//	SetConsoleCursorInfo(handle, &info);
//}

// 显示光标
void Renderer::showCursor()
{
	std::cout << "\33[?25h";
}

// 移动光标
//void Renderer::moveCursor(int16_t x, int16_t y)
//{
//	COORD coord = { x, y };
//	SetConsoleCursorPosition(handle, coord);
//}

// 移动光标
void Renderer::moveCursor(int16_t x, int16_t y)
{
	std::cout << "\033[" << y << ';' << x << 'H';
}
