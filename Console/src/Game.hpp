#pragma once

#include <fstream>

#include "Reversi.hpp"
#include "Chessboard.hpp"

class Renderer;

REVERSI_BEGIN
class Game
{
	using Stream = std::fstream;
	using Identity = Chessboard::State;

private:
	Stream _stream; // 组合文件流
	Renderer& _renderer; // 聚合渲染器

	Chessboard _chessboard; // 棋局
	Identity _black, _white, *_player; // 黑白棋手
	char _round; // 回合数

private:
	// 加载记录
	bool load();

	// 存储记录
	void save();

	// 重新开始
	void restart() noexcept;

	// 轮换执子者
	void rotate(bool _forward) noexcept;

	// 判定游戏结束
	bool judge();

	// 对弈逻辑
	void play();

public:
	Game(Renderer& _renderer) noexcept : \
		_renderer(_renderer), \
		_chessboard(_renderer), \
		_black(Identity::BLACK), _white(Identity::WHITE), \
		_round ('\1') {}

	~Game()
	{
		if (_stream.is_open())
			_stream.close();
	}

	// 游戏循环
	void loop();
};
REVERSI_END
