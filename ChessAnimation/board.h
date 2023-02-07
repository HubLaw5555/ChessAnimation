#ifndef BOARD_H
#define BOARD_H

#include "shader.h"
#include <list>



const double shiftUnit = 0.209243393;
const float moveHeightRatio = 0.15;

enum PieceType {
	None,
	Pawn,
	Queen,
	King,
	Rook,
	Horse,
	Bishop
};

enum PieceColor {
	NoneC,
	White,
	Black
};

class piece {
public:
	PieceType type;
	PieceColor color;
	glm::vec3 totalShift = glm::vec3(0);
	bool isVisible = true;

	void move(int fieldsTop, int fieldsRight)
	{
		totalShift += glm::vec3(fieldsTop, 0, fieldsRight) * (float)shiftUnit;
	}

	piece(PieceType t = None, PieceColor c = NoneC)
	{
		type = t;
		color = c;
	}
};


std::list<std::string> moves{ "A2A3","A7A6", "B2B3", "B7B6", "C2C3", "C7C6",
"D2D3", "D7D6", "E2E3", "E7E6", "F2F3", "F7F6", "G2G3", "G7G6", "H2H3", "H7H6"};

const int movesCount = moves.size();
const int FramesPerMove = 800;

std::string pop_move()
{
	auto m = moves.front();
	moves.pop_front();
	return m;
}


class Board {
	piece*** board;

	int movesDone = 0;
	int frame = 0;

	struct move {
		std::string from, to;
		glm::vec3 direction;
		piece* movedPiece;
		float distance;
		
		move(move const&) = default;
		move() = default;

		move& operator = (std::string str)
		{
			from = str.substr(0, 2);
			to = str.substr(2, 2);
			auto f = CoordToPosition(from);
			auto t = CoordToPosition(to);
			distance = sqrt((f.first - t.first) * (f.first - t.first) + (f.second - t.second) * (f.second - t.second));
			direction = dirVector(f,t);
			movedPiece = nullptr;
			return *this;
		}

		float calcParabola(float x)
		{
			float h = moveHeightRatio * distance;
			return - h*(x * x - distance * x);
		}

	} move;

protected:

	Board()
	{
		board = new piece * *[8];
		for (int i = 0; i < 8; ++i)
		{
			board[i] = new piece * [8];
			for (int j = 0; j < 8; ++j)
				board[i][j] = nullptr;
		}
	}

	static Board* instance;

public:

	static Board* GetInstance() {
		if (instance == nullptr)
			instance = new Board();
		return instance;
	}

	void execute_moves()
	{
		//while (frame < 5000) frame++;

		if (movesDone <= movesCount)
		{
			if (frame++ % FramesPerMove == 0)
			{
				if (movesDone != 0)
				{
					piece* toPiece = GetPiece(move.to);
					if (toPiece != nullptr)
					{
						toPiece->isVisible = false;
					}
					auto from = CoordToPosition(move.to);
					auto to = CoordToPosition(move.to);
					board[from.first][from.second] = nullptr;
					board[to.first][to.second] = move.movedPiece;

				}
				if (movesDone < movesCount)
				{
					move = pop_move();
					move.movedPiece = GetPiece(move.from);
				}
				movesDone++;
			}
			else
			{
				move.movedPiece->totalShift += move.direction / float(FramesPerMove);
				move.movedPiece->totalShift.y = move.calcParabola(float(((frame - 1) % FramesPerMove)*move.distance / float(FramesPerMove)));
			}
		}
	}

	static glm::vec3 dirVector(std::pair<int, int> from, std::pair<int, int> to)
	{
		return glm::vec3(to.second - from.second, 0, from.first - to.first) * float(shiftUnit);
	}

	piece* SetInitialPiece(std::string coord)
	{
		piece* p = GetInitialPieceCoord(coord);
		std::pair<int, int> pos = CoordToPosition(coord);
		board[pos.first][pos.second] = p;
		return p;
	}

	piece* GetPiece(std::string coord)
	{
		auto pos = CoordToPosition(coord);
		return board[pos.first][pos.second];
	}

	static std::pair<int, int> CoordToPosition(std::string coord)
	{
		char letter = coord[0];
		char number = coord[1];

		return std::make_pair(number - 49, 7 - (letter - 65));
	}

