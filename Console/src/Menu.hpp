#pragma once

#include <cstdint>

#include "Reversi.hpp"

class Renderer;

REVERSI_BEGIN
class Menu
{
	Renderer& _renderer; // 渲染器
	bool _stored; // 存储标记

	int16_t _width, _height; // 菜单边框宽度和高度
	int16_t _up, _down, _left, _right; // 菜单边界
	int16_t _size, _number; // 字宽度与句数量

	int16_t _x, _y; // 光标位置
	int _choice; // 选择编号

private:
	// 获取句列间距
	int16_t getSpace(int16_t _width) const noexcept;

	// 刷新光标
	void refreshCursor(const char* _cursor);

	// 更新选择
	void modifyChoice(int _delta) noexcept;

	// 更新菜单
	int update();

public:
	Menu(Renderer& _renderer) noexcept;

	// 继续游戏
	void resume(bool _stored) noexcept
	{
		this->_stored = _stored;
		_choice = _stored ? 2 : 1;
	}

	// 显示菜单
	void show();

	// 隐藏菜单
	void hide() const;

	// 菜单循环
	int loop();
};
REVERSI_END
