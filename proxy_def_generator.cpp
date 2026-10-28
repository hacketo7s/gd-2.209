// make a def file into a proxy SPECIFICALLY for mingw gcc 

#include <fstream>
#include <string>
#include <iostream>

#define double_quote(str) R"(")" + str + R"(")"

std::string make_line_proxy(std::string& line, std::string& original_dll_path) {
    bool ends_with_data = false;

    std::string temp;
    std::string redirection;

    if (line.starts_with(';'))
        return "";

    if (line.starts_with("LIBRARY") or line.starts_with("EXPORT")) {
        return line += '\n';
    }

    if (line.ends_with("DATA")) {
        line.erase(line.size() - 5);
        ends_with_data = true;
    }
    
    for (const char c: line) {
        if (c == ';')
            break;
        if (std::isspace(c))
            continue;
        else
            temp.push_back(c);
    }

    line = temp;

    redirection = double_quote(original_dll_path + '.' + line);

    if (ends_with_data)
        redirection += " DATA";
        
    return double_quote(line) + " = " + redirection + '\n';
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
        output_proxy_def_name += ".def";
    }

    std::ifstream def_stream(def_path);
    if (!def_stream.is_open()) {
        std::cout << "Invalid path?";
        return 0;
    }

    std::string proxy;
    std::string line;

    while (std::getline(def_stream, line)) {
        proxy += make_line_proxy(line, original_dll_path);
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
