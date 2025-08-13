// GameConfig.h
#pragma once

class GameConfig {
public:
    static GameConfig& get() {
        static GameConfig instance;
        return instance;
    }

    int get_window_width() const { return window_width; }
    int get_window_height() const { return window_height; }
    int get_targetFPS() const { return target_fps_; }
    int get_frames_per_sprite() const { return frames_per_sprite_; }
    int get_anim_steps_per_sec() const { return anim_steps_per_sec_; }
    float get_gravity() const { return gravity_; }
    float get_eps() const { return eps_; }

    void set_window_size(int width, int height) {
        window_width = width;
        window_height = height;
    }
    void set_targetFPS(int fps) {
        if(fps <= 0) return;
        if (fps != target_fps_) {
            target_fps_ = fps;
            frames_per_sprite_ = target_fps_ / anim_steps_per_sec_;
        }
    }
    void animStepsPerSec(int steps) {
        if(steps <= 0) return;
        anim_steps_per_sec_ = steps;
    }
    void set_gravity(float g) { gravity_ = g; }
    void set_eps(float e) { eps_ = e; }


private:
    GameConfig() = default;
    int window_width = 640;
    int window_height = 480;
    int target_fps_ = 30;       // cuadros por segundo
    int frames_per_sprite_ = 5;
    int anim_steps_per_sec_ = 6;  // pasos de animación por segundo
    float gravity_ = 2000.0f;  // px/s²
    float eps_ = 0.0f;        // tolerancia numérica
};

// Se crea una instancia global de configuración
// para que pueda ser accedida desde cualquier parte del código.
inline GameConfig& config = GameConfig::get();