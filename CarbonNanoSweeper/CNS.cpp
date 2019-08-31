#include "CNS.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderData.h"
#include "Camera.h"

CNS * CNS::Game = nullptr;

CNS::CNS()
{
	initGame();
	setTileUBO();
	tileSet = 0;
}

CNS::~CNS()
{
}

void CNS::dig(int i)
{
	if (Field[i].open) return;
	if (Field[i].mine != 0) {
		gameState = GAMESTATE_LOST;
		return;
	}
	Field[i].open = true;
	if (Field[i].neighbours == 0) {
		Neighbourhood N(i);
		for (int j = 0; j < 6; j++) {
			if (!Field[N.ids[j]].open) dig(N.ids[j]);
		}
	}
}

void CNS::initGame()
{
	chordId = -1;
	chordTime = -1;

	for (int i = 0; i < 16 * ROW_NUM; i++) {
		Field[i].mine = 0;
		Field[i].neighbours = 0;
		Field[i].open = false;
		Field[i].flag = 0;
	}

	/*for (int i = 0; i < 8 * ROW_NUM; i++) {
		Neighbourhood N(i);
		for (int j = 0; j < 6; j++) {
			Field[i].neighbours += Field[N.ids[j]].mine;
		}
	}*/

	gameState = GAMESTATE_CLOSED;

	setTileUBO();
}

void CNS::generateGame(int safe)
{
	int generated = 0;
	srand(time(NULL));
	Neighbourhood N(safe);
	while (generated < NUM_MINES) {
		int rnd = rand() % (16 * ROW_NUM);
		if((safe != rnd) && (Field[rnd].mine == 0)){
			bool b = true;
			for (int i = 0; i < 6; i++) {
				if (N.ids[i] == rnd) {
					b = false;
					break;
				}
			}
			if (b) {
				Field[rnd].mine = 1;
				generated++;
			}
		}
	}
	for (int i = 0; i < 16 * ROW_NUM; i++) {
		Neighbourhood N(i);
		for (int j = 0; j < 6; j++) {
			Field[i].neighbours += Field[N.ids[j]].mine;
		}
	}
}

void CNS::setTileUBO()
{
	int open = 0;
	int flags = 0;
	for (int i = 0; i < 16 * ROW_NUM; i++) {
		int tileId;
		if (Field[i].open) {
			open++;
			tileId = 4 + Field[i].neighbours;
		}
		else {
			switch (gameState) {
			case GAMESTATE_LOST:
				if (Field[i].mine == 0) {
					if (Field[i].flag != 0) {
						tileId = 2;
						flags++;
					}
					else {
						tileId = 0;
					}
				}
				else {
					tileId = 1;
				}
				break;
			case GAMESTATE_COMPLETE:
				flags++;
				tileId = 3;
				break;
			default:
				if (Field[i].flag != 0) {
					flags++;
					tileId = 2;
				}
				else {
					tileId = 0;
				}
				break;
			}
		}
		RenderData::UBO.Tiles.ids[i] = tileId;
	}
	RenderData::updateUBO(UBOB_TILES);

	title = "Open: " + std::to_string(open) + " / " + std::to_string(16 * ROW_NUM - NUM_MINES)
		+ "   Flags: " + std::to_string(flags) + " / " + std::to_string(NUM_MINES);
	newName = true;

	if ((open == (16 * ROW_NUM - NUM_MINES)) && (gameState == GAMESTATE_PLAYING)) {
		gameState = GAMESTATE_COMPLETE;
		setTileUBO();
	}
}

void CNS::render()
{
	RenderData::UBO.Highlight.HighlightHex_E_E_E.x = Camera::C->findCentralHexagon();
	RenderData::UBO.Highlight.HighlightHex_E_E_E.y = (int)(1000 * glfwGetTime());
	RenderData::updateUBO(UBOB_HIGHLIGHT);

	glUseProgram(RenderData::POSTPROCESSING.RayMarchShaders.atoms);
	RenderData::POSTPROCESSING.Temp[0].setAsTarget();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(RenderData::POSTPROCESSING.RayMarchShaders.bonds);
	RenderData::POSTPROCESSING.Temp[1].setAsTarget();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(RenderData::POSTPROCESSING.RayMarchShaders.cylinder);
	RenderData::TILES[tileSet]->setAs(GL_TEXTURE0);
	RenderData::POSTPROCESSING.Temp2.setAsTarget();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Copy to screen
	glUseProgram(RenderData::POSTPROCESSING.Shaders.combine);
	RenderData::POSTPROCESSING.Temp[0].setAsSource(GL_TEXTURE0, GL_TEXTURE1);
	RenderData::POSTPROCESSING.Temp[1].setAsSource(GL_TEXTURE2, GL_TEXTURE3);
	RenderData::POSTPROCESSING.Temp2.setAsSource(GL_TEXTURE4, GL_TEXTURE5, GL_TEXTURE6);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glViewport(0, 0, RenderData::POSTPROCESSING.width, RenderData::POSTPROCESSING.height);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CNS::leftClick(int i)
{
	//std::cout << "Leftclick " << i << "\n";
	if (Field[i].flag > 0) return;
	if (Field[i].open) {
		float time = glfwGetTime();
		if (((time - chordTime) < 0.4) && (chordId == i)) {
			Neighbourhood N(i);
			int flags = 0;
			for (int j = 0; j < 6; j++) {
				flags += Field[N.ids[j]].flag;
			}
			if (flags == Field[i].neighbours) {
				// Do chording
				for (int j = 0; j < 6; j++) {
					if (Field[N.ids[j]].flag == 0) dig(N.ids[j]);
				}
				setTileUBO();
			}
		}
		chordId = i;
		chordTime = time;
	}
	switch (gameState) {
	case GAMESTATE_CLOSED:
		generateGame(i);
		gameState = GAMESTATE_PLAYING;
	case GAMESTATE_PLAYING:
		dig(i);
		setTileUBO();
		break;
	}
}

void CNS::rightClick(int i)
{
	if (Field[i].open) return;
	if (gameState == GAMESTATE_PLAYING) {
		Field[i].flag = 1 - Field[i].flag;
		//std::cout << Field[i].flag << "\n";
		setTileUBO();
	}
}

void CNS::setTileSet(int i)
{
	if ((i >= 0) && (i < 4)) tileSet = i;
}

Neighbourhood::Neighbourhood(int i)
{
	int n = i & 0xf;

	ids[0] = i - n + ((n - 1) & 0xf);
	ids[1] = i - n + ((n + 1) & 0xf);
	ids[2] = i - 16;
	ids[3] = i + 16;
	ids[4] = i - 16 - n;
	ids[5] = i + 16 - n;

	n += (((i & 0x10) == 0) ? 1 : -1);
	n &= 0xf;

	ids[4] += n;
	ids[5] += n;

	for (int i = 0; i < 6; i++) {
		if (ids[i] < 0) ids[i] += 16 * ROW_NUM;
		if (ids[i] >= (16 * ROW_NUM)) ids[i] -= 16 * ROW_NUM;
	}
}
