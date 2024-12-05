## Programa 4.1

En este programa utilizamos las matrices del modelo, la vista y la proyección para dibujar cubos en tres dimensiones. Actualizando las matrices usando un parámetro que representa el tiempo, podemos animar objetos y hacer que se muevan y giren por el espacio.

El programa dibuja una multitud de cubos, animados de manera independiente. Logramos esto actualizando las matrices en cada paso y llamando nuevamente a `glDrawArrays()`, creando así un nuevo cubo distinto de los anteriormente dibujados.
