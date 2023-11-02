# Tutorial parte 4
## Actualizaciones

* GameObject tiene un atributo "sprite" de tipo puntero SLD_Surface (Eliminado "skin" en Player).
* GameObject init ahora incluye parámetro opcional "sur_" ("sprite"/surface).
* Nueva clase Layer.
* El método render pertenecerá a la clase Layer y se usará posteriormente en la clase Stage, con el fin de crear una única textura (Ambiente donde conviven los objetos e interactuan).
* Parámetros width y height de GameObject opcionales, si su ancho y alto es igual a la imagen (surface/"sprite") utilizado simplemente omitir.