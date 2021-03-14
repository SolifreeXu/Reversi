#pragma once

#include <cstdint>

class Renderer
{
	void *_handle; // 窗口句柄
	int16_t _row, _column; // 窗口行数和列数

private:
	// 初始化窗口
	void init(const char* _title) const;

	// 隐藏光标
	void hideCursor() const;

	// 显示光标
	void showCursor() const;

	// 隐藏光标
	void moveCursor(int16_t _x, int16_t _y) const;

public:
	Renderer(int16_t _row, int16_t _column, const char *_title = nullptr);

	// 获取窗口行数
	int16_t getRow() const noexcept { return _row; }

	// 获取窗口列数
	int16_t getColumn() const noexcept { return _column; }

	// 清空控制台窗口
	void clearScreen() const;

	// 设置窗口颜色
	void setScreenColor(uint16_t _background, uint16_t _foreground = 0x0F) const;

	// 设置文本颜色
	void setTextColor(uint16_t _foreground, uint16_t _background) const;

	// 输出文本至指定位置
	void outText(int16_t _x, int16_t _y, const char *_text) const;

	// 输出文本至指定范围
	void outText(int16_t _left, int16_t _right, int16_t _up, int16_t _down, const char *_text) const;
};
