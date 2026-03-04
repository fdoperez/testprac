/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 *
 * @author Andrés David Riera Rivera, Alejandro Feo Martín e Isaac Domínguez Fajardo
 * @since Feb 04 2026
 * @brief Programa principal para el cálculo de probabilidades condicionadas.
 *        Por ahora, genera o carga una distribución conjunta y permite seleccionar
 *        variables condicionadas y variables de interés de forma intutiva.
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <ctime>
#include <bitset>


#include "data.h"
#include "funciones.h"


/**
 * @brief Punto de entrada del programa.
 *        Permite generar o cargar una distribución de probabilidad,
 *        seleccionar variables condicionadas y de interés, e identificar
 *        las variables a marginalizar.
 * @return int: 0 si todo va bien, 1 si hay error.
 */
int main() {
  Data datos;
  // interfaz inicial
  interfazInicial(datos);

  // Imprimir vector de distribución de probabilidad
  datos.printDistribution();
  
  // selección variables condicionadas
  int n_variables{datos.getVariableCount()};
  std::cout << "Número de variables: " << n_variables << std::endl;
  std::vector<bool> variables_condicionadas(n_variables, false);
  std::vector<bool> valores_condicionadas(n_variables, false);
  variablesCondicionadas(variables_condicionadas, valores_condicionadas, n_variables);

  // imprimir variables condicionadas
  int n_condicionadas = 0;
  std::cout << "Variables condicionadas (valor): ";
  for (int i = 0; i < variables_condicionadas.size(); i++) {
    if(variables_condicionadas[i]) {
      std::cout << "X" << n_variables - i << " (";
      if (valores_condicionadas[i]) {
        std::cout << "1) ";
      } else {
        std::cout << "0) ";
      }
      n_condicionadas++;
    }
  }
  std::cout << std::endl;

  // selección variables de interés
  std::vector<bool> variables_interes(n_variables, false);
  variablesDeInteres(variables_interes, n_variables, variables_condicionadas);

  // imprimir variables de interés
  std::cout << "Variables de interés: ";
  for (int i = 0; i < variables_interes.size(); i++) {
    if(variables_interes[i]) {
      std::cout << "X" << n_variables - i << " ";
    }
  }
  std::cout << std::endl;

  // Resto de variables
  std::vector<bool> variables_a_marginalizar(n_variables, false);
  for (int i = 0; i < n_variables; i++) {
    if (variables_condicionadas[i] != true && variables_interes[i] != true) {
      variables_a_marginalizar[i] = true;
    }
  }
  std::cout << "Variables a marginalizar: ";
  for (int i = 0; i < variables_a_marginalizar.size(); i++) {
    if(variables_a_marginalizar[i]) {
      std::cout << "X" << n_variables - i << " ";
    }
  }
  std::cout << std::endl;
  
  int variablesCond = boolVectorToBinary(variables_condicionadas);
  int valoresCond = boolVectorToBinary(valores_condicionadas);
  int variablesInteres = boolVectorToBinary(variables_interes);
  
  clock_t inicio = clock();
  std::vector<double> probabilidades_condicionadas = datos.prob_cond_bin(variablesCond,
                                                                          valoresCond,
                                                                         variablesInteres);
  clock_t fin = clock();
  double tiempo_segundos = double(fin - inicio) / CLOCKS_PER_SEC;
                                              
  
  // imprimir las probabilidades condicionadas                                                                       
  // cabecera
  int n_interes = 0;
  for (int i = 0; i < variables_interes.size(); ++i){
    if(variables_interes[i]) {
      std::cout << "X" << n_variables - i << " ";
      n_interes++;
    }
  }
  std::cout << "probabilidad" << std::endl;
  // valores tabla
  for (int i = 0; i < probabilidades_condicionadas.size(); ++i) {
    imprimirBinario(i, n_interes);
    std::cout << "  " << probabilidades_condicionadas[i] << std::endl;
  }

  // Datos para informe
  std::cout << "Número de variables: " << n_variables << std::endl;
  std::cout << "Número de variables de interés: " << n_interes << std::endl;
  std::cout << "Número de variables condicionadas: " << n_condicionadas << std::endl;
  std::cout << std::fixed << std::setprecision(6);
  std::cout << "Tiempo de ejecución de la función prob_cond_bin: " << tiempo_segundos  << " segundos" << std::endl;

  return 0;
}