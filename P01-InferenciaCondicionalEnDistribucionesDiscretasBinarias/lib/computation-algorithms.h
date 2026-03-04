/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * 
 * @author Adrián Pérez Poleo
 * @author Francisco Gabriel Ruiz Ruiz
 * @brief Header file containing declarations for probability distribution
 * operations.
 * @bug There are no known bugs.
 */

#ifndef COMPUTATION_ALGORITHMS_H
#define COMPUTATION_ALGORITHMS_H

#include <iostream>
#include <string>
#include <vector>

int ToBase(const std::string&);
std::string ToBinary(int, int);
std::vector<double> ComputeConditionalBinaryProbability(const std::vector<bool>&,
                                    const std::vector<bool>&,
                                    const std::vector<int>&,
                                    const std::vector<double>&);
std::vector<double> CreateRandomProbabilities(int, int = 0);
void DetermineMarginalizationVariables(std::vector<bool>&,
                                  const std::vector<bool>&,
                                  const std::vector<bool>&);
std::vector<double> NormalizeVector(const std::vector<double>&);

#endif