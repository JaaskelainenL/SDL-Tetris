#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "block.h"
#include "state.h"




int curSpeed = 30;

unsigned int lastRenderedScore = 0;

const int GAME_WINDOW_HEIGHT = 640;
const int GAME_WINDOW_WIDTH  = 480;



SDL_Texture* loadTexture(SDL_Renderer* renderer, char* location){

    SDL_Surface* surface = SDL_LoadBMP(location);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return tex;
}

SDL_Texture* textToTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text){

    SDL_Surface* surface = TTF_RenderText_Solid(font, text,{255,255,255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
}




int main(int argc, char* argv[]){

    SDL_Window* window=nullptr;

    // Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    }

    // Init fontsystem
    if(TTF_Init() == -1){
        printf("SDL_ttf couldn't be initialized");
        return 1;
    }


    // Create window
    window = SDL_CreateWindow("C++ SDL Tetris",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
            GAME_WINDOW_HEIGHT,
            GAME_WINDOW_WIDTH,
            SDL_WINDOW_SHOWN);



    // Init renderer
    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // Load grid texture to gridTex
    SDL_Texture* gridTex = loadTexture(renderer,"./assets/grid.bmp");

    // Load block texture to blockTex
    SDL_Texture* blockTex = loadTexture(renderer,"./assets/block.bmp");

    // UI font
    TTF_Font* font = TTF_OpenFont("./assets/OpenSans.ttf",32);

    // Textures for UI Text
    SDL_Texture* nextText = textToTexture(renderer,font,"Next");
    SDL_Texture* holdText = textToTexture(renderer,font,"Hold");
    SDL_Texture* scoreText = textToTexture(renderer,font,"Score");
    SDL_Texture* scoreAmountText = textToTexture(renderer,font,"0");


    // Grid init
    SDL_Rect grid;
    grid.x = 0;
    grid.y = 0;
    grid.w = 640;
    grid.h = 480;

    // Rectangle init
    SDL_Rect rectangle;
    rectangle.w = 25;
    rectangle.h = 25;

    // Ui text holder init
    SDL_Rect uiText;
    uiText.w = 64;
    uiText.h = 32;

    SDL_Rect uiTextScore;
    uiTextScore.w = 16;
    uiTextScore.h = 32;
    uiTextScore.x = 430;
    uiTextScore.y = 332;



    srand(time(NULL));

    generateBag();

    blockId = nextPiece();
    resetBlockState();

    unsigned short framecounter = 0;
	bool gameIsRunning = true;

	//Program loop
	while(gameIsRunning){

        int t0 = SDL_GetTicks();

        framecounter++;

        // Input
		SDL_Event event;
		// Start event loop
		while(SDL_PollEvent(&event)){

            // Allow application to quit
			if(event.type == SDL_QUIT)
				gameIsRunning = false;


            // Read input from keyboard
            const Uint8 *state = SDL_GetKeyboardState(NULL);


            if(state[SDL_SCANCODE_RIGHT] && curX+curWidth<9 && !isColliding(curX,curY,1,0)){
                curX++;
            }
            else if(state[SDL_SCANCODE_LEFT] && curX+curOffset > 0 && !isColliding(curX,curY,-1,0)){
                curX--;
            }

            if(state[SDL_SCANCODE_UP] && canRotate(curX,curY)){

                if(++curRot > 3) curRot = 0;

                getBlockData(curBlock, blockId, curRot);

                curOffset = getOffset(curBlock);
                curWidth = getWidth(curBlock);

                while(curX+curWidth>9){
                    curX--;
                }
                while(curX+curOffset <= 0){
                    curX++;
                }

            }

            if(state[SDL_SCANCODE_DOWN]){
                curSpeed = 2;
            } else{
                curSpeed = 30;
            }


            if(state[SDL_SCANCODE_LSHIFT]){
                switchBlocks();
            }


		}






        // Game tick, where game logic is done
        if(framecounter % curSpeed == 0){
            gameIsRunning = gameTick(); // Returns if next tick can happen I.E if game isn't over

        }






        //
        // Rendering
        //


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // BG Color
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Set color

        // Ui text
        uiText.x = 40;
        uiText.y = 30;
        SDL_RenderCopy(renderer, holdText, NULL, &uiText);
        uiText.x = 440;
        SDL_RenderCopy(renderer, nextText, NULL, &uiText);

        uiText.y = 300;
        SDL_RenderCopy(renderer, scoreText, NULL, &uiText);

        // Game score, texture needs to be redone everytime score updates
        if(gameScore > lastRenderedScore){

            std::string gameScoreText = "" + std::to_string(gameScore);
            scoreAmountText = textToTexture(renderer,font,std::to_string(gameScore).c_str());

            if(gameScore > 100)
                uiTextScore.w = 64;


            lastRenderedScore = gameScore;
        }
            SDL_RenderCopy(renderer, scoreAmountText, NULL, &uiTextScore);



        // Draw grid
        SDL_RenderCopy(renderer, gridTex, NULL, &grid);


        // Draw hold block
        setBlockColor(blockTex, holdId);
        renderUiBlock(renderer, blockTex, rectangle, holdBlock, 30, 70);

        // Draw next block
        setBlockColor(blockTex, blockBag[0]);
        renderUiBlock(renderer, blockTex, rectangle, nextBlock, 440, 70);


        // Draw current block
        setBlockColor(blockTex, blockId);
        renderUiBlock(renderer, blockTex, rectangle, curBlock, 150, 30, curX, curY);



        // Draw everything in matrix
        for(int mx = 0; mx < 10; mx++){
            for(int my = 0; my<16; my++){
                if(gameMatrix[my][mx] != 0){

                    rectangle.x = 150 + mx*25;
                    rectangle.y = 30 + 25*my;

                    setBlockColor(blockTex, gameMatrix[my][mx]-1);
                    SDL_RenderCopy(renderer, blockTex, NULL, &rectangle);
                }
            }
        }


        SDL_RenderPresent(renderer);






        // Delta time, cap at 60fps
        int dt = SDL_GetTicks()-t0;
        if(dt < 16)
            SDL_Delay(16 - dt);

	}

    SDL_DestroyTexture(blockTex);

    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Final score: %i\n",gameScore);

    return 0;
}