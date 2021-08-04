#include <SDL2/SDL.h>
#include <stdio.h>
#include "Player.h"

// g++ -Wall *.cpp -o tutorial -lmingw32 -lSDL2main -lSDL2

// Funciones
Uint32 keyPressed(Uint32 interval, void* obj);
Uint32 rendering(Uint32 interval, void* obj);
// Render
SDL_Renderer* renderer = NULL;
// Ventanas
SDL_Window* window = NULL;
int width = 640;
int height = 480;
// Variables de entorno
bool endGame = false;
// Imagenes
SDL_Surface* player_img = NULL;
// Variables
SDL_TimerID phisics;
SDL_TimerID graphics;
const Uint8 *keystate;
Player* player = NULL;

int main(int argc, char* args[]){

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){//Iniciar SDL con video
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
	}
	else{
		// SDL_WINDOW_HIDDEN,  SDL_WINDOW_SHOWN
		window = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return -1;
		}
	}
    //SDL_Delay(1000); // Uint32 ms

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	player_img = SDL_LoadBMP("../imgs/player_v2.bmp");
	//SDL_Texture* tex =  SDL_CreateTextureFromSurface(renderer, sonic);
    //SDL_FreeSurface(sonic);

    int delay = 1000/30;

    player = new Player(renderer, player_img);

    phisics = SDL_AddTimer(delay, keyPressed, NULL);
    graphics = SDL_AddTimer(delay, rendering, (void*)player);

	while(!endGame){
		//SDL_RenderPresent(renderer);
        SDL_Event e;//recibo evento de la ventana
        if (SDL_PollEvent(&e)) {
            switch(e.type){
                case SDL_QUIT:
                    endGame = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym){
                        case SDLK_q:
                            break;
                        case SDLK_b: 
							endGame = true;  
                            break;
                        case SDLK_c:  
                            break;
                        case SDLK_SPACE:   
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym){
                        /* */
                    }
                    break;
            };
		}
	}

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
// PhisicsEngine
Uint32 keyPressed(Uint32 interval, void* obj){
    keystate = SDL_GetKeyboardState(NULL);
    int x = player->get_x();
    int y = player->get_y();
    int vel_y = player->get_vel_y();

    /*if(keystate[SDL_SCANCODE_DOWN]){
        y += 5;
    }
    if(keystate[SDL_SCANCODE_UP]){
        y -= 5;
    }*/
    if(keystate[SDL_SCANCODE_RIGHT]){
        x += 5;
    }
    if(keystate[SDL_SCANCODE_LEFT]){
        x -= 5;
    }
    if(keystate[SDL_SCANCODE_SPACE] && y == 380){
        vel_y = -80;
    }

    player->set_y(y);
    player->set_x(x);
    player->set_vel_y(vel_y);

    player->update();

    return interval;
}
// GraphicsEngine
Uint32 rendering(Uint32 interval, void* obj){
    ((Player*)obj)->render();
    SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

    return interval;
}