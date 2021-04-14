#include "Renderer.hpp"

#include <Windows.h>

//#include <cstdlib>
//#include <cstring>
//#include <string>
#include <iostream>
#include <sstream>

// 初始化窗口
//void Renderer::init(const char *_title) const
//{
//	// 设置窗口长宽
//	std::string command = "mode con lines=" + std::to_string(_row) + " cols=" + std::to_string(_column);
//	std::system(command.c_str());
//
//	// 设置窗口标题
//	if (_title)
//	{
//		command.assign("title ").append(_title);
//		std::system(command.c_str());
//	}
//
//	// 隐藏光标
//	hideCursor();
//}

// 初始化窗口
void Renderer::init(const char* _title) const
{
	// 设置窗口长宽
	SMALL_RECT rect = { 0, 0, _column - 1, _row - 1 };
	SetConsoleWindowInfo(_handle, TRUE, &rect);

	// 当屏幕缓冲区长宽与窗口长宽一致时，隐藏横纵滚动条
	SetConsoleScreenBufferSize(_handle, COORD{ _column, _row });

	// 固定窗口大小
	SetWindowLongPtr(
		GetConsoleWindow(), // 获取控制台窗口句柄
		GWL_STYLE, // 指定修改窗口样式
		GetWindowLongPtr(GetConsoleWindow(), GWL_STYLE) // 获取窗口原样式
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX); // 窗口大小设为无法改变，最大化按钮设为不活跃

	// 设置窗口标题
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

	// 隐藏光标
	hideCursor();
}

// 隐藏光标
void Renderer::hideCursor() const
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(_handle, &info);
	info.bVisible = FALSE;
	SetConsoleCursorInfo(_handle, &info);
}

// 隐藏光标（不兼容旧版控制台）
//void Renderer::hideCursor() const
//{
//	std::cout << "\033[?25l";
//}

// 显示光标
void Renderer::showCursor() const
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(_handle, &info);
	info.bVisible = TRUE;
	SetConsoleCursorInfo(_handle, &info);
}

// 显示光标（不兼容旧版控制台）
//void Renderer::showCursor() const
//{
//	std::cout << "\033[?25h";
//}

// 移动光标
void Renderer::moveCursor(int16_t _x, int16_t _y) const
{
	COORD coord = { _x, _y };
	SetConsoleCursorPosition(_handle, coord);
}

// 移动光标（不兼容旧版控制台）
//void Renderer::moveCursor(int16_t _x, int16_t _y) const
//{
//	std::cout << "\033[" << _y << ';' << _x << 'H';
//	// \033[0m 禁用所有属性
//	// \033[1m 高亮
//	// \033[4m 下划线
//	// \033[7m 反显（交换前景色与背景色）
//	// \033[nA 光标上移n行
//	// \033[nB 光标下移n行
//	// \033[nC 光标右移n列
//	// \033[nD 光标左移n列
//	// \033[K 清除光标至行尾的内容，即基于当前属性填充光标至行尾
//	// \033[s 保存光标位置
//	// \033[u 恢复光标位置
//}

Renderer::Renderer(int16_t _row, int16_t _column, const char *_title)
	: _handle(GetStdHandle(STD_OUTPUT_HANDLE)), _row(_row), _column(_column)
{
	init(_title);
}

// 清空控制台窗口
//void Renderer::clearScreen() const
//{
//	std::system("cls");
//}

// 清空控制台窗口
void Renderer::clearScreen() const
{
	// 获取控制台缓冲区信息
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(_handle, &info);

	// 以空格填充整个控制台
	DWORD size = info.dwSize.X * info.dwSize.Y; // 缓冲区的字符容纳量
	DWORD number;
	COORD coord = { 0, 0 };
	FillConsoleOutputCharacter(_handle, \
		static_cast<TCHAR>(' '), size, coord, &number);

	// 再次获取缓冲区信息
	GetConsoleScreenBufferInfo(_handle, &info);

	// 以默认颜色填充控制台
	FillConsoleOutputAttribute(_handle, \
		info.wAttributes, size, coord, &number);

	// 移动光标至(0, 0)
	SetConsoleCursorPosition(_handle, coord);
}

