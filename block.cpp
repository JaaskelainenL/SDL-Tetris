#include <algorithm>
#include <random>


// Block color data
// Colors based on standard tetrino colors
const int block_colors[7][3] = {
    {0,173,238},
    {27,116,187},
    {246,146,30},
    {255,241,0},
    {139,197,63},
    {101,45,144},
    {236,27,36}
};


// Blocks and their rotations
const int block_i[4][4][4] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
};

const int block_j[4][4][4] = {
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{0,0,1,0},{0,1,1,0},{0,0,0,0}}

};

const int block_l[4][4][4] = {
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,0,0,0},{1,1,0,0},{0,0,0,0}},
    {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,1,0},{0,0,1,0},{0,0,1,0},{0,0,0,0}}    
};

const int block_o[4][4][4] = {
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}
};

const int block_s[4][4][4] = {
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}    
};

const int block_t[4][4][4] = {
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{0,0,1,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}    
};

const int block_z[4][4][4] = {
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}    
};


// Block queue
int blockBag[7];




// Copy block data to current block
void getBlockData(int (*blockData)[4], int id, int rotation=0){
    switch(id){
        case 0:
            memcpy(blockData, block_i[rotation], sizeof(int) * 4 * 4);
            break;
        case 1: 
            memcpy(blockData, block_j[rotation], sizeof(int) * 4 * 4);
            break;
        case 2: 
            memcpy(blockData, block_l[rotation], sizeof(int) * 4 * 4);
            break;
        case 3: 
            memcpy(blockData, block_o[rotation], sizeof(int) * 4 * 4);
            break;
        case 4: 
            memcpy(blockData, block_s[rotation], sizeof(int) * 4 * 4);
            break;
        case 5: 
            memcpy(blockData, block_t[rotation], sizeof(int) * 4 * 4);
            break;
        case 6: 
            memcpy(blockData, block_z[rotation], sizeof(int) * 4 * 4);
            break;                                    
        
        default:
            break;
    }
}



std::random_device rd;
std::mt19937 g(rd());

void generateBag(){

    int newBag[7] = {0,1,2,3,4,5,6};

    std::shuffle(newBag, newBag + 7, g);

    memcpy(blockBag, newBag, sizeof(int) * 7);
}


int nextPiece(){

    int piece = blockBag[0];

    int newBag[7];

    // Remove first piece from bag
    for(int i = 1; i < 7; i++){
        newBag[i-1] = blockBag[i];
    }
    newBag[6] = -1;


    memcpy(blockBag, newBag, sizeof(int) * 7);

    if(blockBag[0] == -1){
        generateBag();
    }

    return piece;
}



int getWidth(int (*blockData)[4]){
    int width = 0;

    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            if(blockData[i][j] != 0 && j > width)
                width = j;
        }
    }
    return width;

}

int getOffset(int (*blockData)[4]){
    int offset = 2;

    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            if(blockData[i][j] != 0 && j < offset)
                offset = j;
        }        
    }

    return offset;

}



void setBlockColor(SDL_Texture* blockTex, int block_id){
    SDL_SetTextureColorMod(blockTex, block_colors[block_id][0], block_colors[block_id][1], block_colors[block_id][2]);
}



void renderUiBlock(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect rectangle, int(*block)[4], int x, int y, int xOffset=0, int yOffset=0){

    for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                if(block[i][j] != 0){
                    rectangle.x = x + (xOffset + j)*25;
                    rectangle.y = y + (yOffset + i)*25;
                    SDL_RenderCopy(renderer, tex, NULL, &rectangle);
                }
            }
    }        
}