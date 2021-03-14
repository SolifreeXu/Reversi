#include "Chessboard.hpp"
#include "Renderer.hpp"

#include <Windows.h>
#include <conio.h>

#include <cstring>
#include <istream>
#include <ostream>
#include <iomanip>

REVERSI_BEGIN

// ����ɫ����
static constexpr uint16_t SCREEN = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
// ����ɫǰ��
static constexpr uint16_t PROMPT = FOREGROUND_RED | FOREGROUND_INTENSITY;
// ��ɫǰ��������ɫ����
static constexpr uint16_t BOARD[2] = { FOREGROUND_RED, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY };
// ����ɫ����������ɫǰ������ɫǰ��
static constexpr uint16_t CHESS[3] = { BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY, 0, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY };

// �±�˵����
const Chessboard::Index Chessboard::_index = Chessboard::Index();
// ����˵����
const Chessboard::Coordinate Chessboard::_coordinate = Chessboard::Coordinate();

std::istream& operator >>(std::istream& _input, Chessboard& _chessboard)
{
	return _input.read(reinterpret_cast<char*>(_chessboard._states), sizeof _chessboard._states);
}

std::ostream& operator <<(std::ostream& _output, const Chessboard& _chessboard)
{
	return _output.write(reinterpret_cast<const char*>(_chessboard._states), sizeof _chessboard._states);
}

// ת������Ϊ�±�
inline void Chessboard::getIndex(int16_t _x, int16_t _y, int16_t& _row, int16_t& _column) const noexcept
{
	_row = (_y - (_up + 1)) >> 1;
	_column = (_x - (_left + 2)) >> 2;
}

// ת���±�Ϊ����
inline void Chessboard::getCoord(int16_t _row, int16_t _column, int16_t& _x, int16_t& _y) const noexcept
{
	_x = (_column << 2) + (_left + 2);
	_y = (_row << 1) + (_up + 1);
}

// �ж�ָ���±��Ƿ�����
inline bool Chessboard::isChess(Index _index, int16_t _row, int16_t _column) const noexcept
{
	return _states[_row][_column] != EMPTY;
}

// �ж�ָ�������Ƿ�����
bool Chessboard::isChess(Coordinate _coordinate, int16_t _x, int16_t _y) const noexcept
{
	int16_t row, column;
	getIndex(_x, _y, row, column);
	return isChess(_index, row, column);
}

// ��������״̬
inline void Chessboard::setChess(int16_t _row, int16_t _column, State _state) noexcept
{
	_states[_row][_column] = _state;
}

// ���ù����ɫ
void Chessboard::setCursorColor() const
{
	_renderer.setTextColor(CHESS[1], CHESS[0]);
}

// ����������ɫ
void Chessboard::setChessColor(int16_t _x, int16_t _y) const
{
	int16_t row, column;
	getIndex(_x, _y, row, column);
	_renderer.setTextColor(CHESS[_states[row][column]], CHESS[0]);
}

// ��������
void Chessboard::drawChessboard()
{
	_renderer.setTextColor(BOARD[0], BOARD[1]);
	_stream.seekp(0, _stream.beg);
	_stream << "��";
	for (uint8_t j = COLUMN - 1; j > 0; --j)
		_stream << "����";
	_stream << "����";
	const int16_t& x = _left;
	int16_t y = _up;
	_renderer.outText(x, y, _stream.str().c_str());

	for (uint8_t i = ROW - 1; i > 0; --i)
	{
		_stream.seekp(0, _stream.beg);
		for (uint8_t j = 0; j < COLUMN; ++j)
			_stream << "��  ";
		_stream << "��";
		_renderer.outText(x, ++y, _stream.str().c_str());

		_stream.seekp(0, _stream.beg);
		_stream << "��";
		for (uint8_t j = COLUMN - 1; j > 0; --j)
			_stream << "����";
		_stream << "����";
		_renderer.outText(x, ++y, _stream.str().c_str());
	}

	_stream.seekp(0, _stream.beg);
	for (uint8_t j = 0; j < COLUMN; ++j)
		_stream << "��  ";
	_stream << "��";
	_renderer.outText(x, ++y, _stream.str().c_str());

	_stream.seekp(0, _stream.beg);
	_stream << "��";
	for (uint8_t j = COLUMN - 1; j > 0; --j)
		_stream << "����";
	_stream << "����";
	_renderer.outText(x, ++y, _stream.str().c_str());
}

