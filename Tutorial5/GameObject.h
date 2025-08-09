#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <algorithm>   // std::max, std::min, std::clamp
#include <limits>      // std::numeric_limits
#include <vector>
#include <cmath>
#include "Vector2D.h"

class GameObject{
public:
    // ——— Enumeraciones ———————————————————————
    /** @brief Tipos de dirección para move(). */
    enum dir { RIGHT = 0, UP = 90, LEFT = 180, DOWN = 270 };

    /** @brief Ejes para flip y stop(). */
    enum axis { ABSCISSA, ORDINATE, ALL };

    /** @brief Clasificación genérica de objetos. */
    enum type { PLAYER, PLATFORM, NPC };

    struct Hitbox {
        Vector2D center;   // centro del hitbox (coordenadas absolutas)
        int width;
        int height;

        SDL_Rect to_rect() const {
            return {
                int(center.get_x() - width / 2),
                int(center.get_y() - height / 2),
                width,
                height
            };
        }

        // Devuelve los bordes directamente
        float left()   const { return center.get_x() - width / 2.0f; }
        float right()  const { return center.get_x() + width / 2.0f; }
        float top()    const { return center.get_y() - height / 2.0f; }
        float bottom() const { return center.get_y() + height / 2.0f; }
    };

    // ——— Constructores / Destructor —————————————
    /** 
     * @brief Construye un GameObject.
     * @param surface      Superficie SDL a renderizar (puede ser nullptr).
     * @param dstRect      Rectángulo destino en pantalla.
     * @param startPos     Posición inicial (centro) en coordenadas de mundo.
     * @param collidable   Si false, no participa en colisiones.
     * @param gravityOn    Si true, aplica aceleración de gravedad.
     */
    GameObject(SDL_Surface* surface = nullptr,
               SDL_Rect dstRect = {0,0,100,100},
               Vector2D startPos = Vector2D(),
               bool collidable = true,
               bool gravityOn = true);
    /** @brief Libera recursos asociados (textura y superficie). */
    virtual ~GameObject();

    // ——— Renderizado —————————————————————————
    /** 
     * @brief Dibuja el objeto en pantalla.
     * @param renderer  Contexto SDL_Renderer donde pintar.
     */
    virtual void render(SDL_Renderer* renderer);

    /**
     * @brief Actualiza física y colisiones del objeto.
     * 
     * - Integra velocidad y posición.
     * - Realiza broad‐phase contra allObjects.
     * - Comprueba y resuelve colisiones continuas (swept AABB).
     * - Ajusta hit_box y dst_rect.
     * 
     * @param dt          Tiempo transcurrido desde el último frame (s).
     * @param allObjects  Lista de todos los GameObject* del escenario.
     */
    virtual void update(float dt, const std::vector<GameObject*>& allObjects, bool centered = true);

    /**
     * @brief Cambia la aceleración según la dirección indicada.
     * @param direction  Uno de los valores de la enum dir.
     */
    virtual void move(int direction);

    /**
     * @brief Detiene velocidad y aceleración en el eje dado.
     * @param axis  ABSCISSA para X, ORDINATE para Y.
     */
    void stop(axis axis);

    /** @brief Aplica freno horizontal automático. */
    void brake();

    /** @brief Fija al suelo si pos.y ≥ GROUND (constante en .cpp). */
    void ground();

    /**
     * @brief Recalcula el hit_box y dst_rect centrados en pos.
     * Para sprites que definen su box alrededor de su centro.
     */
    void center();

    /**
     * @brief Blitea otro GameObject sobre la superficie interna.
     * @param obj  Puntero al objeto que se dibujará aquí.
     */
    void blit_surface(GameObject* obj);

    /**
     * @brief Voltea la imagen internamente.
     * @param which  Eje a voltear: ABSCISSA (X), ORDINATE (Y) o ALL.
     */
    void flip_surface(axis which = ABSCISSA);

    /**
     * @brief Calcula la velocidad futura sin mutar el estado.
     * @param dt  Delta‐time en segundos.
     * @return La velocidad resultante vel + acc·dt (pre‐clamp).
     */
    Vector2D predict_velocity(float dt) const;

    /**
     * @brief Calcula la posición futura sin mutar el estado.
     * @param dt  Delta‐time en segundos.
     * @return La posición p + v·dt + 0.5·a·dt².
     */
    Vector2D predict_position(float dt) const;

    /**
     * @brief Actualiza la aceleración sobre dt y actualiza vel internamente.
     *        Equivale a: vel = predict_velocity(dt).
     * @param dt  Delta-time en segundos.
     */
    void update_velocity(float dt);

