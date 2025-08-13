#include <SDL2/SDL.h>

SDL_Surface* surface = nullptr;
enum axis { ABSCISSA, ORDINATE, ALL };

void flip_surface(axis axis){
    SDL_LockSurface(surface); // bloquea la superficie para acceso seguro

    int pitch = surface->pitch; // bytes por fila
    int effective_width = surface->w * surface->format->BytesPerPixel;
    unsigned char* temp = new unsigned char[effective_width]; // buffer temporal
    unsigned char* pixels = (unsigned char*) surface->pixels; // píxeles de la superficie

    if(axis == axis::ABSCISSA){
        // Ancho efectivo de la superficie
        // (BytesPerPixel * ancho de la superficie)
        for(int i=0; i < surface->h; ++i){
            // copia una fila de píxeles a un buffer temporal
            // y luego invierte los colores de esa fila
            unsigned char* row = pixels + i * pitch;
            memcpy(temp, row, pitch);
            // invierte la fila respetando el formato de píxeles
            // (asumiendo formato BGR de 3 bytes por píxel)
            for(int iter=0; iter < effective_width; iter+=3){
                pixels[iter + i*pitch] = temp[effective_width - iter - 3];
                pixels[iter + 1 + i*pitch] = temp[effective_width - iter - 2];
                pixels[iter + 2 + i*pitch] = temp[effective_width - iter - 1];
            }
        } 
    }
    
    else if(axis == axis::ORDINATE){
        for(int i = 0; i < surface->h / 2; ++i) {
            // intercambia filas de píxeles
            // usando un buffer temporal para evitar sobrescritura
            unsigned char* row_above = pixels + i * pitch;
            unsigned char* row_below = pixels + (surface->h - i - 1) * pitch;
            
            // intercambia las filas
            memcpy(temp, row_above, pitch); // destiny, source, bytes size
            memcpy(row_above, row_below, pitch);
            memcpy(row_below, temp, pitch);
        }
    }

    // Liberar memoria temporal
    delete[] temp;

    SDL_UnlockSurface(surface); // desbloquea la superficie
}