// ������������
void Chessboard::drawChess() const
{
	for (int16_t row = 0, column, x, y; row < ROW; ++row)
		for (column = 0; column < COLUMN; ++column)
			if (isChess(_index, row, column))
			{
				getCoord(row, column, x, y);
				setChessColor(x, y);
				_renderer.outText(x, y, "��");
			}
}

// ת����������
void Chessboard::convertOne(int16_t _row, int16_t _column, State _state)
{
	Sleep(40);
	setChess(_row, _column, _state);
	int16_t x, y;
	getCoord(_row, _column, x, y);
	setChessColor(x, y);
	_renderer.outText(x, y, "��");
}

// ת���Ϸ�����
bool Chessboard::convertUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row <= 1)
		return false;

	// ���Ϸ�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row - 1;
	for (; m >= 0; --m)
		if (_states[m][_column] == _states[_row][_column] \
			|| _states[m][_column] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m < 0 || m == _row - 1 || _states[m][_column] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row - 1; i > m; --i)
			convertOne(i, _column, _states[_row][_column]);
	}
	return true;
}

// ת���·�����
bool Chessboard::convertDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row >= ROW - 2)
		return false;

	// ���·�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row + 1;
	for (; m < ROW; ++m)
		if (_states[m][_column] == _states[_row][_column] \
			|| _states[m][_column] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m >= ROW || m == _row + 1 || _states[m][_column] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row + 1; i < m; ++i)
			convertOne(i, _column, _states[_row][_column]);
	}
	return true;
}

