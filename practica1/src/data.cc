/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial Avanzada
 *
 * @author Andrés David Riera Rivera, Alejandro Feo martín e Isaac Domínguez Fajardo
 * @since Feb 04 2026
 * @brief Implementación de la clase Data.
 */

#include "data.h"
#include <bitset>

/**
 * @brief Carga una distribución desde un archivo CSV.
 * @param ruta_archivo Ruta del archivo.
 */
void Data::cargarArchivo(const std::string& ruta_archivo) {
  std::ifstream ifs;
  ifs.open(ruta_archivo, std::ios_base::in);
  if (!ifs.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return;
  }
  int line_count{0};
  std::string linea;
  while (std::getline(ifs, linea)) {
    std::string binary = "";
    std::string prob_str = "";

    int counter{0};
    while (linea[counter] != ',') {
        binary += linea[counter];
        ++counter;
    }
    ++counter;
    while (counter < linea.size()) {
        prob_str += linea[counter];
        ++counter;
    }
    ++line_count;
    if (line_count == 1) {
      int n = binary.size();
      variable_count = n;
      eraseChangeSize(variable_count);
    }
    probability(std::stol(binary)) = std::stod(prob_str);
  }
}

/**
 * @brief Redimensiona el vector de probabilidades, borrando sus elementos.
 * @param amount_variables Número de variables.
 */
void Data::eraseChangeSize(int amount_variables) {
  probability_distribution.clear();
  probability_distribution.resize(pow(2, amount_variables));
}

/**
 * @brief Convierte un número binario a decimal.
 * @param binary Número binario representado como entero.
 * @return valor decimal equivalente.
 */
int binaryToDecimal(int binary_literal) {
  int number = binary_literal;
  int decimal_value = 0;
  int base = 1;
  int temporal = number;
  while (temporal) {
    int last_digit = temporal % 10;
    temporal = temporal / 10;
    decimal_value += last_digit * base;
    base = base * 2;
  }
  return decimal_value;
}

/**
 * @brief Cambia el valor del número de variables de nuestra distribución 
 * @param n Nuevo valor
 */
void Data::setVariableCount(int n) {
  variable_count = n;
}

/**
 * @brief Genera una distribución conjunta aleatoria 
 * @param n Número de variables
 */
void Data::generateRandomDistribution(int n) {
  setVariableCount(n);
  eraseChangeSize(n);
  std::vector<double> random_double;
  double total_probabilidades = 0.0;
  for (int i = 0; i < pow(2, n); ++i) {
    double number = static_cast<double>(std::rand() % 100);
    total_probabilidades += number;
    random_double.push_back(number);
  }
  for (double& rnd : random_double) {
    rnd = rnd / total_probabilidades;
  }

  probability_distribution = random_double;
}

/**
 * @brief Imprime en pantalla la distribución conjunta, el vector con sus componentes. 
 */ 
void Data::printDistribution() {
  std::cout << std::endl << "- Distribución conjunta actual - " << std::endl;
  for (int i = 0; i < probability_distribution.size(); ++i) {
    std::cout << "p[" << i << "] : " << std::fixed << std::setprecision(10) 
              << probability_distribution[i] << std::endl;
  }
}

int boolVectorToBinary(std::vector<bool> boolean_vector) {
  std::string binaryLiteralString{""};
  for (bool is_activated : boolean_vector) {
    binaryLiteralString += is_activated ? '1' : '0';
  }
  return binaryToDecimal(std::stoi(binaryLiteralString));
}

/**
 * @brief Calcula la distribución condicional normalizada.
 * @param maskC máscara de variables condicionadas
 * @param valC valores de las variables condicionadas
 * @param maskI máscara de variables de interés
 * @return vector con la distribución condicional normalizada.
 */
std::vector<double> Data::prob_cond_bin(int maskC, int valC, int maskI) {
  int N = variable_count;
  
  // variables de interés.
  int numero_interes = 0;
  for (int i = 0; i < N; i++) {
    if ((maskI >> i) & 1) { // en la posición i hay un 1, es de interés
      numero_interes++;
    }
  }
  
  int tamanoSalida = pow(2, numero_interes);
  std::vector<double> resultado(tamanoSalida, 0.0);
  double total_probabilidades = 0.0;
 
  // todas las combinaciones posibles
  int total = pow(2, N);
  for (int k = 0; k < total; k++) { // combinacion k, ej 10010  
    if ((k & maskC) != valC) {
      continue; // si la combinación actual k NO tiene los valores de las variables condicionadas, se descarta
    }
    int indiceParaIntroducir = 0;
    int contadorInteres = 0;
    // Para cada combinación, calculamos el índice correpondiente.
    for (int i = 0; i < N; i++) {
      if ((maskI >> i) & 1) { // si i es una variable de interés
        if ((k >> i) & 1) { // si el valor de i es 1
          indiceParaIntroducir += pow(2, contadorInteres); // le buscamos un índice acorde a cuantas de interes para insertarlo
        }
        contadorInteres++; // incrementamos el contador de variables de interés para calcular el índice correctamente
      }
    }
    // sumamos la probabilidad correspondiente a esa combinación al resultado, marginalizando.
    resultado[indiceParaIntroducir] += probability_distribution[k];
    total_probabilidades += probability_distribution[k];
  }
  // normalizamos el resultado
  if (total_probabilidades > 0.0) {
    for (int i = 0; i < tamanoSalida; i++) {
      resultado[i] /= total_probabilidades;
    }
  }

  return resultado;
}