    /**
     * @brief Actualiza la posicion sobre dt y actualiza pos internamente.
     *        Equivale a: prev_pos = pos; pos = predict_position(dt);
     * @param dt  Delta-time en segundos.
     */
    void update_position(float dt);

    /**
     * @brief Detecta y resuelve colisiones con otros objetos de la escena.
     *
     * Esta función realiza todo el proceso de gestión de colisiones:
     * - Calcula el volumen de barrido (broad-phase) para descartar objetos lejanos.
     * - Ejecuta el algoritmo de detección de colisiones configurado (por defecto, swept_aabb()).
     * - Determina el instante de colisión y ajusta la posición del objeto para evitar penetraciones.
     * - Aplica la respuesta física (snap al borde del objeto, anulación de velocidad en el eje de colisión).
     * - Integra el movimiento restante en el frame después de la colisión.
     *
     * La implementación está pensada para que, en el futuro, se pueda cambiar el algoritmo
     * de detección de colisiones o añadir otros, sin modificar el resto de la lógica del motor.
     *
     * @param dt Delta de tiempo en segundos para el frame actual.
     * @param allObjects Vector con punteros a todos los objetos presentes en la escena.
     *
     * @note
     * - Esta función asume que la posición `pos` del objeto es el centro de la base (bottom-center) del sprite.
     * - El hitbox utilizado debe estar correctamente definido para cada objeto (posición central + dimensiones).
     * - Si existen múltiples colisiones en un frame, se resuelve la más temprana.
     *
     * @warning
     * - La función no gestiona rebotes (elasticidad). La velocidad en el eje de impacto siempre se pone a cero.
     * - Es responsabilidad del llamador invocar esta función antes de renderizar, para asegurar que la posición final
     *   del objeto ya esté corregida tras las colisiones.
     */
    void resolve_collisions(float dt, const std::vector<GameObject*>& allObjects);


    /**
     * @brief Test de colisión continua (Swept AABB) usando desplazamiento dx,dy.
     * @param obstacle   AABB del obstáculo.
     * @param dx         Desplazamiento en X en este frame.
     * @param dy         Desplazamiento en Y en este frame.
     * @param outT       Fracción [0..1] en la que ocurre la colisión.
     * @param outAxis    0 = eje X, 1 = eje Y.
     * @return true si hay colisión.
     */
    bool swept_aabb(const GameObject& obstacle,
                    float           dx,
                    float           dy,
                    float&          outT,
                    int&            outAxis) const;


    // ——— Getters ———————————————————————————————————————————

    /**
     * @brief Devuelve la superficie SDL asociada al objeto.
     * @return Puntero a la SDL_Surface, o nullptr si no hay ninguna.
     */
    SDL_Surface* get_surface() const {
        return surface;
    }

    /**
     * @brief Devuelve la textura SDL usada para renderizado.
     * @return Puntero a la SDL_Texture, o nullptr si no se ha creado aún.
     */
    SDL_Texture* get_texture() const {
        return texture;
    }

    /**
     * @brief Posición actual (centro) del objeto.
     * @return Referencia constante a un Vector2D con las coordenadas (x,y).
     */
    const Vector2D& get_position() const{
        return pos;
    }

    /**
     * @brief Obtiene la posición actual del objeto.
     *
     * Devuelve una referencia al vector interno de posición,
     * cuyas componentes x e y representan las coordenadas
     * del centro del objeto en el espacio de juego.
     * Al retornar por referencia, permite tanto leer
     * como actualizar directamente la posición.
     *
     * @return Vector2D& Referencia al vector de posición.
     */
    Vector2D& get_position(){
        return pos;
    }

    /**
     * @brief Velocidad actual del objeto.
     * @return Referencia constante a un Vector2D con la velocidad (vx,vy).
     */
    const Vector2D& get_velocity() const{
        return vel;
    }

    /**
     * @brief Obtiene la velocidad actual del objeto.
     *
     * Devuelve una referencia al vector interno de velocidad,
     * cuyas componentes x e y representan la velocidad
     * del objeto en píxeles por segundo.
     * Al retornar por referencia, permite tanto consultar
     * como modificar directamente la velocidad.
     *
     * @return Vector2D& Referencia al vector de velocidad.
     */
    Vector2D& get_velocity(){
        return vel;
    }

    /**
     * @brief Aceleración actual del objeto.
     * @return Referencia constante a un Vector2D con la aceleración (ax,ay).
     */
    const Vector2D& get_acceleration() const{   
        return acc;
    }

