#include "Menu.hpp"
#include "Renderer.hpp"

#include <Windows.h>
#include <conio.h>

#include <cstring>

REVERSI_BEGIN

// ��ɫ����
static constexpr uint16_t COLOR_SCREEN = 0;
// ����ɫǰ��
static constexpr uint16_t COLOR_VERSE = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
// ����ɫǰ��
static constexpr uint16_t COLOR_FRAME = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
// ����ɫǰ��
static constexpr uint16_t COLOR_RESUME = FOREGROUND_RED | FOREGROUND_INTENSITY;
// ����ɫǰ��
static constexpr uint16_t COLOR_RESTART = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
// ����ɫǰ��
static constexpr uint16_t COLOR_EXIT = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

// �ݺ�ڰ�
static const char TEXT_VERSE[][3] = { "��", "��", "��", "��", "��", "��", "��", \
	"Ŀ", "��", "Ⱥ", "��", "��", "��", "��", \
	"��", "��", "��", "��", "��", "��", "֪", \
	"Ե", "��", "��", "��", "��", "��", "��" };

// �˵�����
static const char *TEXT_TITLE = "�ڰ���";
// �˵�ѡ��
static const char *TEXT_RESUME = "������Ϸ";
static const char *TEXT_RESTART = "���¿�ʼ";
static const char *TEXT_EXIT = "�˳���Ϸ";

// �߿����
static const char *TEXT_LINE = "����������������������";
// �߿����
static const char *TEXT_POINT = "��";
// �������
static const char *TEXT_CURSOR = "��";

// ��ȡ���м��
inline int16_t Menu::getSpace(int16_t _width) const noexcept
{
	return (_width - _size * _number) / (_number + 1);
}

// ˢ�¹��
void Menu::refreshCursor(const char* _cursor)
{
	using std::strlen;

	switch (_choice)
	{
	case 0: // �˳���Ϸ
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_EXIT))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 2, _cursor);
		break;

	case 1: // ���¿�ʼ
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_RESTART))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 4, _cursor);
		break;

	case 2: // ������Ϸ
		_x = (_width - static_cast<decltype(_width)>(strlen(TEXT_RESUME))) / 2;
		_x = _left + _x / 2 + 1;
		_renderer.outText(_x, _y - 6, _cursor);
		break;

	default:
		break;
	}
}

// �޸�ѡ��
void Menu::modifyChoice(int _delta) noexcept
{
	// ѡ���ű仯
	_choice += _delta;

	// ��Χ�ݴ���
	if (_choice < 0)
		_choice = _stored ? 2 : 1;
	else if (_stored ? _choice > 2 : _choice >= 2)
		_choice = 0;
}

// ���²˵�
int Menu::update()
{
	// �Ƿ񰴼�
	if (_kbhit() == 0)
		return 0;

	// ���հ���
	bool even = false;
	int key = _getch();
	if (key == 224)
	{
		key = _getch();
		even = true;
	}

	// ���ع��
	refreshCursor("  ");
	Sleep(40);

	// �޸�ѡ��
	if (even) // ˫��
	{
		if (key == UP) // �Ϸ����
			modifyChoice(1);
		else if (key == DOWN) // �·����
			modifyChoice(-1);
	}
	return key;
}

Menu::Menu(Renderer& _renderer) noexcept
	: _renderer(_renderer), _stored(false), _choice(1)
{
	using std::strlen;

	// ����˵��߿�Ŀ�Ⱥ͸߶�
	_width = static_cast<decltype(_width)>(strlen(TEXT_LINE));
	_height = 3 + 4 + 6;

	// ����˵��߽�
	_up = (_renderer.getRow() - _height) / 2;
	_down = _up + _height;
	_left = (_renderer.getColumn() - _width) / 2;
	_right = _left + _width;

	// �����ֿ���������
	_size = static_cast<decltype(_size)>(strlen(TEXT_VERSE[0]));
	_number = sizeof TEXT_VERSE / sizeof TEXT_VERSE[0] / 7 / 2;
}

// ��ʾ�˵�
void Menu::show()
{
	// ���ô�����ɫ
	_renderer.setScreenColor(COLOR_SCREEN);

	// ����������ɫ
	_renderer.setTextColor(COLOR_VERSE, COLOR_SCREEN);

	// ��ӡǰһ��
	int16_t counter = 0;
	int16_t space = getSpace(_left);
	for (int16_t column = _left - (space + _size) * _number, index = 0; \
		index < _number; ++index, column += space + _size)
		for (int16_t row = _up; row < _down; row += 2)
		{
			Sleep(68); // ���ּ��68ms
			_renderer.outText(column, row, TEXT_VERSE[counter++]);
		}

	// ��ӡ��һ��
	space = getSpace(_renderer.getColumn() - _right);
	for (int16_t column = _right + space, index = 0; \
		index < _number; ++index, column += space + _size)
		for (int16_t row = _up; row < _down; row += 2)
		{
			Sleep(68);
			_renderer.outText(column, row, TEXT_VERSE[counter++]);
		}

	// ��ӡ�˵��߿�
	Sleep(96);
	_y = _up;
	_renderer.setTextColor(COLOR_FRAME, COLOR_SCREEN);
	_renderer.outText(_left, _y, TEXT_LINE);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_TITLE);
	_renderer.outText(_left, _y += 2, TEXT_LINE);

	// ������Ϸ
	if (_stored)
	{
		_renderer.setTextColor(COLOR_RESUME, COLOR_SCREEN);
		_y += 2;
		_renderer.outText(_left, _right, _y, _y + 1, TEXT_RESUME);
	}

	// ���¿�ʼ
	_renderer.setTextColor(COLOR_RESTART, COLOR_SCREEN);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_RESTART);

	// �˳���Ϸ
	_renderer.setTextColor(COLOR_EXIT, COLOR_SCREEN);
	_y += 2;
	_renderer.outText(_left, _right, _y, _y + 1, TEXT_EXIT);

	// ��ӡ�˵��߿�
	_renderer.setTextColor(COLOR_FRAME, COLOR_SCREEN);
	_renderer.outText(_left, _y += 2, TEXT_LINE);
	for (int16_t row = _up + 1; row < _y; ++row)
	{
		_renderer.outText(_left, row, TEXT_POINT);
		_renderer.outText(_right - 2, row, TEXT_POINT);
	}

	// ˢ�¹��
	refreshCursor(TEXT_CURSOR);
}

// ���ز˵�
void Menu::hide() const
{
	_renderer.clearScreen();
}

// �˵�ѭ��
int Menu::loop()
{
	int key;
	do
	{
		refreshCursor(TEXT_CURSOR);
		Sleep(40);
		key = update();
	} while (key != ENTER && key != SPACE); // ��Enter��Spaceȷ��ѡ��
	return _choice;
}

REVERSI_END
