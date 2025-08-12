// GameConfig.h
#pragma once

class GameConfig {
public:
    static GameConfig& get() {
        static GameConfig instance;
        return instance;
    }

    float get_gravity() const { return gravity_; }
    float get_eps() const { return eps_; }
    int get_targetFPS() const { return target_fps_; }
    int get_frames_per_sprite() const { return frames_per_sprite_; }
    int get_anim_steps_per_sec() const { return anim_steps_per_sec_; }

    void set_gravity(float g) { gravity_ = g; }
    void set_eps(float e) { eps_ = e; }

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


private:
    GameConfig() = default;
    float gravity_ = 2000.0f;  // px/s²
    float eps_ = 0.0f;        // tolerancia numérica
    int target_fps_ = 30;       // cuadros por segundo
    int frames_per_sprite_ = 5;
    int anim_steps_per_sec_ = 6;  // pasos de animación por segundo
};

// Se crea una instancia global de configuración
// para que pueda ser accedida desde cualquier parte del código.
inline GameConfig& config = GameConfig::get();