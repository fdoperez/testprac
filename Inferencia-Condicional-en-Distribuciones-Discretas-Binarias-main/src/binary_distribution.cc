#include "binary_distribution.h"

namespace IAA {

  std::istream& BinaryDistribution::load(std::istream& is) {
    bool first_iteration = true;
    std::string line;

    while (getline(is, line)) {
      size_t comma_position = line.find(',');
      mask_t key = std::stoi(line.substr(0u, comma_position), nullptr, 2u);
      double value = std::stod(line.substr(comma_position + 1u));

      if (first_iteration) {
        first_iteration = false;
        num_variables_ = comma_position;
        data_.resize(1 << comma_position);
      }

      data_[key] = value;
    }
    check_data();
    return is;
  }

  void BinaryDistribution::gen_rand_bin(mask_t max_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist_size(2, max_size);
    std::uniform_real_distribution<> dist(0.0, 1.0);
    num_variables_ = dist_size(gen);
    size_t size = (1u << num_variables_);
    double sum = 0.0;

    // Limpieza
    data_.clear();
    data_.reserve(size);

    for (size_t i = 0u; i < size; ++i) {
      double value = dist(gen);
      data_.push_back(value);
      sum += value;
    }

    for (double& prob : data_) prob /= sum;
  }

  std::ostream& BinaryDistribution::print(std::ostream& os) const {
    for (size_t i = num_variables_; 1 <= i; --i) os << "X" << i << "   ";
    os << "Probabilidad\n";

    for (mask_t mask = 0; mask < data_.size(); ++mask) {
      for (int bit = (num_variables_ - 1); bit >= 0; --bit) {
          os << ((mask >> bit) & 1) << "    ";
      }
      os << std::fixed << std::setprecision(2) << data_[mask] << "\n";
    }
    return os;
  }

  void BinaryDistribution::check_data() const {
    double sum = 0.0;
    for (auto val : data_) sum += val;
    if (abs(sum - 1.0) > EPSILON) {
      throw std::invalid_argument("Se ha introducido una distribución no válida\n");
    }
  }

  void BinaryDistribution::prob_cond_bin(ConditionalBinaryDistribution& cond_dist) const {
    mask_t maskI = cond_dist.maskI;
    mask_t maskC = cond_dist.maskC;
    mask_t valC = cond_dist.valC;

    mask_t nI = 0;
    for (mask_t i = maskI; i; i >>= 1) {
      if(i & 1) ++nI;
    }

    cond_dist.prob.resize(1 << nI, 0);
    double sum = 0.0;

    for (mask_t mask = 0; mask < data_.size(); ++mask) {
      if ((mask & maskC) != valC) continue;

      mask_t idx = 0;
      size_t slide = 0;
      for (size_t bit = 0; bit < num_variables_; ++bit) {
        if ((maskI & (1 << bit))) {
          if (mask & (1 << bit)) idx = (1 << slide) | idx;
          ++slide;
        }
      }

      cond_dist.prob[idx] += data_[mask];
      sum += data_[mask];
    }

    for(double& v : cond_dist.prob) v /= sum;
  }

  std::ostream& ConditionalBinaryDistribution::print(std::ostream& os) const {
    os << "Condiciones: ";
    size_t idx = 0;
    for (mask_t i = maskC, j = valC; i; i >>= 1, j >>= 1) {
      if(i & 1) os << 'X' << (idx + 1) << '=' << (j & 1) << ' ';
      idx++;
    }
    os << "\n";

    std::vector<size_t> idxs;
    idx = 0;
    for (mask_t i = maskI; i; i >>= 1) {
      if(i & 1) idxs.push_back(idx + 1);
      ++idx;
    }

    for (long int i = idxs.size() - 1; 0 <= i; --i) os << "X" << idxs[i] << "   ";
    os << "Probabilidad\n";

    for (mask_t mask = 0; mask < prob.size(); ++mask) {
      for (int bit = (idxs.size() - 1); bit >= 0; --bit) {
          os << ((mask >> bit) & 1) << "    ";
      }
      os << std::fixed << std::setprecision(2) << prob[mask] << "\n";
    }

    return os;
  }

  std::ostream& operator<<(std::ostream& os, const BinaryDistribution& dist) {
    return dist.print(os);
  }

  std::ostream& operator<<(std::ostream& os, const ConditionalBinaryDistribution& dist) {
    return dist.print(os);
  }

  std::istream& operator>>(std::istream& is, BinaryDistribution& dist) {
    return dist.load(is);
  }
};
