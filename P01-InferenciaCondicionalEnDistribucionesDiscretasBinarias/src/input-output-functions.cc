/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 * 
 * @author Adrián Pérez Poleo
 * @author Francisco Gabriel Ruiz Ruiz
 * @brief Implementation of functions for input output operations.
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
 * @brief Reads the joint distribution from a CSV file.
 * Each bit in the mask represents a variable state.
 * @param input_file_name Name of the source file.
 * @return std::vector<double> Probability array where index k encodes the
 *         configuration.
 */
std::vector<double> ReadFile(const std::string& input_file_name) {
  std::vector<double> results;
  std::string line;
  bool is_first_line = true;
  std::ifstream input_file{input_file_name};
  while (std::getline(input_file, line)) {
    bool found_comma{false};
    int comma_index{0};
    for (int i{0}; !found_comma; ++i) {
      if (line[i] == ',') {
        found_comma = true;
        comma_index = i;
      }
    }
    std::string read_base = "";
    for (int i{0}; i < comma_index; ++i) {
      read_base += line[i];
    }
    int position{ToBase(read_base)};
    std::string probability{""};
    for (int i{comma_index + 1}; i < (int)line.size(); ++i) {
      probability += line[i];
    }
    if (is_first_line) {
      results.resize(pow(2, read_base.size()));
    }
    results[position] = std::stod(probability);
    is_first_line = false;
  }
  input_file.close();
  return results;
}

/**
 * @brief UI to select conditional variables (XC) and their values.
 * @param[out] maskC Boolean vector marking which variables act as conditions.
 * @param[out] valC Vector storing the specific value (0/1) for each
 *                  conditioned variable.
 */
void AskForConditionalVariables(std::vector<bool>& maskC, std::vector<int>& valC) {
  std::cout << "¿Cuántas variables condicionales desea?: ";
  int user_input_conditionals;
  std::cin >> user_input_conditionals;
  for (int i{0}; i < user_input_conditionals; ++i) {
    int desired_variable_index = 0;
    int desired_value = 0;
    bool valid_index = true;
    bool valid_value = true;
    do {
      std::cout << "Introduzca el índice de la variable y valor (ej. 1 1): ";
      std::cin >> desired_variable_index;
      std::cin >> desired_value;
      // Check if index is valid
      if (desired_variable_index > (int)maskC.size() ||
          desired_variable_index <= 0) {
        std::cerr << "Error: ha introducido un índice de variable no válido (se esperaba entre 1 y "
                  << ((int)maskC.size()) << ")\n";
        valid_index = false;
      } else {
        valid_index = true;
      }
      // Check if value is valid
      if (desired_value != 0 && desired_value != 1) {
        std::cerr << "Error: ha introducido un valor no válido (se esperaba 0 o 1)\n";
        valid_value = false;
      } else {
        valid_value = true;
      }
    } while (!valid_index || !valid_value);
    maskC[(int)maskC.size() - desired_variable_index] = true;
    valC[(int)maskC.size() - desired_variable_index] = desired_value;
  }
}

/**
 * @brief UI to select interest variables (XI).
 * @param[out] maskI Boolean vector marking variables for the output
 *                   distribution.
 */
void AskForInterestVariables(std::vector<bool>& maskI) {
  bool valid_interest_variable_amount{false};
  int user_input_interest;
  do {
  std::cout << "¿Cuántas variables de interés desea?: ";
  std::cin >> user_input_interest;
  if (user_input_interest > 0) {
    valid_interest_variable_amount = true;
  } else {
    std::cerr << "Error: se esperaba un número entero mayor que 0." << '\n';
  }
  } while (!valid_interest_variable_amount);
  for (int i{0}; i < user_input_interest; ++i) {
    int desired_variable_index = 0;
    bool valid_index = true;
    do {
      std::cout << "Introduzca el índice: ";
      std::cin >> desired_variable_index;
      // Check if index is valid
      if (desired_variable_index > (int)maskI.size() ||
          desired_variable_index <= 0) {
        std::cerr << "Error: ha introducido un índice no válido (se esperaba entre 1 y "
                  << ((int)maskI.size()) << ")\n";
        valid_index = false;
      } else {
        valid_index = true;
      }
    } while(!valid_index);
    maskI[(int)maskI.size() - desired_variable_index] = true;
  }
}

/**
 * @brief Prints the resulting conditional distribution in a formatted table.
 * Generates headers in the format "X_i | X_j = val" and displays the 
 * truth table for the variables of interest alongside their probabilities.
 * @param distribution_probabilities The calculated conditional probabilities.
 * @param maskI Mask of interest variables.
 * @param maskC Mask of conditional variables.
 * @param valC Values used for the conditioning.
 */
void PrintDistributionProbabilities(const std::vector<double>& distribution_probabilities,
                                    const std::vector<bool>& maskI,
                                    const std::vector<bool>& maskC,
                                    const std::vector<int>& valC) {
  int number_of_variables = (int)maskI.size();

  // Find conditional variables
  std::vector<int> conditional_variables;
  for (int i{0}; i < number_of_variables; ++i) {
    if (maskC[i]) { // Found a conditional variable
      conditional_variables.emplace_back(number_of_variables - i);
    }
  }

  // Configuration of format in function of the conditional variables
  const int col_width = 12 + ((int)conditional_variables.size() * 4);
  std::cout << std::left;

  // Iterate through every interest variable
  for (int i{0}; i < number_of_variables; ++i) {
    if (maskI[i]) { // Found interest variable
      std::stringstream ss;
      // Print header of column
      // Print interest variable
      ss << "X" << (number_of_variables - i);
      // Print the conditional variables
      for (int i{0}; i < (int)conditional_variables.size(); ++i) {
        if (i == 0) {
          ss << "|";
        }
        ss << "X" << (conditional_variables[i]) << "="
                  << valC[number_of_variables - conditional_variables[i]];
        if (i + 1 != (int)conditional_variables.size()) {
          ss << ",";
        }
      }
      std::cout << std::setw(col_width) << ss.str();
    }
  }
  std::cout << "P\n";
  // Print body
  for (int i{0}; i < (int)distribution_probabilities.size(); ++i) {
    const std::string& binary{ToBinary(i, log2((int)distribution_probabilities.size()))};
    for (const auto& value : binary) {
      std::cout << std::setw(col_width) << value;
    }
    std::cout << distribution_probabilities[i] << '\n';
  }
}

/**
 * @brief Displays the full joint probability distribution table.
 * Prints every possible configuration of the N binary variables and 
 * their corresponding probability from the input vector.
 * @param probabilities Vector containing 2^N probability values.
 * @param num_of_variables Number of variables (N) in the distribution.
 */
void PrintJointTable(const std::vector<double>& probabilities, int num_of_variables) {
  for (int i{0}; i < num_of_variables; ++i) {
    std::cout << "X" << (num_of_variables - i) << '\t';
  }
  std::cout << "Probabilidades\n";
  for (int i{0}; i < (int)probabilities.size(); ++i) {
    std::string binary{ToBinary(i, num_of_variables)};
    for (int j{0}; j < (int)binary.size(); ++j) {
      std::cout << binary[j] << '\t';
    }
    std::cout << probabilities[i] << '\n';
  }
}