    /**
     * @brief Obtiene la aceleración actual del objeto (mutable).
     * 
     * Esta versión devuelve una referencia no-const al vector de aceleración
     * interno, permitiendo modificar directamente sus componentes.
     * 
     * @return Referencia al Vector2D interno `acc`.
     */
    Vector2D& get_acceleration() {   
        return acc;
    }

    /**
     * @brief Devuelve el rectángulo fuente para extracción de la textura.
     * @return Referencia al SDL_Rect que define la subregión de la superficie.
     */
    SDL_Rect& get_src_rect(){
        return src_rect;
    }

    /**
     * @brief Devuelve el rectángulo destino en pantalla.
     * @return Referencia al SDL_Rect que define dónde se dibuja el sprite.
     */
    SDL_Rect& get_dst_rect(){
        return dst_rect;
    }

    /**
     * @brief Devuelve la caja de colisión actual.
     * @return Referencia constante al SDL_Rect usado como hit-box.
     */
    const Hitbox& get_hit_box() const{
        return hit_box;
    }

    /**
     * @brief Altura (en píxeles) de la superficie original.
     * @return Entero con la altura en píxeles.
     */
    int get_surface_height() const{
        return surface_height;
    }

    /**
     * @brief Anchura (en píxeles) de la superficie original.
     * @return Entero con la anchura en píxeles.
     */
    int get_surface_width() const{
        return surface_width;
    }

    /**
     * @brief Profundidad de dibujo (orden Z) del objeto.
     * @return Valor sin signo; mayor = se dibuja encima.
     */
    unsigned int get_depth() const{
        return depth;
    }

    /**
     * @brief Tipo de objeto (PLAYER, PLATFORM, NPC).
     * @return Valor de la enum type.
     */
    type get_object_type() const{
        return object_type;
    }

    /**
     * @brief Indica si el objeto participa en colisiones.
     * @return true si el objeto es collidable.
     */
    bool is_collidable() const{
        return collidable;
    }

    /**
     * @brief Consulta si el objeto está en el aire.
     * @return true si no está en contacto con el suelo.
     */
    bool get_on_air() const{
        return on_air;
    }

    /**
     * @brief Indica si la textura está volteada horizontalmente.
     * @return true si hay flip horizontal activo.
     */
    bool get_horizontal_flip() const{
        return horizontal_flip;
    }

    /**
     * @brief Indica si la textura está volteada verticalmente.
     * @return true si hay flip vertical activo.
     */
    bool get_vertical_flip() const{ 
        return vertical_flip;
    }


    // ——— Setters ———————————————————————————————————————————

    /**
     * @brief Asigna una nueva superficie SDL al objeto.
     * @param sur_ Puntero a la SDL_Surface (se libera la anterior si existía).
     */
    void set_surface(SDL_Surface* sur_){
        SDL_FreeSurface(surface);
        surface = sur_;
    }

    /**
     * @brief Asigna una nueva textura SDL al objeto.
     * @param tex_ Puntero a la SDL_Texture (se destruye la anterior si existía).
     */
    void set_texture(SDL_Texture* tex_){
        SDL_DestroyTexture(texture);
        texture = tex_;
    }

    /**
     * @brief Establece la posición actual (centro) del objeto.
     * @param x_ Coordenada X en píxeles.
     * @param y_ Coordenada Y en píxeles.
     */
    void set_current_pos(float x_, float y_){
        pos.set_coord({x_, y_});
    }

    /**
     * @brief Establece la velocidad actual del objeto.
     * @param x_ Componente horizontal de la velocidad (px/s).
     * @param y_ Componente vertical de la velocidad (px/s).
     */
    void set_current_vel(float x_, float y_){
        vel.set_coord({x_, y_});
    }

    /**
     * @brief Establece la aceleración actual del objeto.
     * @param x_ Componente horizontal de la aceleración (px/s²).
     * @param y_ Componente vertical de la aceleración (px/s²).
     */
    void set_current_acc(float x_, float y_){
        acc.set_coord({x_, y_});
    }

    /**
     * @brief Define un nuevo rectángulo destino en pantalla.
     * @param x_ Coordenada X superior-izquierda.
     * @param y_ Coordenada Y superior-izquierda.
     * @param w_ Anchura en píxeles.
     * @param h_ Altura en píxeles.
     */
    void set_dst_rect(int x_, int y_, int w_, int h_){
        dst_rect = {x_, y_, w_, h_};
    }

    /**
     * @brief Define un nuevo rectángulo fuente de la superficie.
     * @param x_ Coordenada X dentro de la superficie.
     * @param y_ Coordenada Y dentro de la superficie.
     * @param w_ Anchura de la región fuente.
     * @param h_ Altura de la región fuente.
     */
    void set_src_rect(int x_, int y_, int w_, int h_){
        src_rect = {x_, y_, w_, h_};
    }

