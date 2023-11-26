#include <iostream>
#include <libteddy/core.hpp>

int main() {
    teddy::bdd_manager manager(3, 1'000);

    std::string const& cesta = "PLA-Files/simplePLA.pla";
    auto plaSubor = teddy::pla_file::load_file(cesta);
    teddy::bdd_manager::diagram_t f;
    f = manager.from_pla(*plaSubor, teddy::fold_type::Left)[0];

    manager.to_dot_graph(std::cout, f);
    std::ofstream ofst("f.dot");
    manager.to_dot_graph(ofst, f);

    long long sc = manager.satisfy_count(1, f);
    std::cout << sc << std::endl;
    
    std::vector<std::array<int, 3>> sa
        = manager.satisfy_all<std::array<int, 3>>(1, f);

    for (std::array<int, 3> kombinacia : sa)
    {
        for (int i : kombinacia) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

	return 0;
}