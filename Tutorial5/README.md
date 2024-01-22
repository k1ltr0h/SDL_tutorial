# Tutorial parte 4

## Actualizaciones

* Motor refactorizado.

* GameObject se encarga de centrar sprites y actualizar src_rect y dst_rect.
* Toda la lógica de Init se a trasladado a constructores.
* Nuevas clases Box, Layer, Stage y Stage0.
* Stage representa el entorno (simulación) en el que los objetos interactúan entre sí. Por lo tanto, es quien se encarga de actualizar y renderizar desde el fondo, efectos, personajes y otros objetos.
* Cambios en nombres de variables.
* Cambios en el formato de nombres en variables y funciones (minúsculas y separados con _).
* Gameobject cuenta con nuevas variables como on_air y orientation. La primera es un bool que representa si el objeto está en el aire o tocando el suelo y el segundo es utilizado para saber hacia donde mira el objeto.

* Entre otros (ver video en youtube).
