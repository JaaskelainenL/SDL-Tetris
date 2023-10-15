#include "block.cpp"



// Copies block data to current block
void getBlockData(int (*blockData)[4], int id, int rotation);

void generateBag();

int getWidth(int (*blockData)[4]);

int getOffset(int (*blockData)[4]);

void setBlockColor(SDL_Texture* blockTex, int block_id);

void renderUiBlock(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect rectangle, int(*block)[4], int x, int y, int xOffset, int yOffset);