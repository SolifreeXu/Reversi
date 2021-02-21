#pragma once

#include <cstdint>

class Renderer
{
	void *handle;
	short row, column;
public:
	Renderer(int16_t row, int16_t column, const char *title = nullptr);
	short getRow() { return row; }
	short getColumn() { return column; }
	void clearScreen();
	void setScreenColor(uint16_t background);
	void setTextColor(uint16_t foreground, uint16_t background);
	void outText(int16_t x, int16_t y, const char *text);
	void outTextRange(int16_t left, int16_t right, int16_t up, int16_t down, const char *text);
private:
	void init(int16_t row, int16_t column, const char *title);
	void hideCursor();
	void showCursor();
	void moveCursor(int16_t x, int16_t y);
};