// 清空控制台窗口（不兼容旧版控制台）
//void Renderer::clearScreen() const
//{
//	std::cout << "\033[2J" << "\033[0;0H";
//}

/*
 * 设置窗口颜色
 * 参数：
 *     _background    背景色
 *     _foreground    前景色
 */
//void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
//{
//	std::ostringstream output;
//	output << "color " << std::hex << (_background >> 4 & 0x0F) << (_foreground & 0x0F);
//	system(output.str().c_str());
//}

// 设置窗口颜色
void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
{
	// 获取缓冲区信息
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(_handle, &info);

	// 以默认颜色填充控制台
	DWORD size = info.dwSize.X * info.dwSize.Y; // 缓冲区的字符容纳量
	DWORD number;
	COORD coord = { 0, 0 };
	FillConsoleOutputAttribute(_handle, \
		_background, size, coord, &number);

	// 移动光标至(0, 0)
	SetConsoleCursorPosition(_handle, coord);

	// 设置字体颜色
	setTextColor(_foreground, _background);
}

/*
 * 交换红色位和蓝色位
 * 参数：
 *     _color    颜色
 *     _offset   左向偏移
 */
//static uint16_t swapRedAndBlue(uint16_t _color, uint16_t _offset) noexcept
//{
//	// 获取红色位或蓝色位
//	uint16_t temp = _color & 0x1 << _offset;
//
//	// 清零色位并赋值为另一色位
//	_color = _color & ~(0x1 << _offset) | (_color & 0x4 << _offset) >> 2;
//	return _color & ~(0x4 << _offset) | temp << 2;
//}

// 设置窗口颜色（不兼容旧版控制台）
//void Renderer::setScreenColor(uint16_t _background, uint16_t _foreground) const
//{
//	// 交换色位
//	uint16_t color = swapRedAndBlue(_background, 4);
//
//	// 设置背景色
//	std::ostringstream output;
//	output << "\033[" << (color >> 7 & 0x01) << ";4" << (color >> 4 & 0x07) << 'm';
//	std::cout << output.str();
//
//	// 填充窗口
//	clearScreen();
//
//	// 清除设置
//	std::cout << "\033[0m";
//
//	// 设置字体颜色
//	setTextColor(_foreground, _background);
//}

// 设置文本颜色
void Renderer::setTextColor(uint16_t _foreground, uint16_t _background) const
{
	SetConsoleTextAttribute(_handle, _foreground & 0x0F | _background & 0xF0);
}

// 设置文本颜色（不兼容旧版控制台）
//void Renderer::setTextColor(uint16_t _foreground, uint16_t _background) const
//{
//	// 交换色位
//	_foreground = swapRedAndBlue(_foreground, 0);
//	_background = swapRedAndBlue(_background, 4);
//
//	// 设置背景色与前景色
//	std::ostringstream output;
//	output << "\033[" << (_background >> 7 & 0x01) << ";4" << (_background >> 4 & 0x07) << 'm'
//		<< "\033[" << (_foreground >> 3 & 0x01) << ";3" << (_foreground & 0x07) << 'm';
//	std::cout << output.str();
//}

// 输出文本
void Renderer::outText(int16_t _x, int16_t _y, const char *_text) const
{
	moveCursor(_x, _y);
	std::cout << _text;
}

// 输出文本至指定范围
void Renderer::outText(int16_t _left, int16_t _right, int16_t _up, int16_t _down, const char *_text) const
{
	auto length = static_cast<decltype(_left)>(std::strlen(_text));
	moveCursor((_left + _right - length) / 2, (_up + _down - 1) / 2);
	std::cout << _text;
}
