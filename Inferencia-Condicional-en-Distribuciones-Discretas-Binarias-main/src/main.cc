#include "interface.h"

int main(int argc, char* argv[]) {
  try {
    interface program_manager;
    program_manager.launch();
  } catch (std::exception& e) {
    std::cerr << "Se ha producido un error en el sistema." << std::endl;
  }
  return 0;
}