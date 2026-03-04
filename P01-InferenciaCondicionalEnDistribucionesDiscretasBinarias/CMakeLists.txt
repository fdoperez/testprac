cmake_minimum_required(VERSION 3.10)  # Versión mínima requerida de CMake

# Nombre del proyecto
project("Practica 01: Inferencia Condicional")

# Establecer la versión mínima del compilador
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadir los directorios de encabezados
include_directories(lib)

# Definir los archivos fuente
set(SOURCES     
  src/computation-algorithms.cc
  src/input-output-functions.cc
  src/main.cc
)

# Crear el ejecutable
add_executable(p01_inferencia_condicional ${SOURCES})
