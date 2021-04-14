#include "Chessboard.hpp"
#include "Renderer.hpp"

#include <Windows.h>
#include <conio.h>

#include <cstring>
#include <istream>
#include <ostream>
#include <iomanip>

REVERSI_BEGIN

// 亮蓝色背景
static constexpr uint16_t SCREEN = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
// 亮红色前景
static constexpr uint16_t PROMPT = FOREGROUND_RED | FOREGROUND_INTENSITY;
// 红色前景，亮黄色背景
static constexpr uint16_t BOARD[2] = { FOREGROUND_RED, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY };
// 亮黄色背景，亮白色前景，黑色前景
static constexpr uint16_t CHESS[3] = { BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY, 0, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY };

// 下标说明符
const Chessboard::Index Chessboard::_index = Chessboard::Index();
// 坐标说明符
const Chessboard::Coordinate Chessboard::_coordinate = Chessboard::Coordinate();

std::istream& operator >>(std::istream& _input, Chessboard& _chessboard)
{
	return _input.read(reinterpret_cast<char*>(_chessboard._states), sizeof _chessboard._states);
}

std::ostream& operator <<(std::ostream& _output, const Chessboard& _chessboard)
{
	return _output.write(reinterpret_cast<const char*>(_chessboard._states), sizeof _chessboard._states);
}

// 转换坐标为下标
inline void Chessboard::getIndex(int16_t _x, int16_t _y, int16_t& _row, int16_t& _column) const noexcept
{
	_row = (_y - (_up + 1)) >> 1;
	_column = (_x - (_left + 2)) >> 2;
}

// 转换下标为坐标
inline void Chessboard::getCoord(int16_t _row, int16_t _column, int16_t& _x, int16_t& _y) const noexcept
{
	_x = (_column << 2) + (_left + 2);
	_y = (_row << 1) + (_up + 1);
}

// 判断指定下标是否棋子
inline bool Chessboard::isChess(Index _index, int16_t _row, int16_t _column) const noexcept
{
	return _states[_row][_column] != EMPTY;
}

// 判断指定坐标是否棋子
bool Chessboard::isChess(Coordinate _coordinate, int16_t _x, int16_t _y) const noexcept
{
	int16_t row, column;
	getIndex(_x, _y, row, column);
	return isChess(_index, row, column);
}

// 设置棋子状态
inline void Chessboard::setChess(int16_t _row, int16_t _column, State _state) noexcept
{
	_states[_row][_column] = _state;
}

// 设置光标颜色
void Chessboard::setCursorColor() const
{
	_renderer.setTextColor(CHESS[1], CHESS[0]);
}

// 设置棋子颜色
void Chessboard::setChessColor(int16_t _x, int16_t _y) const
{
	int16_t row, column;
	getIndex(_x, _y, row, column);
	_renderer.setTextColor(CHESS[_states[row][column]], CHESS[0]);
}

// 绘制棋盘
void Chessboard::drawChessboard()
{
	_renderer.setTextColor(BOARD[0], BOARD[1]);
	_stream.seekp(0, _stream.beg);
	_stream << "┏";
	for (uint8_t j = COLUMN - 1; j > 0; --j)
		_stream << "━┳";
	_stream << "━┓";
	const int16_t& x = _left;
	int16_t y = _up;
	_renderer.outText(x, y, _stream.str().c_str());

	for (uint8_t i = ROW - 1; i > 0; --i)
	{
		_stream.seekp(0, _stream.beg);
		for (uint8_t j = 0; j < COLUMN; ++j)
			_stream << "┃  ";
		_stream << "┃";
		_renderer.outText(x, ++y, _stream.str().c_str());

		_stream.seekp(0, _stream.beg);
		_stream << "┣";
		for (uint8_t j = COLUMN - 1; j > 0; --j)
			_stream << "━╋";
		_stream << "━┫";
		_renderer.outText(x, ++y, _stream.str().c_str());
	}

	_stream.seekp(0, _stream.beg);
	for (uint8_t j = 0; j < COLUMN; ++j)
		_stream << "┃  ";
	_stream << "┃";
	_renderer.outText(x, ++y, _stream.str().c_str());

	_stream.seekp(0, _stream.beg);
	_stream << "┗";
	for (uint8_t j = COLUMN - 1; j > 0; --j)
		_stream << "━┻";
	_stream << "━┛";
	_renderer.outText(x, ++y, _stream.str().c_str());
}