// ת��������
bool Chessboard::convertLeft(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_column <= 1)
		return false;

	// ���󷽱�����ֱ���Լ������ӻ��߿�λΪֹ
	int16_t n = _column - 1;
	for (; n >= 0; --n)
		if (_states[_row][n] == _states[_row][_column] \
			|| _states[_row][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (n < 0 || n == _column - 1 || _states[_row][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t j = _column - 1; j > n; --j)
			convertOne(_row, j, _states[_row][_column]);
	}
	return true;
}

// ת���ҷ�����
bool Chessboard::convertRight(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_column >= COLUMN - 2)
		return false;

	// ���ҷ�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t n = _column + 1;
	for (; n < COLUMN; ++n)
		if (_states[_row][n] == _states[_row][_column] \
			|| _states[_row][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (n >= COLUMN || n == _column + 1 || _states[_row][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t j = _column + 1; j < n; ++j)
			convertOne(_row, j, _states[_row][_column]);
	}
	return true;
}

// ת����������
bool Chessboard::convertLeftUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row <= 1 || _column <= 1)
		return false;

	// �����Ϸ�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row - 1, n = _column - 1;
	for (; m >= 0 && n >= 0; --m, --n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m < 0 || n < 0 || m == _row - 1 || _states[m][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row - 1, j = _column - 1; i > m; --i, --j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// ת����������
bool Chessboard::convertRightUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row <= 1 || _column >= COLUMN - 2)
		return false;

	// �����Ϸ�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row - 1, n = _column + 1;
	for (; m >= 0 && n < COLUMN; --m, ++n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m < 0 || n >= COLUMN || m == _row - 1 || _states[m][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row - 1, j = _column + 1; i > m; --i, ++j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// ת����������
bool Chessboard::convertLeftDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row >= ROW - 2 || _column <= 1)
		return false;

	// �����·�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row + 1, n = _column - 1;
	for (; m < ROW && n >= 0; ++m, --n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m >= ROW || n < 0 || m == _row + 1 || _states[m][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row + 1, j = _column - 1; i < m; ++i, --j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// ת����������
bool Chessboard::convertRightDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// ��ת���ռ�
	if (_row >= ROW - 2 || _column >= COLUMN - 2)
		return false;

	// �����·�������ֱ���Լ������ӻ��߿�λΪֹ
	int16_t m = _row + 1, n = _column + 1;
	for (; m < ROW && n < COLUMN; ++m, ++n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// �����߽���߲���ת����������
	if (m >= ROW || n >= COLUMN || m == _row + 1 || _states[m][n] == EMPTY)
		return false;

	// ���ǳ���ת����������ת������
	if (!_judgmental)
	{
		for (int16_t i = _row + 1, j = _column + 1; i < m; ++i, ++j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// ת������
bool Chessboard::convert(int16_t _row, int16_t _column, bool _judgmental)
{
	// ת���Ϸ�����
	bool result = convertUp(_row, _column, _judgmental);
	bool convertible = result;
	if (_judgmental && convertible)
		return convertible;

	// ת���·�����
	result = convertDown(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת��������
	result = convertLeft(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת���ҷ�����
	result = convertRight(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת����������
	result = convertLeftUp(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת����������
	result = convertRightUp(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת����������
	result = convertLeftDown(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// ת����������
	result = convertRightDown(_row, _column, _judgmental);
	convertible = convertible || result;
	return convertible;
}

// �ȴ�����
void Chessboard::wait() const
{
	do
	{
		// ��ʾ���
		setCursorColor();
		_renderer.outText(_x, _y, "��");
		Sleep(40);

		// �������λ��������
		if (isChess(_coordinate, _x, _y))
		{
			// ��ʾ����
			setChessColor(_x, _y);
			_renderer.outText(_x, _y, "��");
			Sleep(40);
		}
	} while (!_kbhit()); // �����˳�ѭ��

	// �������λ��������
	if (!isChess(_coordinate, _x, _y))
		_renderer.outText(_x, _y, "  "); // ������
}

// �����¼�
int Chessboard::handle()
{
	// ���հ���
	bool even = false;
	int key = _getch();
	if (key == 224)
	{
		key = _getch();
		even = true;
	}

	// ����˫���򷵻�
	if (!even)
		return key;

	switch (key)
	{
	case UP: // �Ϸ����
		key = 0;
		_y -= 2;
		if (_y < _up)
			_y = _down - 2;
		break;

	case DOWN: // �·����
		key = 0;
		_y += 2;
		if (_y >= _down)
			_y = _up + 1;
		break;

	case LEFT: // �����
		key = 0;
		_x -= 4;
		if (_x < _left)
			_x = _right - 4;
		break;

	case RIGHT: // �ҷ����
		key = 0;
		_x += 4;
		if (_x >= _right)
			_x = _left + 2;
		break;

	default:
		break;
	}
	return key;
}

Chessboard::Chessboard(Renderer& _renderer) noexcept
	: _renderer(_renderer)
{
	// �������̵Ŀ�Ⱥ͸߶�
	_height = ROW + (ROW + 1);
	_width = (COLUMN + COLUMN + 1) * 2;

	// �������̱߽�
	_up = (_renderer.getRow() - _height) / 2;
	_down = _up + _height;
	_left = (_renderer.getColumn() - _width) / 2;
	_right = _left + _width;

	// ��ʼ���λ��
	getCoord(ROW / 2 - 1, COLUMN / 2 - 1, _x, _y);
}

// ��������Ч��
Chessboard::operator bool() const noexcept
{
	for (uint8_t row = 0; row < ROW; ++row)
		for (uint8_t column = 0; column < COLUMN; ++column)
			if (_states[row][column] < EMPTY || _states[row][column] > WHITE)
				return false;
	return true;
}

// ��ʼ����
void Chessboard::init() noexcept
{
	// ��ʼ������״̬
	memset(_states, EMPTY, sizeof _states);
	uint8_t row = ROW / 2 - 1, column = COLUMN / 2 - 1;
	_states[row][column + 1] = _states[row + 1][column] = BLACK;
	_states[row][column] = _states[row + 1][column + 1] = WHITE;
}

// ��ʾ���
void Chessboard::show()
{
	// ���ô�����ɫ
	_renderer.setScreenColor(SCREEN);

	// ��������
	drawChessboard();

	// ��������
	drawChess();

	// ����������ɫ
	_renderer.setTextColor(PROMPT, SCREEN);

	// ��ӡ������
	int16_t x = (_left - 8) / 2, y = _renderer.getRow() / 2;
	_renderer.outText(x, y, "���ӣ�");
	int16_t column = _renderer.getColumn();
	x = (column + _right - 8) / 2;
	_renderer.outText(x, y, "���ӣ�");

	// ��ӡ�غ�����ִ������
	x = (column - 8) / 2;
	y = (_up - 2) / 2;
	_renderer.outText(x, y, "�غϣ�");
	_renderer.outText(x, y + 1, "ִ�ӣ�");
}

// �������
void Chessboard::hide() const
{
	_renderer.clearScreen();
}

// �������
void Chessboard::update(uint8_t _round, State _state)
{
	using std::setw;

	// ����˫��������
	_numbers[State::BLACK - 1] = _numbers[State::WHITE - 1] = 0;
	for (uint8_t row = 0; row < ROW; ++row)
		for (uint8_t column = 0; column < COLUMN; ++column)
			if (isChess(_index, row, column))
				++_numbers[_states[row][column] - 1];

	// ����������ɫ
	_renderer.setTextColor(PROMPT, SCREEN);

	// ��ӡ������
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_numbers[State::BLACK - 1]) << std::ends;
	int16_t x = (_left - 8) / 2 + 6, y = _renderer.getRow() / 2;
	_renderer.outText(x, y, _stream.str().c_str());

	// ��ӡ������
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_numbers[State::WHITE - 1]) << std::ends;
	int16_t column = _renderer.getColumn();
	x = (column + _right - 8) / 2 + 6;
	_renderer.outText(x, y, _stream.str().c_str());

	// ��ӡ�غ���
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_round) << std::ends;
	x = (column - 8) / 2 + 6;
	y = (_up - 2) / 2;
	_renderer.outText(x, y, _stream.str().c_str());

	// ��ӡִ������
	_renderer.outText(x, y + 1, _state == BLACK ? "��" : "��");
}

// �ж��ܷ�����
bool Chessboard::operable(State _state)
{
	for (int16_t row = 0; row < ROW; ++row)
		for (int16_t column = 0; column < COLUMN; ++column)
		{
			// ��ǰλ��������
			if (isChess(_index, row, column))
				continue;

			// ����ת��
			setChess(row, column, _state);
			bool convertible = convert(row, column, true);
			setChess(row, column, EMPTY);

			// �Ƿ��ܹ�ת��
			if (convertible)
				return true;
		}
	return false;
}

// ���ӿ���
void Chessboard::skip() const
{
	int16_t row = _renderer.getRow(), column = _renderer.getColumn();
	_renderer.outText(0, column, _down, row, "���ӿ��䣡");
	Sleep(500);
	_renderer.outText(0, column, _down, row, "          ");
}

// �ж���ֽ���
bool Chessboard::judge() const noexcept
{
	// �Ƿ�һ�����ӻ�����������
	return _numbers[0] == 0 || _numbers[1] == 0 \
		|| _numbers[0] + _numbers[1] == ROW * COLUMN;
}

// չʾ���
void Chessboard::finish(bool _inoperable) const
{
	// ����������ɫ
	_renderer.setTextColor(PROMPT, SCREEN);

	// ˫������������δ������˫�����ӿ���
	if (_inoperable)
	{
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "˫�����ӿ��䣡");
		Sleep(500);
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "              ");
	}

	const char* result;
	if (_numbers[State::BLACK - 1] > _numbers[State::WHITE - 1])
		result = "����ʤ��";
	else if (_numbers[State::BLACK - 1] < _numbers[State::WHITE - 1])
		result = "����ʤ��";
	else
		result = "ƽ�֣�";
	_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), result);
	Sleep(500);
}

// ���Ӳ���
bool Chessboard::put(State _state)
{
	// �����±�
	int16_t row, column;
	getIndex(_x, _y, row, column);

	// ��ǰλ��������
	if (isChess(_index, row, column))
		return false;

	// �������Ӳ�����ת��
	setChess(row, column, _state);
	if (!convert(row, column, true))
	{
		// ת��ʧ�ܻ��˲���
		setChess(row, column, EMPTY);
		return false;
	}

	// ת���ɹ���չʾת������
	convert(row, column, false);
	return true;
}

// ����ѭ��
int Chessboard::loop()
{
	using std::setw;

	// ��������
	int key;
	do
	{
		wait();
		key = handle();
	} while (key == 0); // ��ֵΪ�㣬˵����ֲ��񰴼����������հ���

	// ��ֹ����
	if (key == ESCAPE)
	{
		_renderer.setTextColor(PROMPT, SCREEN);
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "��ֹ���ģ�");
		Sleep(500);
	}
	return key;
}

REVERSI_END
