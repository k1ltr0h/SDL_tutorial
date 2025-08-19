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
// Variables de entorno
bool endGame = false;
// Imagenes
SDL_Surface* player_img = NULL;
// Variables
SDL_Event e; ///< Eventos de SDL
const Uint8 *keystate;
// Objetos
Player* player = NULL;
Stage* playground = NULL;

int main(int argc, char* args[]){

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){//Iniciar SDL con video
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return -1;
	}
	else{
		// SDL_WINDOW_HIDDEN,  SDL_WINDOW_SHOWN
		window = SDL_CreateWindow("Tutorial", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                config.get_window_width(), 
                                config.get_window_height(), 
                                SDL_WINDOW_SHOWN);
		if( window == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return -1;
		}
	}

	player_img = SDL_LoadBMP("../imgs/player_v3_3.bmp");

    if (!player_img) {
        SDL_Log("Error cargando imagen: %s", SDL_GetError());
    }

    player = new Player(player_img);
    playground = new Stage0(window, player);

    renderer = playground->get_renderer();

    int frame_delay = 1000 / config.get_targetFPS(); // El tiempo de espera entre frames en milisegundos
    if(frame_delay < 0) {
        frame_delay = 0; // Asegurarse de que no sea negativo
    }

    Uint32 last_ticks = SDL_GetTicks();

	while(!endGame){
        Uint32 now = SDL_GetTicks();
        float dt = (now - last_ticks) / 1000.0f;  // Tiempo transcurrido en segundos
        last_ticks = now;

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
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym){
                        case SDLK_LEFT:
                            break;
                        case SDLK_RIGHT:
                            break;
                    }
                    break;
            };
		}

        // Espera lo necesario para mantener el frame rate
        Uint32 frame_time = SDL_GetTicks() - now;
        if (frame_time < (Uint32)frame_delay) {
            SDL_Delay(frame_delay - frame_time);
        }
	}

    delete player;
    delete playground;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
// PhisicsEngine
void keyPressed(float dt){
    keystate = SDL_GetKeyboardState(NULL);

    // Eje x
    if(keystate[SDL_SCANCODE_RIGHT]){
        player->move(GameObject::dir::RIGHT);
    }
    if(keystate[SDL_SCANCODE_LEFT]){
        player->move(GameObject::dir::LEFT);
    }
    if(keystate[SDL_SCANCODE_LSHIFT]){
        player->stop(GameObject::axis::ABSCISSA);
    }

    // Eje y
    if(keystate[SDL_SCANCODE_SPACE] && !player->has_jumped_since_grounded()){
        player->jump();
    }
}

void phisics(float dt, Stage* playground){
    // Si el boton de movimiento horizontal no está presionado,
    // y la velocidad es 0, se detiene la animación del jugador.
    if(player->get_velocity().get_x() == 0)
        player->set_move_dir_x(0);
    // Recoger las teclas presionadas
    keyPressed(dt);
    // Actualizar la posición de los objetos en el escenario
    playground->update(dt);
}

// GraphicsEngine
void graphics(Stage* playground){
    // Actualizar texturas y renderizar objetos
    playground->render();
 
    // Actualizar pantalla
    SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

}