    /**
     * @brief Ajusta únicamente las dimensiones del rectángulo destino.
     * @param w_ Nueva anchura en píxeles.
     * @param h_ Nueva altura en píxeles.
     */
    void set_dst_rect_dimensions(int w_, int h_){
        dst_rect.w = w_;
        dst_rect.h = h_;
    }

    /**
     * @brief Ajusta únicamente las dimensiones del rectángulo fuente.
     * @param w_ Nueva anchura de la subregión.
     * @param h_ Nueva altura de la subregión.
     */
    void set_src_rect_dimensions(int w_, int h_){
        src_rect.w = w_;
        src_rect.h = h_;
    }

    /**
     * @brief Mueve el rectángulo destino a una nueva posición.
     * @param x_ Coordenada X superior-izquierda.
     * @param y_ Coordenada Y superior-izquierda.
     */
    void set_dst_rect_position(int x_, int y_){
        dst_rect.x = x_;
        dst_rect.y = y_;
    }

    /**
     * @brief Mueve el rectángulo fuente dentro de la superficie.
     * @param x_ Coordenada X de inicio.
     * @param y_ Coordenada Y de inicio.
     */
    void set_src_rect_position(int x_, int y_){
        src_rect.x = x_;
        src_rect.y = y_;
    }

    /**
     * @brief Establece la profundidad de dibujo (orden Z).
     * @param depth_ Valor sin signo; mayor = dibuja encima.
     */
    void set_depth(unsigned int depth_){
        depth = depth_;
    }

    /**
     * @brief Marca si el objeto está en el aire.
     * @param on_air_ true = en el aire; false = apoyado.
     */
    void set_on_air(bool on_air_){
        on_air = on_air_;
    }

    /**
     * @brief Activa o desactiva el flip horizontal.
     * @param horizontal_flip_ true = volteo horizontal.
     */
    void set_horizontal_flip(bool horizontal_flip_){
        horizontal_flip = horizontal_flip_;
    }

    /**
     * @brief Activa o desactiva el flip vertical.
     * @param vertical_flip_ true = volteo vertical.
     */
    void set_vertical_flip(bool vertical_flip_){
        vertical_flip = vertical_flip_;
    }

    /**
     * @brief Establece una hit-box personalizada.
     * @param hitbox_ Referencia constante al SDL_Rect que define la nueva caja.
     */
    void set_hit_box(const Hitbox& hitbox_){
        hit_box = hitbox_;
    }

    /**
     * @brief Establece el tipo de este objeto.
     * @param newType  Uno de los valores de la enumeración type:
     *                 PLAYER, PLATFORM o NPC.
     */
    void set_object_type(type type_){
        object_type = type_;
    }


private:
    /// Rectángulo fuente dentro de la superficie SDL (x, y, w, h).
    SDL_Rect src_rect;

    /// Rectángulo destino en pantalla (x, y, w, h).
    SDL_Rect dst_rect;

    /// Caja de colisión (hit-box) actual.
    Hitbox hit_box;

    /// Superficie SDL original. Puede ser nullptr si no se ha cargado.
    SDL_Surface* surface = nullptr;

    /// Textura SDL en GPU, creada a partir de `surface`. nullptr hasta crearla.
    SDL_Texture* texture = nullptr;

    /// Posición actual del centro en coordenadas de mundo.
    Vector2D pos;

    /// Velocidad actual en píxeles por segundo.
    Vector2D vel;

    /// Aceleración actual en píxeles por segundo al cuadrado.
    Vector2D acc;

    /// Tipo de objeto (PLAYER, PLATFORM, NPC).
    type object_type;

    /// Si es true, se aplica la aceleración de gravedad cada update.
    bool gravity_activated;

    /// Si es true, participa en los tests de colisión.
    bool collidable;

    /// True cuando el objeto está en el aire (no apoyado).
    bool on_air = false;

    /// True si la textura está volteada horizontalmente.
    bool horizontal_flip = false;

    /// True si la textura está volteada verticalmente.
    bool vertical_flip = false;

    /// Velocidad máxima permitida en X (px/s).
    int max_vel_x;

    /// Velocidad máxima permitida en Y (px/s).
    int max_vel_y;

    /// Anchura de la superficie original (px).
    int surface_width;

    /// Altura de la superficie original (px).
    int surface_height;

    /// Profundidad de dibujo (orden Z): valores mayores se renderizan encima.
    unsigned int depth;
};

