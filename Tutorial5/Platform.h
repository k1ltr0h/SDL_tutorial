#include "GameObject.h"

class Platform: public GameObject{
public:
    Platform(SDL_Surface* sprite,  SDL_Rect rect_, Vector2D pos_);
    void render(SDL_Renderer* renderer) override;
    void update(float dt, 
         Vector2D& camera_offset, 
         const std::vector<GameObject*>& allObjects) override;

    void set_total_width(int total_width_pixels) { total_width_ = total_width_pixels; }
    int  get_total_width() const { return total_width_; }

private:
    int total_width_ = 0; ///< Ancho total del sprite para el tile
    int total_height_ = 0; ///< Altura total del sprite para el tile
    int tile_width_ = 0; ///< Ancho de cada tile
    int tile_height_ = 0; ///< Altura de cada tile
};