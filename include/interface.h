#ifndef INTERFACE_H
#define INTERFACE_H

// Estructuras de datos
#include <set>
#include "binary_distribution.h"

// Funcionalidades
#include <algorithm>

class interface {
 public:
  interface() = default;
  
  // Executes run
  void launch(); 

 private:

  // I/O 
  void print_menu() const; 
  void print_error(const std::string&) const; 
  void print_empty_error() const;
  void clrscr() const;
  void stop() const;   
  
  // Posibles acciones del programa
  void read_dist();
  void gen_dist();
  void print_dist() const; 
  void print_con_dist() const;
  void read_maskC();
  void print_maskC() const;
  void read_maskI();
  void print_maskI() const;
  
  // Atributos
  IAA::BinaryDistribution bin_dist_;
  IAA::ConditionalBinaryDistribution bin_cond_dist_;
};

#endif //  INTERFACE_H