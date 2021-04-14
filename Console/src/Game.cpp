#include "Game.hpp"
#include "Renderer.hpp"
#include "Menu.hpp"

#include <Windows.h>
#include <conio.h>

REVERSI_BEGIN

// 记录文件名
static const char *IMAGE = "Reversi.dat";

// 加载记录
bool Game::load()
{
	// 关闭文件流
	if (_stream.is_open())
		_stream.close();

	// 以读方式打开文件流
	_stream.open(IMAGE, Stream::in);
	if (!_stream.is_open())
		return false;

	// 读取记录
	_stream >> _chessboard;
	if (!_chessboard)
		return false;

	_stream.read(&_round, sizeof _round);
	_player = (_round & 0x01) == Identity::BLACK ? &_black : &_white;
	return true;
}

// 存储记录
void Game::save()
{
	// 关闭文件流
	if (_stream.is_open())
		_stream.close();

	// 以写方式打开文件流
	_stream.open(IMAGE, Stream::out);
	if (_stream.is_open())
	{
		// 写入记录
		_stream << _chessboard;
		_stream.write(&_round, sizeof _round);
	}
}

// 重新开始
void Game::restart() noexcept
{
	_round = '\1'; // 初始回合数
	_player = &_black; // 初始执子者
	_chessboard.init(); // 初始化棋局
}

// 轮换执子者
void Game::rotate(bool _forward) noexcept
{
	// 前向或者后向切换回合
	_forward ? ++_round : --_round;

	switch (*_player)
	{
	case Identity::BLACK: // 黑子换为白子
		_player = &_white;
		break;

	case Identity::WHITE: // 白子换为黑子
		_player = &_black;
		break;

	default:
		break;
	}
}

// 判断游戏结束
bool Game::judge()
{
	// 有子可落
	if (_chessboard.operable(*_player))
		return false;

	// 前向轮换执子者
	rotate(true);

	// 有子可落
	if (_chessboard.operable(*_player))
	{
		// 后向轮换执子者
		rotate(false);
		return false;
	}

	// 双方无子可落
	return true;
}

// 对弈逻辑
void Game::play()
{
	// 显示棋局
	_chessboard.show();
	_chessboard.update(_round, *_player);

	int key;
	do
	{
		// 棋局操作循环
		key = _chessboard.loop();

		// 中止游戏
		if (key == ESCAPE)
		{
			save(); // 存储记录
			break;
		}
		// 落子操作
		else if (key == SPACE)
		{
			// 是否成功落子
			if (!_chessboard.put(*_player))
				continue;

			// 轮换执子者
			rotate(true);
			// 更新棋局
			_chessboard.update(_round, *_player);

			/*
			 * 游戏结束条件之一：一方无子或者棋盘已满
			 * 游戏结束条件之二：双方无子可落
			 */
			if (bool inoperable = false; _chessboard.judge() || (inoperable = judge()))
			{
				// 展示结局
				_chessboard.finish(inoperable);
				break;
			}

			// 无子可落
			if (!_chessboard.operable(*_player))
			{
				_chessboard.skip();
				rotate(true);
				_chessboard.update(_round, *_player);
			}
		}
	} while (true);

	// 隐藏棋局
	_chessboard.hide();
}

// 游戏循环
void Game::loop()
{
	int choice;
	Menu menu(_renderer);
	do
	{
		// 加载记录，根据有无记录，初始化菜单光标
		menu.resume(load());

		// 选择菜单项
		menu.show();
		choice = menu.loop();
		menu.hide();

		switch (choice)
		{
		case 2: // 继续游戏
			play();
			break;

		case 1: // 重新开始
			restart();
			play();
			break;

		default:
			break;
		}
	} while (choice != 0); // 选择退出游戏即退出循环
}

REVERSI_END
