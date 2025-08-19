# Tutorial parte 5

## Actualizaciones

1. Sistema de física y colisiones

* Antes:
    - La gravedad estaba “hardcodeada” con un valor fijo.
    - Las colisiones eran muy básicas, con detección limitada y posiciones forzadas (ej. GROUND fijo).
* Ahora:
    - Se implementó un sistema completo de colisiones continuas (Swept AABB) con broad-phase para descartar objetos lejanos y narrow-phase precisa.
    - Soporta múltiples impactos en un mismo frame.
    - El cálculo usa dt real, lo que hace que la física sea independiente del framerate.
    - Hitboxes configurables y desacoplados del tamaño del sprite.
2. Animación de sprites
* Antes:
    - No existía un sistema de animación automático, solo render estático.
* Ahora:
    - Se agregó animación por sprite sheet con control de frames desde GameConfig.
    - Soporta flip horizontal automático al cambiar de dirección.
    - El personaje solo avanza de frame si hay movimiento (optimización visual).
3. Gestión de objetos y clases nuevas
* Antes:
    - Objetos como Box y Background heredaban directamente de GameObject o Layer.
    - No había un objeto específico para suelos.
* Ahora:
    - Se añade Platform, que puede dibujarse en mosaico para crear suelos y muros largos.
    - GameObject ahora maneja tipos de objeto (PLAYER, PLATFORM, NPC) y estado (static, on_air, etc.).
    - Se introduce un coyote time infinito: el jugador puede saltar una vez aunque ya esté “en el aire” desde que dejó el suelo.
4. Cámara y escenario
* Antes:
    - La cámara estaba fija y el render centraba manualmente objetos.
* Ahora:
    - El Stage integra una cámara con seguimiento al jugador y límites según el tamaño del fondo.
    - update_dst_rect_from_pos() centraliza el cálculo de posiciones en pantalla usando offset de cámara.
5. Control y loop principal
* Antes:
    - Uso de SDL_AddTimer para física y render, con tiempos fijos.
* Ahora:
    - Un solo bucle principal que mide dt en cada frame, sincronizando física y render con SDL_GetTicks().
    - Esto elimina dependencias de temporizadores y hace que la simulación sea suave y consistente.
6. Configuración centralizada
* Antes:
    - Parámetros como FPS, gravedad y tamaño de ventana estaban dispersos o fijos en el código.
* Ahora:
    - Se añade GameConfig para manejar valores globales como FPS objetivo, gravedad, tolerancia de colisiones y velocidad de animación.
    - Facilita ajustes sin tocar el código principal.
