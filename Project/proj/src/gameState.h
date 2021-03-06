#pragma once

#include "bitmap.h"

typedef struct {
	int done;
	int backgroundX;
	int velocity;
	int laneY[3];
	int lives;
	int points;
	int scoreWidth;
	int gameover;
	int gotRtc;

	unsigned long* day;
	unsigned long* month;
	unsigned long* year;
	unsigned long* hour;
	unsigned long* minute;
	unsigned long* seconds;
	Bitmap* gameoverbmp;

	Bitmap* background;
	Bitmap* riscas;
	Bitmap* car;
	Bitmap* rock;
	Bitmap* rock2;
	Bitmap* cone;
	Bitmap* cone2;
	Bitmap* heart;
	Bitmap* coin;
	Bitmap* hole;
	Bitmap* hole2;
	Bitmap* score;

	Bitmap* zero;
	Bitmap* um;
	Bitmap* dois;
	Bitmap* tres;
	Bitmap* quatro;
	Bitmap* cinco;
	Bitmap* seis;
	Bitmap* sete;
	Bitmap* oito;
	Bitmap* nove;
}GameState;

/**
 * @brief Initializes course(race) state
 * @return The struct with the current course state
 */
GameState* newGameState();

/**
 * @brief Updates game state regarding the user's decisions to move the car
 * @param state The struct with the current course state
 * @param scancode Code generated by pressing a key
 */
void updateGameState(GameState* state, unsigned long scancode);

/**
 * @brief Draws all bitmaps required for the course state
 * @param state The struct with the current course state
 */
void drawGameState(GameState* state);

/**
 * @brief Deletes all bitmaps used for the game and frees the space allocated for the course state
 * @param state The struct with the current course state
 */
void deleteGameState(GameState* state);
