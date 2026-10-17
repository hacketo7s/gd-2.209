// make a def file of libcocos2d.dll with gendef, gendef is the input of the program

#include <fstream>
#include <string>
#include <iostream>

std::string make_line_proxy(std::string line, std::string original_dll_path) {
    if (line.starts_with(";")) return "";

    if (line.starts_with("LIBRARY") or line.starts_with("EXPORT")) {
        return line.append("\n");
    }

    bool ends_with_data = false;

    if (line.ends_with("DATA")) {
        line.erase(line.size() - 5);
        ends_with_data = true;
    }

    std::string double_quoted_line = R"(")" + line.append(R"(")");
    std::string redirection = original_dll_path.append(".").append(double_quoted_line);

    if (ends_with_data)
        redirection.append(" DATA");
        
    return double_quoted_line.append(" = ").append(redirection).append("\n");
}

int main(int argc, char** argv){
    if (argc != 4) {
        std::cout << "Only three arguments";
        return 0;
    }

    std::string def_path = argv[1];
    std::string output_proxy_def_name = argv[2];
    std::string original_dll_path = argv[3];

    if (!output_proxy_def_name.ends_with(".def")) {
        output_proxy_def_name.append(".def");
    }

    std::ifstream def_stream(def_path);
    if (!def_stream.is_open()) {
        std::cout << "Invalid path?";
        return 0;
    }

    std::string proxy;
    std::string line;

    while (std::getline(def_stream, line)) {
        proxy.append(make_line_proxy(line, original_dll_path));
    }

    std::ofstream proxy_def(output_proxy_def_name);
    if (!proxy_def.is_open()) {
        std::cout << "Something went wrong";
        return 0;
    }

    proxy_def << proxy;
    proxy_def.close();

    std::cout << "Success";
    return 0;
}
