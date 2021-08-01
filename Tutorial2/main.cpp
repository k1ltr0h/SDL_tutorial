#include <SDL2/SDL.h>
#include <stdio.h>

// g++ -Wall main.cpp -o tutorial -lmingw32 -lSDL2main -lSDL2

SDL_Renderer* renderer = NULL;
//Ventanas
SDL_Window* window = NULL;
int width = 640;
int height = 480;
// Variables de entorno
bool endGame = false;
// Imagenes
SDL_Surface* sonic = NULL;

int main(int argc, char* args[]){

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){//Iniciar SDL con video
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
	}
	else{
		// SDL_WINDOW_HIDDEN,  SDL_WINDOW_SHOWN
		window = SDL_CreateWindow( "Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return -1;
		}
	}
    //SDL_Delay(1000); // Uint32 ms

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	sonic = SDL_LoadBMP("sonic.bmp");
	SDL_Texture* tex =  SDL_CreateTextureFromSurface(renderer, sonic);
    SDL_FreeSurface(sonic);
	int x = 0;
	int y = 0;
	SDL_Rect rect = {x,y,100,100};

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
							printf("hola");
							//SDL_Log("sdffasd");
                            break;
                        case SDLK_b: 
							endGame = true;  
                            break;
                        case SDLK_c:  
                            break;
                        case SDLK_SPACE:   
                            break;
                        case SDLK_LEFT:
							x -= 2;
                            break;
                        case SDLK_RIGHT:
							x += 2;
                            break;
                        case SDLK_UP:
							y -= 2;
                            break;
                        case SDLK_DOWN:
							y += 2;
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
		if(y > 200){
			y = 200;
		}
		//SDL_Delay(1000);
		rect = {x,y,100,100};
		SDL_RenderCopy(renderer, tex, NULL, &rect);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}