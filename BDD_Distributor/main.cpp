#include "bdd_distributor.h"
#include <cstdint>
#include <ostream>

int main(int argc, char* argv[]) {

    std::ofstream output_file("../BDD_Distributor/Modules/Experiments/experiment_map.conf");

    if (!output_file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(4);
        return 0; 
    }

    for (int i = 0; i < 46; i++) {
        output_file << "M" << i << " ../BDD_Distributor/Modules/Experiments/apex1.pla 0\n";
    }

    output_file << "\n";

    for (int i = 0; i < 46; i++) {
        output_file << "M" << i << " ";
        for (int j = 0; j < 46; j++) {
            if (j == 0) {
                    continue;
                }
            if (i == 0) {
                output_file << "M" << j;
            } else {
                output_file << "V";
            }

            if (j == 45) {
                output_file << "\n";
            }
        }
    }

    output_file.close();


    // std::string conf_path = argc > 1 ? argv[1] : "module_map.conf";
    // int divider_flag = argc > 2 ? std::stoi(argv[2]) : 0;
    // int state = argc > 3 ? std::stoi(argv[3]) : 1;

    // bdd_distributor distributor;
    // distributor.set_conf_path(conf_path);
    // distributor.calculate_availability(divider_flag, state);
    return 0;
}