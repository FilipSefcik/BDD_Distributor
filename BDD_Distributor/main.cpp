#include "bdd_distributor.h"

int main(int argc, char* argv[]) {
    std::string conf_path = argc > 1 ? argv[1] : "module_map.conf";
    int divider_flag = argc > 2 ? std::stoi(argv[2]) : 0;
    int state = argc > 3 ? std::stoi(argv[3]) : 1;
    bool timer = false;
    if (argc > 4) {
        std::string yes = "y";
        if (yes == argv[4]) {
            timer = true;
        }
    }

    bdd_distributor distributor;
    distributor.set_conf_path(conf_path);
    distributor.calculate_availability(divider_flag, state, timer);
    if (timer) { distributor.get_max_time(); }
    return 0;
}