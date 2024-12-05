## Ejercicio 4.2

En el programa 4.1, la matriz de la vista se define en la función `display()` simplemente como el opuesto de la ubicación de la cámara:

```c
vec3 negCameraPos;
glm_vec3_negate_to(cameraPos, negCameraPos);

glm_mat4_identity(vMat);
glm_translate(vMat, negCameraPos);
```

Reemplaza este código con una implementación de la computación mostrada en la figura 3.13. Esto permite posicionar la cámara indicando una posición y tres ejes de orientaciones. Es necesario almacenar los vectores U, V y N descritos en la sección 3.7. Experimenta con diversas posiciones para la cámara y observa la apariencia resultante de los cubos.