// 绘制所有棋子
void Chessboard::drawChess() const
{
	for (int16_t row = 0, column, x, y; row < ROW; ++row)
		for (column = 0; column < COLUMN; ++column)
			if (isChess(_index, row, column))
			{
				getCoord(row, column, x, y);
				setChessColor(x, y);
				_renderer.outText(x, y, "●");
			}
}

// 转换单个棋子
void Chessboard::convertOne(int16_t _row, int16_t _column, State _state)
{
	Sleep(40);
	setChess(_row, _column, _state);
	int16_t x, y;
	getCoord(_row, _column, x, y);
	setChessColor(x, y);
	_renderer.outText(x, y, "●");
}

// 转换上方棋子
bool Chessboard::convertUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row <= 1)
		return false;

	// 向上方遍历，直到自己的棋子或者空位为止
	int16_t m = _row - 1;
	for (; m >= 0; --m)
		if (_states[m][_column] == _states[_row][_column] \
			|| _states[m][_column] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m < 0 || m == _row - 1 || _states[m][_column] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row - 1; i > m; --i)
			convertOne(i, _column, _states[_row][_column]);
	}
	return true;
}

// 转换下方棋子
bool Chessboard::convertDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row >= ROW - 2)
		return false;

	// 向下方遍历，直到自己的棋子或者空位为止
	int16_t m = _row + 1;
	for (; m < ROW; ++m)
		if (_states[m][_column] == _states[_row][_column] \
			|| _states[m][_column] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m >= ROW || m == _row + 1 || _states[m][_column] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row + 1; i < m; ++i)
			convertOne(i, _column, _states[_row][_column]);
	}
	return true;
}

