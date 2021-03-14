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
	Stream _stream; // ����ļ���
	Renderer& _renderer; // �ۺ���Ⱦ��

	Chessboard _chessboard; // ���
	Identity _black, _white, *_player; // �ڰ�����
	char _round; // �غ���

private:
	// ���ؼ�¼
	bool load();

	// �洢��¼
	void save();

	// ���¿�ʼ
	void restart() noexcept;

	// �ֻ�ִ����
	void rotate(bool _forward) noexcept;

	// �ж���Ϸ����
	bool judge();

	// �����߼�
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

	// ��Ϸѭ��
	void loop();
};
REVERSI_END
