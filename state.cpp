
int gameMatrix[16][10];

unsigned int gameScore = 0;

int curX = 4;
int curY = 0;

int curBlock[4][4];
int curWidth;
int curOffset;
int curRot = 0;

int blockId;
int holdId = -1;


int holdBlock[4][4];
int nextBlock[4][4];

bool holdLock = false;

bool isColliding(int x, int y, int dx=0, int dy=1){


    for(int i = 0; i<4; i++){
        for(int j = 3; j>=0; j--){
            if(curBlock[i][j] != 0) {

                if(gameMatrix[y+i+dy][x+j+dx] != 0){
                    return true;
                }
                
                if(y+i+dy == 16) return true;
            }
        }
    }

    return false;

}


bool canRotate(int x, int y){
    int nextRot[4][4] = {};

    getBlockData(nextRot, blockId, curRot+1);

    int nextWidth = getWidth(nextRot);
    int nextOffset = getOffset(nextRot);

    while(x+nextWidth>9){
        x--;
    }
    while(x+nextOffset <= 0){
        x++;
    }


    for(int i = 0; i<4; i++){
        for(int j = 3; j>=0; j--){
            if(nextRot[i][j] != 0) {

                if(gameMatrix[y+i][x+j] != 0){
                    return false;
                }

                
                if(y+i >= 16) return false;
            }
        }
    }


    return true;
}


void resetBlockState(){
    curY = 0;
    curX = 4;
    curRot = 0;
    getBlockData(curBlock, blockId);
    curOffset = getOffset(curBlock);
    curWidth = getWidth(curBlock);   

    getBlockData(nextBlock,blockBag[0]); 
}




void switchBlocks(){

    if(holdLock)
        return;

    if(holdId == -1)
        holdId = nextPiece();

    int tmp = blockId;
    blockId = holdId;
    holdId = tmp;

    resetBlockState();

    getBlockData(holdBlock,holdId);

    holdLock = true;

}





void clearLine(int deleteY){

    int newMatrix[16][10] = {0};




    for(int y = 15; y>=1; y--){
        
        if(y > deleteY){

            memcpy(newMatrix[y], gameMatrix[y], sizeof(int) * 10);

        } else{

            memcpy(newMatrix[y], gameMatrix[y-1], sizeof(int) * 10);

        }
    }


    memcpy(gameMatrix, newMatrix, sizeof(int) * 16 * 10);
}





void checkForLines(){

    unsigned int linesCleared = 0;

    for(int y = 0; y<16; y++){

        bool fullLine = true;

        for(int x = 0; x<10; x++){
            if(gameMatrix[y][x] == 0)
                fullLine = false;
        }

        if(fullLine){
            clearLine(y);
            linesCleared++;
        }
    }


    if(linesCleared > 0){
        int scores[4] = {800, 1200, 1800, 2000};

        gameScore += scores[linesCleared-1];


    }


}



bool gameTick(){


    if(isColliding(curX, curY)){

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(curBlock[j][i] != 0){
                    
                    // Check if block collides when it's created
                    if(curY == 0 && curX == 4)
                        return false;
                 
                    gameMatrix[curY+j][curX+i] = blockId+1;
                }
            }
        }


        blockId = nextPiece();

        resetBlockState();

        holdLock = false;

        checkForLines();




    } else{
        curY++;
    }


    return true;

}
