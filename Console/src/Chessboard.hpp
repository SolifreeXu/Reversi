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
	static constexpr uint8_t ROW = 8, COLUMN = 8; // ����ÿ�߸�����
	static const Index _index; // �±�˵����
	static const Coordinate _coordinate; // ����˵����

	std::stringstream _stream; // �ַ���
	Renderer& _renderer; // ��Ⱦ��

	int16_t _width, _height; // ���̿�Ⱥ͸߶�
	int16_t _up, _down, _left, _right; // ���̱߽�

	int16_t _x, _y; // ���λ��

	State _states[ROW][COLUMN]; // ����״̬
	uint8_t _numbers[2]; // ˫��������

private:
	friend std::istream& operator >>(std::istream& _input, Chessboard& _chessboard);
	friend std::ostream& operator <<(std::ostream& _output, const Chessboard& _chessboard);

	// ת������Ϊ�±�
	void getIndex(int16_t _x, int16_t _y, int16_t& _row, int16_t& _column) const noexcept;

	// ת���±�Ϊ����
	void getCoord(int16_t _row, int16_t _column, int16_t& _x, int16_t& _y) const noexcept;

	// �ж�ָ���±��Ƿ�����
	bool isChess(Index _index, int16_t _row, int16_t _column) const noexcept;

	// �ж�ָ�������Ƿ�����
	bool isChess(Coordinate _coordinate, int16_t _x, int16_t _y) const noexcept;

	// ��������״̬
	void setChess(int16_t _row, int16_t _column, State _state) noexcept;

	// ���ù����ɫ
	void setCursorColor() const;

	// ����������ɫ
	void setChessColor(int16_t _x, int16_t _y) const;

	// ��������
	void drawChessboard();

	// ������������
	void drawChess() const;

	// ת���������ӣ�չʾ��̬Ч��
	void convertOne(int16_t _row, int16_t _column, State _state);

	// ת���Ϸ�����
	bool convertUp(int16_t _row, int16_t _column, bool _judgmental);

	// ת���·�����
	bool convertDown(int16_t _row, int16_t _column, bool _judgmental);

	// ת��������
	bool convertLeft(int16_t _row, int16_t _column, bool _judgmental);

	// ת���ҷ�����
	bool convertRight(int16_t _row, int16_t _column, bool _judgmental);

	// ת����������
	bool convertLeftUp(int16_t _row, int16_t _column, bool _judgmental);

	// ת����������
	bool convertRightUp(int16_t _row, int16_t _column, bool _judgmental);

	// ת����������
	bool convertLeftDown(int16_t _row, int16_t _column, bool _judgmental);

	// ת����������
	bool convertRightDown(int16_t _row, int16_t _column, bool _judgmental);

	// ת������
	bool convert(int16_t _row, int16_t _column, bool _judgmental);

	// �ȴ�����
	void wait() const;

	// �����¼�
	int handle();

public:
	Chessboard(Renderer& _renderer) noexcept;

	// ��������Ч��
	explicit operator bool() const noexcept;

	// ��ʼ����
	void init() noexcept;

	// ��ʾ���
	void show();

	// �������
	void hide() const;

	// �������
	void update(uint8_t _round, State _state);

	// �ж��ܷ�����
	bool operable(State _state);

	// ���ӿ���
	void skip() const;

	// �ж���ֽ���
	bool judge() const noexcept;

	// չʾ���
	void finish(bool _inoperable) const;

	// ���Ӳ���
	bool put(State _state);

	// ����ѭ��
	int loop();
};
REVERSI_END
