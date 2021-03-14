#pragma once

#include <cstdint>
#include <sstream>

#include "Reversi.hpp"

class Renderer;

REVERSI_BEGIN
class Chessboard
{
	struct Index {};
	struct Coordinate {};

public:
	enum State : char { EMPTY, BLACK, WHITE };

private:
	static constexpr uint8_t ROW = 8, COLUMN = 8; // 棋盘每边格子数
	static const Index _index; // 下标说明符
	static const Coordinate _coordinate; // 坐标说明符

	std::stringstream _stream; // 字符流
	Renderer& _renderer; // 渲染器

	int16_t _width, _height; // 棋盘宽度和高度
	int16_t _up, _down, _left, _right; // 棋盘边界

	int16_t _x, _y; // 光标位置

	State _states[ROW][COLUMN]; // 棋子状态
	uint8_t _numbers[2]; // 双方棋子数

private:
	friend std::istream& operator >>(std::istream& _input, Chessboard& _chessboard);
	friend std::ostream& operator <<(std::ostream& _output, const Chessboard& _chessboard);

	// 转换坐标为下标
	void getIndex(int16_t _x, int16_t _y, int16_t& _row, int16_t& _column) const noexcept;

	// 转换下标为坐标
	void getCoord(int16_t _row, int16_t _column, int16_t& _x, int16_t& _y) const noexcept;

	// 判断指定下标是否棋子
	bool isChess(Index _index, int16_t _row, int16_t _column) const noexcept;

	// 判断指定坐标是否棋子
	bool isChess(Coordinate _coordinate, int16_t _x, int16_t _y) const noexcept;

	// 设置棋子状态
	void setChess(int16_t _row, int16_t _column, State _state) noexcept;

	// 设置光标颜色
	void setCursorColor() const;

	// 设置棋子颜色
	void setChessColor(int16_t _x, int16_t _y) const;

	// 绘制棋盘
	void drawChessboard();

	// 绘制所有棋子
	void drawChess() const;

	// 转换单个棋子，展示动态效果
	void convertOne(int16_t _row, int16_t _column, State _state);

	// 转换上方棋子
	bool convertUp(int16_t _row, int16_t _column, bool _judgmental);

	// 转换下方棋子
	bool convertDown(int16_t _row, int16_t _column, bool _judgmental);

	// 转换左方棋子
	bool convertLeft(int16_t _row, int16_t _column, bool _judgmental);

	// 转换右方棋子
	bool convertRight(int16_t _row, int16_t _column, bool _judgmental);

	// 转换左上棋子
	bool convertLeftUp(int16_t _row, int16_t _column, bool _judgmental);

	// 转换右上棋子
	bool convertRightUp(int16_t _row, int16_t _column, bool _judgmental);

	// 转换左下棋子
	bool convertLeftDown(int16_t _row, int16_t _column, bool _judgmental);

	// 转换右下棋子
	bool convertRightDown(int16_t _row, int16_t _column, bool _judgmental);

	// 转换棋子
	bool convert(int16_t _row, int16_t _column, bool _judgmental);

	// 等待操作
	void wait() const;

	// 处理事件
	int handle();

public:
	Chessboard(Renderer& _renderer) noexcept;

	// 检查棋局有效性
	explicit operator bool() const noexcept;

	// 初始棋子
	void init() noexcept;

	// 显示棋局
	void show();

	// 隐藏棋局
	void hide() const;

	// 更新棋局
	void update(uint8_t _round, State _state);

	// 判断能否落子
	bool operable(State _state);

	// 无子可落
	void skip() const;

	// 判断棋局结束
	bool judge() const noexcept;

	// 展示结局
	void finish(bool _inoperable) const;

	// 落子操作
	bool put(State _state);

	// 操作循环
	int loop();
};
REVERSI_END
