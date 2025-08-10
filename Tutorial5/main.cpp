/**
 * \mainpage Tutorial SDL2
 *
 * Bienvenido a la documentación del proyecto.
 *
 * En estos tutoriales de pretende explicar diferentes funcionalidad básicas de SDL2,\n
 * todo por medio de un plataformero 2D hecho en casa y por el amor al arte. 
 * 
 * Compilar proyecto con: `g++ -Wall *.cpp -o tutorial -lmingw32 -lSDL2main -lSDL2`\n
 * Agregar `-mwindows` para ocultar la consola.
 * 
 * Generar Icono con: `windres icon.rc -O coff -o icon.res`\n
 * Agregar `icon.res` al compilar para vincular icono al ejecutable.
 * 
 * **Ejemplo:** `g++ -Wall *.cpp -o tutorial -lmingw32 -lSDL2main -lSDL2 icon.res -mwindows`
 * 
 * - [Clase principal](classGameObject.html)
 * - [Archivo principal](main_8cpp.html)
 * 
 * ![Descripción de la imagen](../../../imgs/GameEngine_v2.png)
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include "GameConfig.h"
#include "Player.h"
#include "Stage0.h"

// Funciones
void keyPressed(float dt);
void phisics(float dt, Stage* playground);
void graphics(Stage* playground);
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
SDL_Event e; ///< Eventos de SDL
const Uint8 *keystate;
Player* player = NULL;
Stage* playground = NULL;

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

	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	player_img = SDL_LoadBMP("../imgs/player_v2.bmp");
	//SDL_Texture* tex =  SDL_CreateTextureFromSurface(renderer, sonic);
    //SDL_FreeSurface(sonic);

    int frameDelay = 1000 / config.get_targetFPS();

    player = new Player(player_img, Vector2D(100, 100));

    playground = new Stage0(window, player);

    renderer = playground->get_renderer();

    Uint32 lastTicks = SDL_GetTicks();

	while(!endGame){
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTicks) / 1000.0f;  // dt en segundos
        lastTicks = now;

		//SDL_RenderPresent(renderer);
        phisics(dt, playground);
        graphics(playground);
        
        if (SDL_PollEvent(&e)) {
            switch(e.type){
                case SDL_QUIT:
                    endGame = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym){
                        case SDLK_ESCAPE:
                            endGame = true;
                            break;
                        case SDLK_q: 
                            break;
                        case SDLK_b:  
                            break;
                        case SDLK_c:  
                            break;
                        case SDLK_SPACE:   
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym){
                        case SDLK_LEFT:
                            //player->set_directionValue(Player::LEFT, false);
                            break;
                        case SDLK_RIGHT:
                            //player->set_directionValue(Player::RIGHT, false);
                            break;
                    }
                    break;
            };
		}

        // Espera lo necesario para mantener el frame rate
        Uint32 frameTime = SDL_GetTicks() - now;
        if (frameDelay > 0 && frameTime < (Uint32)frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
	}

    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
// PhisicsEngine
void keyPressed(float dt){
    keystate = SDL_GetKeyboardState(NULL);
    //Vector2D* acc = player->get_current_acc();
    //Vector2D* pos = player->get_current_pos();
    //int vel_y = player->get_vel_y();

    /*if(keystate[SDL_SCANCODE_DOWN]){
        y += 5;
    }
    if(keystate[SDL_SCANCODE_UP]){
        y -= 5;
    }*/
    // Eje x

    //printf("keystate: %d-%d ", keystate[SDL_SCANCODE_RIGHT], keystate[SDL_SCANCODE_LEFT]);

    if(keystate[SDL_SCANCODE_RIGHT]){
        //x += 8;
        player->move(GameObject::dir::RIGHT);
        //player->set_directionValue(Player::RIGHT, true);
    }
    if(keystate[SDL_SCANCODE_LEFT]){
        //x -= 8;
        player->move(GameObject::dir::LEFT);
        //player->set_directionValue(Player::LEFT, true);
    }
    if(keystate[SDL_SCANCODE_LSHIFT]){
        player->stop(GameObject::axis::ABSCISSA);
       /* if(player->get_directionValue(Player::RIGHT) && !player->get_directionValue(Player::LEFT)){
            x += 7;
        }
        else if(!player->get_directionValue(Player::RIGHT) && player->get_directionValue(Player::LEFT)){
            x -= 7;
        }*/
    }
    // Eje y
    if(keystate[SDL_SCANCODE_SPACE] && !player->get_on_air()){
        player->jump();
        //acc->setY(15); // va a volar, REVISAR!!!
    }
}

void phisics(float dt, Stage* playground){
    if(player->get_velocity().get_x() == 0)
        player->set_move_dir_x(0);

    keyPressed(dt);
    // Actualizar la posición de los objetos en el escenario
    playground->update(dt);
}

// GraphicsEngine
void graphics(Stage* playground){
    playground->render();

    SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

}