// 转换左方棋子
bool Chessboard::convertLeft(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_column <= 1)
		return false;

	// 向左方遍历，直到自己的棋子或者空位为止
	int16_t n = _column - 1;
	for (; n >= 0; --n)
		if (_states[_row][n] == _states[_row][_column] \
			|| _states[_row][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (n < 0 || n == _column - 1 || _states[_row][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t j = _column - 1; j > n; --j)
			convertOne(_row, j, _states[_row][_column]);
	}
	return true;
}

// 转换右方棋子
bool Chessboard::convertRight(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_column >= COLUMN - 2)
		return false;

	// 向右方遍历，直到自己的棋子或者空位为止
	int16_t n = _column + 1;
	for (; n < COLUMN; ++n)
		if (_states[_row][n] == _states[_row][_column] \
			|| _states[_row][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (n >= COLUMN || n == _column + 1 || _states[_row][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t j = _column + 1; j < n; ++j)
			convertOne(_row, j, _states[_row][_column]);
	}
	return true;
}

// 转换左上棋子
bool Chessboard::convertLeftUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row <= 1 || _column <= 1)
		return false;

	// 向左上方遍历，直到自己的棋子或者空位为止
	int16_t m = _row - 1, n = _column - 1;
	for (; m >= 0 && n >= 0; --m, --n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m < 0 || n < 0 || m == _row - 1 || _states[m][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row - 1, j = _column - 1; i > m; --i, --j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// 转换右上棋子
bool Chessboard::convertRightUp(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row <= 1 || _column >= COLUMN - 2)
		return false;

	// 向右上方遍历，直到自己的棋子或者空位为止
	int16_t m = _row - 1, n = _column + 1;
	for (; m >= 0 && n < COLUMN; --m, ++n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m < 0 || n >= COLUMN || m == _row - 1 || _states[m][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row - 1, j = _column + 1; i > m; --i, ++j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// 转换左下棋子
bool Chessboard::convertLeftDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row >= ROW - 2 || _column <= 1)
		return false;

	// 向左下方遍历，直到自己的棋子或者空位为止
	int16_t m = _row + 1, n = _column - 1;
	for (; m < ROW && n >= 0; ++m, --n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m >= ROW || n < 0 || m == _row + 1 || _states[m][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row + 1, j = _column - 1; i < m; ++i, --j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// 转换右下棋子
bool Chessboard::convertRightDown(int16_t _row, int16_t _column, bool _judgmental)
{
	// 无转换空间
	if (_row >= ROW - 2 || _column >= COLUMN - 2)
		return false;

	// 向右下方遍历，直到自己的棋子或者空位为止
	int16_t m = _row + 1, n = _column + 1;
	for (; m < ROW && n < COLUMN; ++m, ++n)
		if (_states[m][n] == _states[_row][_column] \
			|| _states[m][n] == EMPTY)
			break;

	// 超出边界或者不可转换对手棋子
	if (m >= ROW || n >= COLUMN || m == _row + 1 || _states[m][n] == EMPTY)
		return false;

	// 若非尝试转换，则依次转换棋子
	if (!_judgmental)
	{
		for (int16_t i = _row + 1, j = _column + 1; i < m; ++i, ++j)
			convertOne(i, j, _states[_row][_column]);
	}
	return true;
}

// 转换棋子
bool Chessboard::convert(int16_t _row, int16_t _column, bool _judgmental)
{
	// 转换上方棋子
	bool result = convertUp(_row, _column, _judgmental);
	bool convertible = result;
	if (_judgmental && convertible)
		return convertible;

	// 转换下方棋子
	result = convertDown(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换左方棋子
	result = convertLeft(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换右方棋子
	result = convertRight(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换左上棋子
	result = convertLeftUp(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换右上棋子
	result = convertRightUp(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换左下棋子
	result = convertLeftDown(_row, _column, _judgmental);
	convertible = convertible || result;
	if (_judgmental && convertible)
		return convertible;

	// 转换右下棋子
	result = convertRightDown(_row, _column, _judgmental);
	convertible = convertible || result;
	return convertible;
}

// 等待操作
void Chessboard::wait() const
{
	do
	{
		// 显示光标
		setCursorColor();
		_renderer.outText(_x, _y, "◇");
		Sleep(40);

		// 光标所在位置有棋子
		if (isChess(_coordinate, _x, _y))
		{
			// 显示棋子
			setChessColor(_x, _y);
			_renderer.outText(_x, _y, "●");
			Sleep(40);
		}
	} while (!_kbhit()); // 按键退出循环

	// 光标所在位置无棋子
	if (!isChess(_coordinate, _x, _y))
		_renderer.outText(_x, _y, "  "); // 清除光标
}

// 处理事件
int Chessboard::handle()
{
	// 接收按键
	bool even = false;
	int key = _getch();
	if (key == 224)
	{
		key = _getch();
		even = true;
	}

	// 若非双键则返回
	if (!even)
		return key;

	switch (key)
	{
	case UP: // 上方向键
		key = 0;
		_y -= 2;
		if (_y < _up)
			_y = _down - 2;
		break;

	case DOWN: // 下方向键
		key = 0;
		_y += 2;
		if (_y >= _down)
			_y = _up + 1;
		break;

	case LEFT: // 左方向键
		key = 0;
		_x -= 4;
		if (_x < _left)
			_x = _right - 4;
		break;

	case RIGHT: // 右方向键
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
	// 计算棋盘的宽度和高度
	_height = ROW + (ROW + 1);
	_width = (COLUMN + COLUMN + 1) * 2;

	// 计算棋盘边界
	_up = (_renderer.getRow() - _height) / 2;
	_down = _up + _height;
	_left = (_renderer.getColumn() - _width) / 2;
	_right = _left + _width;

	// 初始光标位置
	getCoord(ROW / 2 - 1, COLUMN / 2 - 1, _x, _y);
}

// 检查棋局有效性
Chessboard::operator bool() const noexcept
{
	for (uint8_t row = 0; row < ROW; ++row)
		for (uint8_t column = 0; column < COLUMN; ++column)
			if (_states[row][column] < EMPTY || _states[row][column] > WHITE)
				return false;
	return true;
}

// 初始棋子
void Chessboard::init() noexcept
{
	// 初始化棋子状态
	memset(_states, EMPTY, sizeof _states);
	uint8_t row = ROW / 2 - 1, column = COLUMN / 2 - 1;
	_states[row][column + 1] = _states[row + 1][column] = BLACK;
	_states[row][column] = _states[row + 1][column + 1] = WHITE;
}

// 显示棋局
void Chessboard::show()
{
	// 设置窗口颜色
	_renderer.setScreenColor(SCREEN);

	// 绘制棋盘
	drawChessboard();

	// 绘制棋子
	drawChess();

	// 设置字体颜色
	_renderer.setTextColor(PROMPT, SCREEN);

	// 打印棋子数
	int16_t x = (_left - 8) / 2, y = _renderer.getRow() / 2;
	_renderer.outText(x, y, "黑子：");
	int16_t column = _renderer.getColumn();
	x = (column + _right - 8) / 2;
	_renderer.outText(x, y, "白子：");

	// 打印回合数和执子棋手
	x = (column - 8) / 2;
	y = (_up - 2) / 2;
	_renderer.outText(x, y, "回合：");
	_renderer.outText(x, y + 1, "执子：");
}

// 隐藏棋局
void Chessboard::hide() const
{
	_renderer.clearScreen();
}

// 更新棋局
void Chessboard::update(uint8_t _round, State _state)
{
	using std::setw;

	// 计算双方棋子数
	_numbers[State::BLACK - 1] = _numbers[State::WHITE - 1] = 0;
	for (uint8_t row = 0; row < ROW; ++row)
		for (uint8_t column = 0; column < COLUMN; ++column)
			if (isChess(_index, row, column))
				++_numbers[_states[row][column] - 1];

	// 设置字体颜色
	_renderer.setTextColor(PROMPT, SCREEN);

	// 打印黑子数
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_numbers[State::BLACK - 1]) << std::ends;
	int16_t x = (_left - 8) / 2 + 6, y = _renderer.getRow() / 2;
	_renderer.outText(x, y, _stream.str().c_str());

	// 打印白子数
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_numbers[State::WHITE - 1]) << std::ends;
	int16_t column = _renderer.getColumn();
	x = (column + _right - 8) / 2 + 6;
	_renderer.outText(x, y, _stream.str().c_str());

	// 打印回合数
	_stream.seekp(0, _stream.beg);
	_stream << setw(2) << std::to_string(_round) << std::ends;
	x = (column - 8) / 2 + 6;
	y = (_up - 2) / 2;
	_renderer.outText(x, y, _stream.str().c_str());

	// 打印执子棋手
	_renderer.outText(x, y + 1, _state == BLACK ? "黑" : "白");
}

// 判断能否落子
bool Chessboard::operable(State _state)
{
	for (int16_t row = 0; row < ROW; ++row)
		for (int16_t column = 0; column < COLUMN; ++column)
		{
			// 当前位置有棋子
			if (isChess(_index, row, column))
				continue;

			// 尝试转换
			setChess(row, column, _state);
			bool convertible = convert(row, column, true);
			setChess(row, column, EMPTY);

			// 是否能够转换
			if (convertible)
				return true;
		}
	return false;
}

// 无子可落
void Chessboard::skip() const
{
	int16_t row = _renderer.getRow(), column = _renderer.getColumn();
	_renderer.outText(0, column, _down, row, "无子可落！");
	Sleep(500);
	_renderer.outText(0, column, _down, row, "          ");
}

// 判断棋局结束
bool Chessboard::judge() const noexcept
{
	// 是否一方无子或者棋盘已满
	return _numbers[0] == 0 || _numbers[1] == 0 \
		|| _numbers[0] + _numbers[1] == ROW * COLUMN;
}

// 展示结局
void Chessboard::finish(bool _inoperable) const
{
	// 设置字体颜色
	_renderer.setTextColor(PROMPT, SCREEN);

	// 双方有子且棋盘未满，但双方无子可落
	if (_inoperable)
	{
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "双方无子可落！");
		Sleep(500);
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "              ");
	}

	const char* result;
	if (_numbers[State::BLACK - 1] > _numbers[State::WHITE - 1])
		result = "黑棋胜！";
	else if (_numbers[State::BLACK - 1] < _numbers[State::WHITE - 1])
		result = "白棋胜！";
	else
		result = "平局！";
	_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), result);
	Sleep(500);
}

// 落子操作
bool Chessboard::put(State _state)
{
	// 计算下标
	int16_t row, column;
	getIndex(_x, _y, row, column);

	// 当前位置有棋子
	if (isChess(_index, row, column))
		return false;

	// 设置棋子并尝试转换
	setChess(row, column, _state);
	if (!convert(row, column, true))
	{
		// 转换失败回退操作
		setChess(row, column, EMPTY);
		return false;
	}

	// 转换成功则展示转换过程
	convert(row, column, false);
	return true;
}

// 操作循环
int Chessboard::loop()
{
	using std::setw;

	// 按键操作
	int key;
	do
	{
		wait();
		key = handle();
	} while (key == 0); // 键值为零，说明棋局捕获按键，继续接收按键

	// 中止对弈
	if (key == ESCAPE)
	{
		_renderer.setTextColor(PROMPT, SCREEN);
		_renderer.outText(0, _renderer.getColumn(), _down, _renderer.getRow(), "中止对弈！");
		Sleep(500);
	}
	return key;
}

REVERSI_END
