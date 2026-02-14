#include "differential_privacy.h"
#include <random>
#include <cmath>

namespace Crypto {

DifferentialPrivacy::DifferentialPrivacy() {}

void DifferentialPrivacy::set_privacy_parameters(double epsilon, double delta) {
    params.epsilon = epsilon;
    params.delta = delta;
    params.num_queries = 0;
    
    std::cout << "\n=== Differential Privacy Config ===" << std::endl;
    std::cout << "Epsilon (ε): " << epsilon << std::endl;
    std::cout << "Delta (δ): " << delta << std::endl;
}

double DifferentialPrivacy::add_laplace_noise(double true_value, double sensitivity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> dist(1.0 / (sensitivity / params.epsilon));
    
    double noise = dist(gen);
    if (rand() % 2) noise = -noise;
    
    params.num_queries++;
    
    return true_value + noise;
}

void DifferentialPrivacy::print_privacy_budget() {
    std::cout << "\n=== Privacy Budget Status ===" << std::endl;
    std::cout << "Queries Used: " << params.num_queries << std::endl;
    std::cout << "Remaining Epsilon: " << (10.0 - params.num_queries * 0.1) << std::endl;
    
    if (params.num_queries > 100) {
        std::cout << "[!] WARNING: Privacy budget nearly exhausted!" << std::endl;
    }
}

} // namespace Crypto
