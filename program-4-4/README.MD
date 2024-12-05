## Programa 4.4

En este programa implementamos una pila de matrices muy básica, que nos permite anidar objetos, haciendo que los objetos anidados hereden las transformaciones de sus ancestros. Ya que en este programa modelizamos un sistema solar, en el cual la rotación de los cuerpos no se propaga, deshabilitamos la herencia de las matrices de rotación, permitiendo sólo la herencia de las traslaciones.

También añadimos diversas optimizaciones, moviendo datos que no se actualizan cada fotograma a la función `init()`. Al mover la inicialización de la matriz de proyección a la función `initProjection()` (que es llamada en `init()`) nos ahorramos tener que recalcularla en cada fotograma.

Además, indicamos a GLFW que llame a `reshape()` si se produce algún cambio en las dimensiones de la ventana. En `reshape()` actualizamos el tamaño de la ventana gráfica y llamamos a `initProjection()` para recrear la matriz de proyección, ya que esta depende de las dimensiones de la ventana gráfica. De esta manera nos aseguramos de sólo actualizar la matriz de proyección cuando sea necesario.
