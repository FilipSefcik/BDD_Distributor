#include "merger.h"
#include "pla_function.h"

void merger::merge_pla() {
    std::string path = "Merging/big_pla1.pla";

    //testing file start

    std::filesystem::path dir_path = std::filesystem::path(path).parent_path();

    if (!std::filesystem::exists(dir_path)) {
        if (!std::filesystem::create_directories(dir_path)) {
            std::cerr << "Error creating directories!" << std::endl;
            exit(4);
            return; // Return an error code
        }
    }

    std::ofstream output_file(path);

    if (!output_file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(4);
        return; 
    }

    //testing file end

    //sorting modules
    std::sort(this->modules->begin(), this->modules->end(), [](module* a, module* b) { return a->get_priority() > b->get_priority(); });

    for (int i = 0; i < this->modules->size(); i++) {
        module* mod = this->modules->at(i);
        auto input_file = std::ifstream(mod->get_path());

        if (not input_file.is_open())
        {
            throw std::runtime_error("Error opening " + mod->get_name() + " PLA file");
            return;
        }

        std::string line;

        output_file << mod->get_name() << "\n";
        int var_count;
        pla_function* mod_fun = mod->get_pla_function();

        while (std::getline(input_file, line))
        {   
            char first = line[0];
            if (first == '.' || first == '#' || line.empty()) {
                if (line[1] == 'i') {
                    var_count = line[3] - '0';
                }
                continue;
            }

            std::string variables = line.substr(0, var_count);
            int value = std::stoi(line.substr(var_count + 1));

            mod_fun->add_line(variables, value);


            output_file << line << "\n";






        }

        std::cout << mod->get_name() << std::endl;
        mod_fun->print_function();

    }



    output_file.close();


}