	static piece* GetInitialPieceCoord(std::string pos)
	{
		char letter = pos[0];
		char number = pos[1] - 48;

		PieceColor color = number > 2 ? Black : White;
		if (number == 2 || number == 7)
		{
			return new piece(Pawn, color);
		}

		PieceType type = None;
		switch (letter)
		{
		case 'A':
		case 'H':
			type = Rook; break;
		case 'B':
		case 'G':
			type = Horse; break;
		case 'C':
		case 'F':
			type = Bishop; break;
		case 'D':
			type = Queen; break;
		case 'E':
			type = King; break;
		default: break;
		}
		return new piece(type, color);
	}
};

// kolejnosc tu sie liczy, musi byc jak w pliku
std::vector<std::string> piecesFromFile
{ "A7", "A8", "B8", "C8", "D8",
  "E8", "H8", "G8", "F8", "B7",
  "C7", "D7", "E7", "F7", "G7",
  "H7", "A2", "A1", "B1", "C1",
  "D1", "E1", "H1", "G1", "F1",
  "B2", "C2", "D2", "E2", "F2",
  "G2", "H2" };

Board* Board::instance = nullptr;



//table_Cube.001					avgX: 0, avgY : 0.101587
//white_pawn_1_Plane.002			avgX : 0.739002, avgY : 0.273696	[A7]	Black Pawn
//white_castle_left_Plane.003		avgX : 0.740013, avgY : 0.264772	[A8]	Black Rook
//white_horse_left_Plane.004		avgX : 0.519547, avgY : 0.282155	[B8]	Black Horse
//white_elephant_left_Plane.005		avgX : 0.315856, avgY : 0.296658	[C8]	Black Bishop
//white_vizier_Plane.006			avgX : 0.104146, avgY : 0.334549	[D8]	Black Queen
//white_shah_Plane.007				avgX : -0.109311, avgY : 0.326997	[E8]	Black King
//white_castle_right_Plane.008		avgX : -0.74453, avgY : 0.264772	[H8]	Black Rook
//white_horse_right_Plane.009		avgX : -0.530685, avgY : 0.282155	[G8]	Black Horse
//white_elephant_right_Plane.010	avgX : -0.313153, avgY : 0.296658	[F8]	Black Bishop
//white_pawn_2_Plane.011			avgX : 0.530083, avgY : 0.273696	[B7]	Black Pawn
//white_pawn_3_Plane.012			avgX : 0.318913, avgY : 0.273696	[C7]	Black Pawn
//white_pawn_4_Plane.013			avgX : 0.109992, avgY : 0.273696	[D7]	Black Pawn
//white_pawn_5_Plane.014			avgX : -0.0989303, avgY : 0.273696	[E7]	Black Pawn
//white_pawn_6_Plane.015			avgX : -0.316247, avgY : 0.273696	[F7]	Black Pawn
//white_pawn_7_Plane.016			avgX : -0.529453, avgY : 0.273696	[G7]	Black Pawn
//white_pawn_8_Plane.017			avgX : -0.74656, avgY : 0.273696	[H7]	Black Pawn
//black_pawn_1_Plane.018			avgX : 0.738996, avgY : 0.273696	[A2]	White Pawn
//black_castle_right_Plane.019		avgX : 0.740008, avgY : 0.264773	[A1]	White Rook
//black_horse_right_Plane.020		avgX : 0.519563, avgY : 0.282155	[B1]	White Horse
//black_elephant_right_Plane.021	avgX : 0.315843, avgY : 0.296657	[C1]	White Bishop
//black_vizier_Plane.022			avgX : 0.10416, avgY : 0.334549		[D1]	White Queen
//black_shah_Plane.023				avgX : -0.10932, avgY : 0.326997	[E1]	White King
//black_castle_left_Plane.024		avgX : -0.733302, avgY : 0.264773	[H1]	White Rook
//black_horse_left_Plane.025		avgX : -0.528785, avgY : 0.282155	[G1]	White Horse
//black_elephant_left_Plane.026		avgX : -0.318782, avgY : 0.296657	[F1]	White Bishop
//black_pawn_2_Plane.027			avgX : 0.52371, avgY : 0.273696		[B2]	White Pawn
//black_pawn_3_Plane.028			avgX : 0.314039, avgY : 0.273696	[C2]	White Pawn
//black_pawn_4_Plane.029			avgX : 0.104368, avgY : 0.273696	[D2]	White Pawn
//black_pawn_5_Plane.030			avgX : -0.105302, avgY : 0.273696	[E2]	White Pawn
//black_pawn_6_Plane.031			avgX : -0.316845, avgY : 0.273696	[F2]	White Pawn
//black_pawn_7_Plane.032			avgX : -0.519028, avgY : 0.273696	[G2]	White Pawn
//black_pawn_8_Plane.033			avgX : -0.738059, avgY : 0.273696	[H2]	White Pawn

#endif // !BOARD_H
