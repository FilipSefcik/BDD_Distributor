#include "BDD_Distributor.h"

int main(int argc, char* argv[]) {

    //std::string conf_path = "module_map.conf";
    //int divider_flag = 0;
    //int state = 1;

    std::string conf_path = argc > 1 ? argv[1] : "module_map.conf";
    int divider_flag = argc > 2 ? std::stoi(argv[2]) : 0;
    int state = argc > 3 ? std::stoi(argv[3]) : 1;

    BDD_Distributor distributor;
    distributor.set_conf_path(conf_path);
    distributor.calculate_availability(divider_flag, state);
    return 0;
}