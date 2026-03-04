#ifndef BINARY_DISTRIBUTION_H
#define BINARY_DISTRIBUTION_H

// I/O
#include <iostream>
#include <iomanip>
#include <fstream>

// Tipos de datos
#include <string>
#include <vector>
#include <cstdint>

// Matemáticas
#include <cmath>
#include <random>

// Manejo de excepciones
#include <exception>

namespace IAA {
  
  // Constante
  constexpr double EPSILON = 0.01;
  
  using mask_t = unsigned;
  using std::size_t;

  /* Distribución Condicional */
  struct ConditionalBinaryDistribution {
    mask_t maskC = 0;
    mask_t valC = 0;
    mask_t maskI = 0;
    std::vector<double> prob = {};
    
    // Método de impresión
    std::ostream& print(std::ostream& os) const;
  };
  
  // Representación de una distribución binaria
  class BinaryDistribution {
   public:
   
    /* Constructor */
    explicit BinaryDistribution() : num_variables_{0}, data_{}{};
   
    /* Carga de la distribución */
    std::istream& load(std::istream& is);
    void gen_rand_bin(unsigned max_size);
    
    /* Getters */
    inline size_t num_variables() const { return num_variables_; }
    
    /* Impresión de la distribución */
    std::ostream& print(std::ostream& os) const;
    
    /* Cálculo de la distribución condicionada */
    void prob_cond_bin(ConditionalBinaryDistribution&) const;
   
   private:
    
    /* Control de validez de la distribución */
    void check_data() const;
    
    /* Atributos */
    size_t num_variables_;       ///< Número de variables
    std::vector<double> data_;   ///< Pares máscara-probabilidad
  };
  
  // Facilita la impresión de las distribuciones
  std::ostream& operator<<(std::ostream&, const BinaryDistribution&);
  std::ostream& operator<<(std::ostream&, const ConditionalBinaryDistribution&);

  // Facilita la lectura de la distribución
  std::istream& operator>>(std::istream&, BinaryDistribution&);

};

#endif // BINARY_DISTRIBUTION_H