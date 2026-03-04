#include "interface.h"
#include "chronometer.h"

void interface::print_maskI() const {
  clrscr();

  // Preparación
  IAA::mask_t maskI = bin_cond_dist_.maskI;

  // Impresión
  std::cout << "Variable\n";
  for (size_t i = 0; i < bin_dist_.num_variables(); ++i) {
    if((1 << i) & maskI) std::cout << "X" << (i + 1) << "\n";
  }
  std::cout << std::endl;

  stop();
  clrscr();
}

void interface::read_maskI() {
  clrscr();

  // Reseteo de la mascara
  bin_cond_dist_.maskI = 0;

  // Preparación
  IAA::mask_t nC = 0;
  IAA::mask_t maskC = bin_cond_dist_.maskC;
  for (IAA::mask_t i = maskC; i; i >>= 1) {
    if(i & 1) ++nC;
  }
  long int count = 0;
  unsigned max_vars = bin_dist_.num_variables() - std::max(1u, nC);


  // Lectura del número de variables de interés
  std::cout << "Número de variables de interes (0.." << max_vars << ") ?  ";
  std::cin >> count;
  while(count < 0 || max_vars < count) {
    std::cerr << "Error: número inválido.\n";
    stop();
    clrscr();
    std::cout << "Número de variables de interes (0.." << max_vars  << ") ?  ";
    std::cin >> count;
  }

  // Lectura de variables de interés
  for (int i = 0; i < count;) {
    std::cout << "Variable (1.." <<  bin_dist_.num_variables() << ") ?  ";
    long int idx;
    std::cin >> idx;

    if(idx < 1 || idx >  bin_dist_.num_variables()) {
      std::cerr << "Error: variable inválida.\n";
      stop();
      clrscr();
      continue;
    }

    IAA::mask_t new_bit = (1 << (idx - 1));

    if (new_bit & maskC) {
      std::cerr << "Error: la variable ya es condicionada.\n";
      stop();
      clrscr();
      continue;
    }

    bin_cond_dist_.maskI |= new_bit;
    ++i;
  }

  stop();
  clrscr();
}

void interface::print_maskC() const {
  clrscr();

  // Preparación
  IAA::mask_t maskC = bin_cond_dist_.maskC;
  IAA::mask_t valC = bin_cond_dist_.valC;

  // Impresión
  std::cout << "Variable  Valor\n";
  for (size_t i = 0; i < bin_dist_.num_variables(); ++i) {
    IAA::mask_t slide = (1u << i);
    if(slide & maskC) {
      std::cout << 'X' << (i + 1) << "        " << (valC & slide) << "\n";
    }
  }
  std::cout << std::endl;

  stop();
  clrscr();
}

void interface::read_maskC() {
  clrscr();

  // Reseteo de las mascaras
  bin_cond_dist_.maskC = 0;
  bin_cond_dist_.valC = 0;

  // Preparación
  IAA::mask_t nI = 0;
  IAA::mask_t maskI = bin_cond_dist_.maskI;
  for (IAA::mask_t i = maskI; i; i >>= 1) {
    if(i & 1) ++nI;
  }
  long int count = 0;
  unsigned max_vars = bin_dist_.num_variables() - std::max(1u, nI);


  // Lectura del número de variables de interés
  std::cout << "Número de variables condicionadas (0.." << max_vars << ") ?  ";
  std::cin >> count;
  while(count < 0 || max_vars < count) {
    std::cerr << "Error: número inválido.\n";
    stop();
    clrscr();
    std::cout << "Número de variables condicionadas (0.." << max_vars << ") ?  ";
    std::cin >> count;
  }

  // Lectura de variables de interés
  for (int i = 0; i < count;) {
    std::cout << "Variable (1.." <<   bin_dist_.num_variables() << ") ?  ";
    long int idx;
    std::cin >> idx;

    if(idx < 1 || idx >  bin_dist_.num_variables()) {
      std::cerr << "Error: variable inválida.\n";
      stop();
      clrscr();
      continue;
    }

    IAA::mask_t new_bit = (1 << (idx - 1));

    if (new_bit & maskI) {
      std::cerr << "Error: la variable ya es interesada.\n";
      stop();
      clrscr();
      continue;
    }

    // Lectura de valor
    int val;
    std::cout << "Valor de la variable X" << idx << " (0|1) ?  ";
    std::cin >> val;
    while(val != 0 && val != 1){
      std::cerr << "Error: valor inválido.\n";
      stop();
      clrscr();
      std::cout << "Valor de la variable X" << idx << " (0|1) ?  ";
      std::cin >> val;
    }

    bin_cond_dist_.maskC |= new_bit;
    if(val) bin_cond_dist_.valC |= new_bit;
    ++i;
  }

  stop();
  clrscr();
}

