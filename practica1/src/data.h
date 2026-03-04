/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 *
 * @author Andrés David Riera Rivera, Alejandro Feo Martín e Isaac Domínguez Fajardo
 * @since Feb 04 2026
 * @brief Definición de la clase Data para el manejo de distribuciones de probabilidad.
 */

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <cmath>
#include <string>
#include <iomanip>

int binaryToDecimal(int binary_literal);
int boolVectorToBinary(std::vector<bool> boolean_vector);

/**
 * @class Data
 * @brief Representa una distribución de probabilidad conjunta sobre variables binarias.
 *        Permite generar distribuciones aleatorias, cargarlas desde archivo y consultarlas.
 */
class Data {
 public:
  Data() {};
  double& probability(int binary_index) { return probability_distribution[binaryToDecimal(binary_index)]; }
  void cargarArchivo(const std::string& ruta_archivo);
  void eraseChangeSize(int amount_variables);

  void setVariableCount(int n);
  int getVariableCount() { return variable_count;};
  const std::vector<double>& getProbs() { return probability_distribution; };
  void printDistribution();
  void generateRandomDistribution(int n);
  std::vector<double> prob_cond_bin(int maskC, int valC, int maskI);
  
  
 private:
  std::vector<double> probability_distribution;
  int variable_count{0};
};

#endif
