#ifndef DIFFERENTIAL_PRIVACY_H
#define DIFFERENTIAL_PRIVACY_H

#include <iostream>
#include <string>

namespace Crypto {

class DifferentialPrivacy {
public:
    struct Dataset {
        std::vector<double> values;
        std::string sensitivity;
    };
    
    struct PrivacyParams {
        double epsilon;
        double delta;
        int num_queries;
    };
    
    DifferentialPrivacy();
    void set_privacy_parameters(double epsilon, double delta);
    double add_laplace_noise(double true_value, double sensitivity);
    void print_privacy_budget();

private:
    PrivacyParams params;
};

} // namespace Crypto

#endif
