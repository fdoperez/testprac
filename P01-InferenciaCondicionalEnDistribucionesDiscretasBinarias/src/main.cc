/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * 
 * @author Adrián Pérez Poleo
 * @author Francisco Gabriel Ruiz Ruiz
 * @brief Main entry point for the discrete distribution inference program.
 * Handles data loading, variable selection, and joint distribution display.
 * @bug There are no known bugs.
 */

#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>

#include "../lib/input-output-functions.h"
#include "../lib/computation-algorithms.h"

int main(int argc, char* argv[]) {
  std::vector<double> indexed_probabilities;
  int number_of_variables = 0;
  if (argc == 2) {
    std::string file_name{argv[1]};
    indexed_probabilities = ReadFile(file_name);
    number_of_variables = log2((int)indexed_probabilities.size());
  } else {
    std::cout << "Introduzca el número de variables: ";
    std::cin >> number_of_variables;
    if (number_of_variables <= 0) {
      std::cerr << "Error: se esperaba un número entero mayor que 0." << '\n';
      return 1;
    }
    int seed = 0;
    indexed_probabilities = CreateRandomProbabilities(number_of_variables, seed);
  }

  std::vector<bool> maskC(number_of_variables, false);
  std::vector<int> valC(number_of_variables, 0);
  AskForConditionalVariables(maskC, valC);
  std::vector<bool> maskI(number_of_variables, false);
  AskForInterestVariables(maskI);
  std::vector<bool> maskM(number_of_variables, false);
  DetermineMarginalizationVariables(maskM, maskC, maskI);
  // Measure the computation of the probabilities distribution
  std::vector<double> distribution_probabilities;
  auto start = std::chrono::high_resolution_clock::now();
  distribution_probabilities = ComputeConditionalBinaryProbability(maskI, maskC, valC, indexed_probabilities);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> execution_time = end - start;
  // Print final results
  std::cout << "\n\n============================================================"
            << "\n\nRESULTADOS:\n\n"
            << "> Tabla de distribución conjunta:\n";
  PrintJointTable(indexed_probabilities, number_of_variables);
  std::cout << "\n> Tabla de distribución condicional normalizada:\n";;
  PrintDistributionProbabilities(distribution_probabilities, maskI, maskC, valC);
  std::cout << "\n> Tiempo de ejecución: " << execution_time.count() << " ms\n";
}