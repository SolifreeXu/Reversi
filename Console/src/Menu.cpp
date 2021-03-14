#include "Menu.hpp"
#include "Renderer.hpp"

#include <Windows.h>
#include <conio.h>

#include <cstring>

REVERSI_BEGIN

// 黑色背景
static constexpr uint16_t COLOR_SCREEN = 0;
// 淡黄色前景
static constexpr uint16_t COLOR_VERSE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
// 亮白色前景
static constexpr uint16_t COLOR_FRAME = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
// 亮红色前景
static constexpr uint16_t COLOR_RESUME = FOREGROUND_RED | FOREGROUND_INTENSITY;
// 亮蓝色前景
static constexpr uint16_t COLOR_RESTART = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
// 亮绿色前景
static constexpr uint16_t COLOR_EXIT = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

// 纵横黑白
static const char TEXT_VERSE[][3] = { "胸", "罗", "万", "象", "纵", "横", "有", \
	"目", "悦", "群", "情", "黑", "白", "明", \
	"如", "今", "纵", "横", "已", "相", "知", \
	"缘", "何", "黑", "白", "难", "相", "融" };

// 菜单标题
static const char *TEXT_TITLE = "黑白棋";
// 菜单选项
static const char *TEXT_RESUME = "继续游戏";
static const char *TEXT_RESTART = "重新开始";
static const char *TEXT_EXIT = "退出游戏";

// 边框横线
static const char *TEXT_LINE = "◆◆◆◆◆◆◆◆◆◆◆";
// 边框点形
static const char *TEXT_POINT = "◆";
// 光标字形
static const char *TEXT_CURSOR = "☆";

// 获取句列间距
inline int16_t Menu::getSpace(int16_t _width) const noexcept
{
	return (_width - _size * _number) / (_number + 1);
}

// 刷新光标
void Menu::refreshCursor(const char* _cursor)
{
	using std::strlen;

	switch (_choice)
	{
	case 0: // 退出游戏
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_EXIT))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 2, _cursor);
		break;

	case 1: // 重新开始
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_RESTART))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 4, _cursor);
		break;

	case 2: // 继续游戏
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_RESUME))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 6, _cursor);
		break;

	default:
		break;
	}
}

// 修改选择
void Menu::modifyChoice(int _delta) noexcept
{
	// 选择编号变化
	_choice += _delta;

	// 范围容错处理
	if (_choice < 0)
		_choice = _stored ? 2 : 1;
	else if (_stored ? _choice > 2 : _choice >= 2)
		_choice = 0;
}

// 更新菜单
int Menu::update()
{
	// 是否按键
	if (_kbhit() == 0)
		return 0;

	// 接收按键
	bool even = false;
	int key = _getch();
	if (key == 224)
	{
		key = _getch();
		even = true;
	}

	// 隐藏光标
	refreshCursor("  ");
	Sleep(40);

	// 修改选择
	if (even) // 双键
	{
		if (key == UP) // 上方向键
			modifyChoice(1);
		else if (key == DOWN) // 下方向键
			modifyChoice(-1);
	}
	return key;
}

Menu::Menu(Renderer& _renderer) noexcept
	: _renderer(_renderer), _stored(false), _choice(1)
{
	using std::strlen;

	// 计算菜单边框的宽度和高度
	_width = static_cast<decltype(_width)>(strlen(TEXT_LINE));
	_height = 3 + 4 + 6;

	// 计算菜单边界
	_up = (_renderer.getRow() - _height) / 2;
	_down = _up + _height;
	_left = (_renderer.getColumn() - _width) / 2;
	_right = _left + _width;

	// 计算字宽度与句数量
	_size = static_cast<decltype(_size)>(strlen(TEXT_VERSE[0]));
	_number = sizeof TEXT_VERSE / sizeof TEXT_VERSE[0] / 7 / 2;
}

// 显示菜单
void Menu::show()
{
	// 设置窗口颜色
	_renderer.setScreenColor(COLOR_SCREEN);

	// 设置字体颜色
	_renderer.setTextColor(COLOR_VERSE, COLOR_SCREEN);

	// 打印前一句
	int16_t counter = 0;
	int16_t space = getSpace(_left);
	for (int16_t column = _left - (space + _size) * _number, index = 0; \
		index < _number; ++index, column += space + _size)
		for (int16_t row = _up; row < _down; row += 2)
		{
			Sleep(68); // 两字间隔68ms
			_renderer.outText(column, row, TEXT_VERSE[counter++]);
		}

	// 打印后一句
	space = getSpace(_renderer.getColumn() - _right);
	for (int16_t column = _right + space, index = 0; \
		index < _number; ++index, column += space + _size)
		for (int16_t row = _up; row < _down; row += 2)
		{
			Sleep(68);
			_renderer.outText(column, row, TEXT_VERSE[counter++]);
		}

	// 打印菜单边框
	Sleep(96);
	_y = _up;
	_renderer.setTextColor(COLOR_FRAME, COLOR_SCREEN);
	_renderer.outText(_left, _y, TEXT_LINE);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_TITLE);
	_renderer.outText(_left, _y += 2, TEXT_LINE);

	// 继续游戏
	if (_stored)
	{
		_renderer.setTextColor(COLOR_RESUME, COLOR_SCREEN);
		_y += 2;
		_renderer.outText(_left, _right, _y, _y + 1, TEXT_RESUME);
	}

	// 重新开始
	_renderer.setTextColor(COLOR_RESTART, COLOR_SCREEN);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_RESTART);

	// 退出游戏
	_renderer.setTextColor(COLOR_EXIT, COLOR_SCREEN);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_EXIT);

	// 打印菜单边框
	_renderer.setTextColor(COLOR_FRAME, COLOR_SCREEN);
	_renderer.outText(_left, _y += 2, TEXT_LINE);
	for (int16_t row = _up + 1; row < _y; ++row)
	{
		_renderer.outText(_left, row, TEXT_POINT);
		_renderer.outText(_right - 2, row, TEXT_POINT);
	}

	// 刷新光标
	refreshCursor(TEXT_CURSOR);
}

// 隐藏菜单
void Menu::hide() const
{
	_renderer.clearScreen();
}

// 菜单循环
int Menu::loop()
{
	int key;
	do
	{
		refreshCursor(TEXT_CURSOR);
		Sleep(40);
		key = update();
	} while (key != ENTER && key != SPACE); // 按Enter或Space确定选项
	return _choice;
}

REVERSI_END
