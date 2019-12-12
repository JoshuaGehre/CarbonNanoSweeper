#ifndef JG_CNS_H
#define JG_CNS_H

#include <string>

#include "DTGlobal.h"

#define GAMESTATE_CLOSED 0
#define GAMESTATE_PLAYING 1
#define GAMESTATE_COMPLETE 2
#define GAMESTATE_LOST 3

struct Tile {
	bool open;
	int mine;
	int flag;
	int neighbours;
};

struct Neighbourhood {
	int ids[6];
	Neighbourhood(int i);
};

class CNS {
private:
	Tile Field[16 * ROW_NUM];

	int gameState;

	int tileSet;

	int chordId;
	float chordTime;
public:
	CNS();
	~CNS();

	void dig(int i);

	void initGame();

	void generateGame(int safe);

	void setTileUBO();

	void render();

	void leftClick(int i);

	void rightClick(int i);

	void setTileSet(int i);

	std::string title;
	bool newName;

	static CNS * Game;
};

#endif
