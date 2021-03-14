#pragma once

#include <cstdint>

class Renderer
{
	void *_handle; // ���ھ��
	int16_t _row, _column; // ��������������

private:
	// ��ʼ������
	void init(const char* _title) const;

	// ���ع��
	void hideCursor() const;

	// ��ʾ���
	void showCursor() const;

	// ���ع��
	void moveCursor(int16_t _x, int16_t _y) const;

public:
	Renderer(int16_t _row, int16_t _column, const char *_title = nullptr);

	// ��ȡ��������
	int16_t getRow() const noexcept { return _row; }

	// ��ȡ��������
	int16_t getColumn() const noexcept { return _column; }

	// ��տ���̨����
	void clearScreen() const;

	// ���ô�����ɫ
	void setScreenColor(uint16_t _background, uint16_t _foreground = 0x0F) const;

	// �����ı���ɫ
	void setTextColor(uint16_t _foreground, uint16_t _background) const;

	// ����ı���ָ��λ��
	void outText(int16_t _x, int16_t _y, const char *_text) const;

	// ����ı���ָ����Χ
	void outText(int16_t _left, int16_t _right, int16_t _up, int16_t _down, const char *_text) const;
};
