#pragma once

#include <cstdint>

#include "Reversi.hpp"

class Renderer;

REVERSI_BEGIN
class Menu
{
	Renderer& _renderer; // ��Ⱦ��
	bool _stored; // �洢���

	int16_t _width, _height; // �˵��߿��Ⱥ͸߶�
	int16_t _up, _down, _left, _right; // �˵��߽�
	int16_t _size, _number; // �ֿ���������

	int16_t _x, _y; // ���λ��
	int _choice; // ѡ����

private:
	// ��ȡ���м��
	int16_t getSpace(int16_t _width) const noexcept;

	// ˢ�¹��
	void refreshCursor(const char* _cursor);

	// ����ѡ��
	void modifyChoice(int _delta) noexcept;

	// ���²˵�
	int update();

public:
	Menu(Renderer& _renderer) noexcept;

	// ������Ϸ
	void resume(bool _stored) noexcept
	{
		this->_stored = _stored;
		_choice = _stored ? 2 : 1;
	}

	// ��ʾ�˵�
	void show();

	// ���ز˵�
	void hide() const;

	// �˵�ѭ��
	int loop();
};
REVERSI_END
