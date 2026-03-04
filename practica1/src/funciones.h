/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 *
 * @author Andrés David Riera Rivera, Alejandro Feo Martín e Isaac Domínguez Fajardo
 * @since Feb 10 2026
 * @brief Implementación de las funciones utilizadas en el código principal
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <ctime>
#include <bitset>

#include "data.h"

/**
 * @brief Interactúa con el usuario para construir la distribución conjunta
 * @param datos
 */
void interfazInicial(Data& datos) {
  std::cout << "Generar (1) o cargar (2) tabla" << std::endl;
  int opcion = 0;
  while (opcion != 1 && opcion != 2) {
    std::cin >> opcion;
    if (opcion == 1) {
      int n_variables;
      std::cout << "Introduzca el número de variables: ";
      std::cin >> n_variables;
      // semilla para el numero aleatorio
      std::srand(std::time(nullptr));
      datos.generateRandomDistribution(n_variables);
    } else if (opcion == 2) {
      std::string ruta_archivo;
      std::cout << "Introduzca la ruta del archivo: ";
      std::cin >> ruta_archivo;
      datos.cargarArchivo(ruta_archivo);
    } else {
      std::cerr << "Opción inválida" << std::endl;
    }
  }
}

/**
 * @brief Interactúa con el usuario para seleccionar las variables condicionadas y definir sus valores
 * @param variables_condicionadas
 * @param valores_condicionadas
 * @param n_variables
 */
void variablesCondicionadas(std::vector<bool>& variables_condicionadas, std::vector<bool>& valores_condicionadas, int n_variables) {
  int opcion = -1;
  while (opcion != 0) {
    int variable;
    std::cout << "Seleccione una variable condicionada: ";
    std::cin >> variable;
    variable = n_variables - variable;
    if (variable >= n_variables || variable < 0) {
      std::cerr << "Error: variable no existente" << std::endl;
      continue;
    } else if (variables_condicionadas[variable]) {
      std::cerr << "Error: variable ya añadida" << std::endl;
      continue;
    }
    variables_condicionadas[variable] = true;
        
    // asignar valor a la variable
    int valor = -1;
    while (valor != 0 && valor != 1) {
      std::cout << "Asigne un valor a la variable (0 o 1): ";
      std::cin >> valor;
      if (valor != 0 && valor != 1) {
        std::cerr << "Error: valor inválido" << std::endl;
      } else {
        valores_condicionadas[variable] = static_cast<bool>(valor);
      }
    }
    
    std::cout << "Se ha añadido la variable X" << n_variables - variable << " como variable condicionada con el valor " << valor << std::endl;

    std::cout << "Quiere añadir otra variable condicionada?" << std::endl << "No (0)." << std::endl << "Sí (1)." << std::endl;
    std::cin >> opcion;
  }
}

/**
 * @brief Interactúa con el usuario para seleccionar las variables de interés
 * @param variables_condicionadas
 * @param valores_condicionadas
 * @param n_variables
 */
void variablesDeInteres(std::vector<bool>& variables_interes, int n_variables, std::vector<bool>& variables_condicionadas) {
  int opcion = -1;
  while (opcion != 0) {
    int variable;
    std::cout << "Seleccione una variable de interés: ";
    std::cin >> variable;
    variable = n_variables - variable;
    if (variable >= n_variables || variable < 0) {
      std::cout << "Error: variable no existente" << std::endl;
      continue;
    } else if (variables_condicionadas[variable]) {
      std::cerr << "Error: variable ya añadida como condicionada" << std::endl;
      continue;
    } else if (variables_interes[variable]) {
      std::cerr << "Error: variable ya añadida" << std::endl;
      continue;
    }
    variables_interes[variable] = true;

    std::cout << "Se ha añadido la variable X" << n_variables - variable << " como variable de interés" << std::endl;
    
    std::cout << "Quiere añadir otra variable de interés?" << std::endl << "No (0)." << std::endl << "Sí (1)." << std::endl;
    std::cin >> opcion;
  }
}

/**
 * @brief Imprime en pantalla un número en binario
 * @param numero
 * @param digitos
 */
void imprimirBinario(int numero, int digitos) {
  for (int i = digitos - 1; i >= 0; i--) {
    // Extraemos el bit en la posición i
    int bit = (numero >> i) & 1;
    std::cout << bit;

    // Si no es el último bit, imprimimos dos espacios
    if (i > 0) {
      std::cout << "  ";
    }
  }
}

#endif

