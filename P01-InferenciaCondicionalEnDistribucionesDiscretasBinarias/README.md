# Propuesta de resolución de la primera parte de la Práctica 01
## Ejercicios realizados durante la sesión del 4 de febrero de 2026

### Funcionamiento
Para poder compilar y ejecutar el programa se deben seguir los siguientes pasos:

1. Crear un directorio `build` y acceder al mismo (`cd build`).
2. Ejecutar el comando `cmake ..` en dicho directorio para que se configure y genere un fichero Makefile.
3. Ejecutar el comando `make` para que se cree el archivo ejecutable.
4. Ejecutar `./p01_inferencia_condicional`:
    - Si se escribe el nombre del fichero posteriormente se abrirá dicho fichero.
    - En otro caso se generarán datos aleatorios.

En el directorio `test` se han facilitado algunos ficheros de prueba para comprobar el correcto
funcionamiento del programa. 

Recuerde introducir la ruta completa: `../test/<nombre_fichero>`

### Ejemplo de ejecución

 `./p01_inferencia_condicional ../test/t1.csv`