void interface::print_con_dist() const {
  clrscr();

  IAA::ConditionalBinaryDistribution dummy{bin_cond_dist_};
  Chronometer chrono;
  chrono.start();
  bin_dist_.prob_cond_bin(dummy);
  chrono.stop();
  dummy.print(std::cout);
  std::cout << "\nTiempo: " << chrono.get_time() << " ns\n\n";

  stop();
  clrscr();
}

void interface::print_dist() const {
  clrscr();
  bin_dist_.print(std::cout);
  stop();
  clrscr();
}

void interface::gen_dist() {
  clrscr();

  long int size;
  std::cout << "Tamaño máximo (2..30) ?  ";
  std::cin >> size;
  while (size < 2 || 30 < size) {
    std::cout << "Error: tamaño inválido.\n";
    stop();
    clrscr();
    std::cout << "Tamaño máximo (2..30) ?  ";
    std::cin >> size;
  }

  clrscr();
  std::cout << "Se ha generado la siguiente distribución:\n";
  bin_dist_.gen_rand_bin(size);
  std::cout << bin_dist_;

  stop();
  clrscr();
}

void interface::read_dist() {
  clrscr();

  std::string filename;
  std::cout << "Ruta del archivo `.csv` (-1 para abortar) ?  ";
  std::cin >> filename;
  bool abort = false;
  std::ifstream ifs{filename};

  while (!ifs && !abort) {
    if(filename == "-1") {
      abort = true;
      break;
    }
    std::cout << "Error: no se pudo abrir el archivo.\n";
    stop();
    clrscr();
    std::cout << "Ruta del archivo `.csv` (-1 para abortar) ?  ";
    std::cin >> filename;
  }

  if (abort) return;

  ifs >> bin_dist_;

  stop();
  clrscr();
}

void interface::stop() const {
  std::cout << "Pulsa enter para continuar . . . ";
  std::cin.ignore();
  std::cin.get();
}

void interface::clrscr() const {
 #ifdef _WIN32
    std::system("cls");
  #else
    std::system("clear");
  #endif
}

void interface::print_empty_error() const {
  clrscr();
  std::cout << "Error: no hay ninguna distribución cargada.\n\n";
  std::cout << "Introduzca `1` o `2` para cargar una distribución.\n";
  stop();
  clrscr();
}

void interface::print_error(const std::string& entry) const {
  clrscr();
  std::cerr << "Opción `" << entry << "` desconocida." << std::endl;
  stop();
  clrscr();
}

void interface::print_menu() const {
  clrscr();
  std::cout << "===== IAA: INFERENCIA CONDICIONAL =====\n";
  std::cout << "1. Cargar la distribución desde un fichero .csv\n";
  std::cout << "2. Generar la distribución de forma aleatoria\n";
  std::cout << "3. Mostrar distribución\n";
  std::cout << "4. Fijar variables condicionadas\n";
  std::cout << "5. Fijar variables de interés\n";
  std::cout << "6. Mostrar variables condicionales\n";
  std::cout << "7. Mostrar variables de interés\n";
  std::cout << "8. Mostrar tabla de probabilidad condicionada\n";
  std::cout << "0. Salir\n";
}

void interface::launch() {
  std::string option;
  do {
    print_menu();
    std::cout << "\n>>> ";
    std::cin >> option;
    switch (std::stoi(option)) {
      case 1:
        read_dist();
        break;

      case 2:
        gen_dist();
        break;

      case 3:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else print_dist();
        break;

      case 4:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else read_maskC();
        break;

      case 5:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else read_maskI();
        break;

      case 6:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else print_maskC();
        break;

      case 7:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else print_maskI();
        break;

      case 8:
        if (bin_dist_.num_variables() == 0) print_empty_error();
        else print_con_dist();
        break;

      case 0:
        std::cout << "Saliendo...\n";
        break;

      default:
        print_error(option);
    }
  } while(option != "0");
  stop();
  clrscr();
}
