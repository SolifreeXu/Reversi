#include "Game.hpp"
#include "Renderer.hpp"
#include "Menu.hpp"

#include <Windows.h>
#include <conio.h>

REVERSI_BEGIN

// ��¼�ļ���
static const char *IMAGE = "Reversi.dat";

// ���ؼ�¼
bool Game::load()
{
	// �ر��ļ���
	if (_stream.is_open())
		_stream.close();

	// �Զ���ʽ���ļ���
	_stream.open(IMAGE, Stream::in);
	if (!_stream.is_open())
		return false;

	// ��ȡ��¼
	_stream >> _chessboard;
	if (!_chessboard)
		return false;

	_stream.read(&_round, sizeof _round);
	_player = (_round & 0x01) == Identity::BLACK ? &_black : &_white;
	return true;
}

// �洢��¼
void Game::save()
{
	// �ر��ļ���
	if (_stream.is_open())
		_stream.close();

	// ��д��ʽ���ļ���
	_stream.open(IMAGE, Stream::out);
	if (_stream.is_open())
	{
		// д���¼
		_stream << _chessboard;
		_stream.write(&_round, sizeof _round);
	}
}

// ���¿�ʼ
void Game::restart() noexcept
{
	_round = '\1'; // ��ʼ�غ���
	_player = &_black; // ��ʼִ����
	_chessboard.init(); // ��ʼ�����
}

// �ֻ�ִ����
void Game::rotate(bool _forward) noexcept
{
	// ǰ����ߺ����л��غ�
	_forward ? ++_round : --_round;

	switch (*_player)
	{
	case Identity::BLACK: // ���ӻ�Ϊ����
		_player = &_white;
		break;

	case Identity::WHITE: // ���ӻ�Ϊ����
		_player = &_black;
		break;

	default:
		break;
	}
}

// �ж���Ϸ����
bool Game::judge()
{
	// ���ӿ���
	if (_chessboard.operable(*_player))
		return false;

	// ǰ���ֻ�ִ����
	rotate(true);

	// ���ӿ���
	if (_chessboard.operable(*_player))
	{
		// �����ֻ�ִ����
		rotate(false);
		return false;
	}

	// ˫�����ӿ���
	return true;
}

// �����߼�
void Game::play()
{
	// ��ʾ���
	_chessboard.show();
	_chessboard.update(_round, *_player);

	int key;
	do
	{
		// ��ֲ���ѭ��
		key = _chessboard.loop();

		// ��ֹ��Ϸ
		if (key == ESCAPE)
		{
			save(); // �洢��¼
			break;
		}
		// ���Ӳ���
		else if (key == SPACE)
		{
			// �Ƿ�ɹ�����
			if (!_chessboard.put(*_player))
				continue;

			// �ֻ�ִ����
			rotate(true);
			// �������
			_chessboard.update(_round, *_player);

			/*
			 * ��Ϸ��������֮һ��һ�����ӻ�����������
			 * ��Ϸ��������֮����˫�����ӿ���
			 */
			if (bool inoperable = false; _chessboard.judge() || (inoperable = judge()))
			{
				// չʾ���
				_chessboard.finish(inoperable);
				break;
			}

			// ���ӿ���
			if (!_chessboard.operable(*_player))
			{
				_chessboard.skip();
				rotate(true);
				_chessboard.update(_round, *_player);
			}
		}
	} while (true);

	// �������
	_chessboard.hide();
}

// ��Ϸѭ��
void Game::loop()
{
	int choice;
	Menu menu(_renderer);
	do
	{
		// ���ؼ�¼���������޼�¼����ʼ���˵����
		menu.resume(load());

		// ѡ��˵���
		menu.show();
		choice = menu.loop();
		menu.hide();

		switch (choice)
		{
		case 2: // ������Ϸ
			play();
			break;

		case 1: // ���¿�ʼ
			restart();
			play();
			break;

		default:
			break;
		}
	} while (choice != 0); // ѡ���˳���Ϸ���˳�ѭ��
}

REVERSI_END
