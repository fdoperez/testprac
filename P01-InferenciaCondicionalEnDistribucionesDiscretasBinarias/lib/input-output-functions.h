/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * 
 * @author Adrián Pérez Poleo
 * @author Francisco Gabriel Ruiz Ruiz
 * @brief Header file containing declarations for input-output operations.
 * @bug There are no known bugs.
 */

#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include <iostream>
#include <vector>

void AskForConditionalVariables(std::vector<bool>&, std::vector<int>&);
void AskForInterestVariables(std::vector<bool>&);
std::vector<double> ReadFile(const std::string&);
void PrintDistributionProbabilities(const std::vector<double>&,
                                    const std::vector<bool>&,
                                    const std::vector<bool>&,
                                    const std::vector<int>&);
void PrintJointTable(const std::vector<double>& probabilities, int num_of_variables);

#endif