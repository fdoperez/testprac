/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * 
 * @author Adrián Pérez Poleo
 * @author Francisco Gabriel Ruiz Ruiz
 * @brief Implementation of functions for conditional inference calculation 
 * in discrete binary distributions using variable marginalization.
 * @bug There are no known bugs.
 */

#include <cmath>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>
#include <utility>

#include "../lib/input-output-functions.h"
#include "../lib/computation-algorithms.h"

/**
 * @brief Converts a binary string mask into its decimal integer value.
 * @param read_base String of 0s and 1s representing the configuration.
 * @return int Corresponding index k in the probability array.
 */
int ToBase(const std::string& read_base) {
  int result = 0;
  for (int i{(int)read_base.size() - 1}; i >= 0; --i) {
    if (read_base[i] == '1') {
      result += pow(2, (read_base.size() - 1 - i));
    }
  }
  return result;
}

/**
 * @brief Converts a decimal number into its binary string representation.
 * @param number Decimal value to convert.
 * @param padding Desired string length (number of variables N).
 * @return std::string Binary string with leading zeros if necessary.
 */
std::string ToBinary(int number, int padding) {
  std::string binary = "";
  while (number > 0) {
    if (number % 2 == 0) {
      binary = '0' + binary;
    } else {
      binary = '1' + binary;
    }
    number /= 2;
  }
  while ((int)binary.size() < padding) {
    binary = '0' + binary;
  }
  return binary;
}

/**
 * @brief Normalizes a vector so the sum of all elements equals 1.0.
 * @param vector Vector containing positive probability values.
 * @return std::vector<double> Normalized vector where Σ P(x) = 1.
 */
std::vector<double> NormalizeVector(const std::vector<double>& vector) {
  double sum = 0.0;
  for (double value : vector) {
    sum += value;
  }
  std::vector<double> normalized_vector(vector.size());
  double check_normalization = 0.0;
  for (int i{0}; i < (int)normalized_vector.size(); ++i) {
    normalized_vector[i] = vector[i] / sum;
    check_normalization += vector[i] / sum;
  }
  return normalized_vector;
}

/**
 * @brief Generates a random normalized joint distribution.
 * @param number Exponent of base 2 (number of variables N).
 * @param seed Optional seed for the random generator (0 for random_device).
 * @return std::vector<double> Normalized vector of size 2^N.
 */
std::vector<double> CreateRandomProbabilities(int number, int seed) {
  unsigned int seed_value;
  if (seed == 0) {
    std::random_device rd;
    seed_value = rd();
  } else {
    seed_value = static_cast<unsigned int>(seed);
  }
  std::mt19937 gen(seed_value); 
  std::uniform_int_distribution<int> dis(0, 9);

  int number_of_variables = pow(2, number);
  std::vector<double> probabilities(number_of_variables);
  for(int i = 0; i < number_of_variables; ++i) {
    probabilities[i] = (double)dis(gen);
  }
  probabilities = NormalizeVector(probabilities);
  return probabilities;
}

/**
 * @brief Identifies variables to be marginalized.
 * Logic: A variable is marginalized if it is neither an interest variable 
 * nor a conditional variable (XM = Ω \ {XI ∪ XC}).
 * @param[out] maskM Boolean vector to be filled, marking variables to
 *                   marginalize.
 * @param[in] maskC Boolean vector marking conditioned variables.
 * @param[in] maskI Boolean vector marking interest variables.
 */
void DetermineMarginalizationVariables(std::vector<bool>& maskM,
                                  const std::vector<bool>& maskC,
                                  const std::vector<bool>& maskI) {
  for (int i{0}; i < (int)maskM.size(); ++i) {
    if (!maskC[i] && !maskI[i]) {
      maskM[i] = true;
    }
  }
}

/**
 * @brief Performs conditional inference via variable
 * elimination/marginalization. Calculates P(XI | XC = valC) by summing out
 * irrelevant variables and  filtering configurations that do not match the
 * evidence valC.
 * @param maskI Mask of interest variables.
 * @param maskC Mask of conditional variables.
 * @param valC Specific values (0/1) for the conditioned variables.
 * @param indexed_probabilities The full joint distribution table.
 * @return std::vector<double> Normalized distribution of the interest
 *         variables.
 */
std::vector<double> ComputeConditionalBinaryProbability(const std::vector<bool>& maskI,
                                                        const std::vector<bool>& maskC,
                                                        const std::vector<int>& valC,
                                                        const std::vector<double>& indexed_probabilities) {
    
  int number_of_variables = maskI.size();
  std::vector<int> interest_indices;
  // Guardamos los índices de las variables de interés (de X3 a X1)
  for(int i = 0; i < number_of_variables; ++i) {
      if(maskI[i]) {
        interest_indices.push_back(i);
      }
  }

  int num_combinations = 1 << interest_indices.size();
  std::vector<double> joint_results(num_combinations, 0.0);

  for (int i = 0; i < (int)indexed_probabilities.size(); ++i) {
    // Obtenemos la representación binaria de la fila actual
    std::string binary = ToBinary(i, number_of_variables);
    bool matches_condition = true;

    // Verify that the conditional variable is equal to the expected, if not jumps to next position
    for (int j = 0; j < number_of_variables; ++j) {
      if (maskC[j] && (binary[j] - '0' != valC[j])) {
        matches_condition = false;
        break;
      }
    }

    if (matches_condition) {
      // Build the position in which we will add the joint table probability (i)
      std::string distribution_probabilities_index_string = "";
      for (int j{(int)binary.size() - 1}; j >= 0; --j) {
        bool is_position_found = false;
        for (const auto& interest_variable_index : interest_indices) {
          if (j == interest_variable_index) {
            is_position_found = true;
          }
        }
        if (is_position_found) {
          distribution_probabilities_index_string = binary[j] + distribution_probabilities_index_string;
        }
      }
      // Sum to the position of the resulting vector
      int distribution_probabilities_index = ToBase(distribution_probabilities_index_string);
      joint_results[distribution_probabilities_index] += indexed_probabilities[i];
    }
  }
  return NormalizeVector(joint_results);
}