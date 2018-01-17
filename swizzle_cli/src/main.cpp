
#include <swizzle/lexer/Tokenizer.hpp>
#include <swizzle/parser/Parser.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/utility/string_view.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace swizzle {

    struct Config
    {
        Config()
            : description("Options")
        {
        }
        
        std::string backend;
        boost::filesystem::path file;
        
        boost::program_options::options_description description;
        boost::program_options::variables_map vars;
    };
    
    Config parseConfig(const int argc, char const * const argv[])
    {
        namespace po = boost::program_options;
        Config config;
        
        config.description.add_options()
            ("help", "produce this message")
            ("list-backends", "list backends for processing AST")
            ("backend", po::value<std::string>(&config.backend)->default_value("print"), "backend for processing AST")
            ("path", po::value<boost::filesystem::path>(&config.file), "input filename");
        
        po::positional_options_description positionalArguments;
        positionalArguments.add("path", 1);
        
        po::store(po::command_line_parser(argc, argv).options(config.description).positional(positionalArguments).run(), config.vars);
        po::notify(config.vars);
        
        return config;
    }

    // TODO: memory map the file and put it into a wield message
    std::string load_file(const std::string& name)
    {
        std::ifstream file(name.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        std::ifstream::pos_type fileSize = file.tellg();
        
        if(fileSize < 0) return std::string();
        file.seekg(0, std::ios::beg);
        
        std::vector<char> bytes(fileSize);
        file.read(bytes.data(), fileSize);
        
        return std::string(bytes.data(), fileSize);
    }
}

using namespace swizzle;

int main(const int argc, char const * const argv[])
{
    Config config = parseConfig(argc, argv);
    if((argc < 2) || (config.vars.count("help")) || (config.file.string().empty()))
    {
        std::cout << config.description << std::endl;
        return 0;
    }
    
    if(config.vars.count("list-backends"))
    {
        // TODO: implement
        return 0;
    }
    
    std::cout << "backend: " << config.backend << ", file: " << config.file.string() << std::endl;
    return 0;
}
