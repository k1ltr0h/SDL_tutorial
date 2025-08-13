#include "GameObject.h"

class Layer: public GameObject{
public:
    Layer(SDL_Window* window_, 
          SDL_Surface* sur_, 
          SDL_Rect rect_={0,0,640,480}, 
          unsigned int depth_=100);
    virtual ~Layer();

    void set_depth(int depth_);
    int get_depth();

private:
    SDL_Surface* surface;
    int width, height;
    int dst_rect_w, dst_rect_h;
    int src_rect_w, src_rect_h;
    Vector2D::Coord src_coord;
    Vector2D::Coord dst_coord;
    int depth;
    
};