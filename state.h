#include "state.cpp"



bool isColliding(int x, int y, int dx, int dy);

bool canRotate(int x, int y);

void resetBlockState();

void switchBlocks();

void clearLine(int deleteY);

void drawScore(SDL_Texture* scoreAmountText, SDL_Renderer* renderer);

void checkForLines();

bool gameTick(SDL_Texture* scoreAmountText, SDL_Renderer* renderer);