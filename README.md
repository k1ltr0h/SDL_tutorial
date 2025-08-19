# SDL C/C++ Tutorial en Español

## Contenido

* El 1er tutorial no lo alcanzé a apartar pero contenía la instalación de SDL y MingW, 
los paths y la creación de la ventana.
* Tutorial 2: Surface, Texture, Renderer, dibujar en pantalla.
* Tutorial 3: Gravedad, Movimiento fluido, Motores(gráfico y físico), memory leaks, clases(GameObject y Player), 
transparencia y cambio de sprite.

* Tutorial 4: Refactorización del motor. Stage: entorno de juego. Cámara sigue al jugador. Fondos, más objetos e icono para ejecutable. Nuevas clases (Stage, Stage0, Layer, Background, Box).
* Tutorial 5: En esta implementación se resolvió la detección de colisiones continuas mediante el método swept AABB: primero se hace un broad-phase construyendo un área de búsqueda que cubre la posición actual y la futura del objeto para descartar rápidamente obstáculos lejanos, y luego con los candidatos se aplica un narrow-phase que calcula, a partir de la velocidad y del desplazamiento (dx, dy), los tiempos de entrada y salida en cada eje para determinar con precisión si, cuándo y sobre qué eje ocurre la colisión dentro del deltaTime; al encontrar un impacto se avanza al instante exacto del choque, se ajusta la posición para “encajar” contra el obstáculo, se anula la velocidad en el eje afectado y se continúa con el tiempo restante del frame, permitiendo detectar colisiones múltiples en un mismo paso de simulación.

**Bien lejos en algún lugar, crecía la esperanza de que el mundo pudiera cambiar. Las tragedias e injusticias siempre han pasado, pero cada cierto tiempo quienes sentían el peso de la injusticia alzaban la voz. Poco a poco los humanos fueron comprendiendo que no tenía sentido despreciar por el sexo, la raza y el color. La humanidad por fin dormía tranquila, aunque nada es perfecto en la sociedad. Aún había personas que seguían luchando por sus derechos y exigiendo equidad. Tampoco acabaron las guerras, solo las vimos pasar. Todo es perfecto si miramos para otro lado. Después de todo la vida es una, y no la quieren malgastar. Mientras tanto en todo lugar, utilizamos y sacamos provecho de los demás. ¿A quien le importa? Dirán, son inferiores, no son humanos. Tenemos derecho a usarlos. Ya sea como peluche, herramienta o disfraz**

**~k1ltr0**

## Como compilar

Ejecutar el siguiente comando en consola:

 `g++ -Wall *.cpp -o tutorial -lmingw32 -lSDL2main -lSDL2`

**Creado por InsuRetro(K1ltr0).**
