
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
    
    Config parse_config(const int argc, char const * const argv[])
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
        
        if(argc < 2)
        {
            std::cerr << config.description << std::endl;
            throw std::runtime_error("Exception: too few command line arguments");
        }
        
        return config;
    }

    bool validate_file(const boost::filesystem::path& file)
    {
        return boost::filesystem::exists(file) && !boost::filesystem::is_directory(file);
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
    
    struct CreateTokenCallback
    {
        CreateTokenCallback(std::deque<lexer::TokenInfo>& tokens)
            : tokens_(tokens)
        {
        }

        void operator()(const lexer::TokenInfo& token)
        {
            tokens_.push_back(token);
        }

    private:
        std::deque<lexer::TokenInfo>& tokens_;
    };
    
    template<typename Callback>
    void tokenize(lexer::Tokenizer<Callback>& tokenizer, const boost::string_view& sv)
    {
        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();
    }

    void parse(parser::Parser& parser, const std::deque<lexer::TokenInfo>& tokens)
    {
        for(const auto token : tokens)
        {
            parser.consume(token);
        }

        parser.finalize();
    }
    
    int validate_config(const Config& config)
    {
        if(config.vars.count("help"))
        {
            std::cout << config.description << std::endl;
            return 0;
        }
        
        if(config.file.string().empty())
        {
            std::cout << config.description << std::endl;
            return -1;
        }
        
        if(config.vars.count("list-backends"))
        {
            // TODO: implement
            return 0;
        }
       
        if(!validate_file(config.file))
        {
            std::cerr << config.file << " does not exist or is a directory" << std::endl;
            return -1;
        }
        
        return 0;
    }
    
    void process(const Config& config)
    {
        std::deque<lexer::TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);
        
        lexer::Tokenizer<CreateTokenCallback> tokenizer = lexer::Tokenizer<CreateTokenCallback>(config.file.string(), callback);
        parser::Parser parser;

        std::string file = load_file(config.file.string());
        boost::string_view sv = boost::string_view(file);
        
        tokenize(tokenizer, sv);
        parse(parser, tokens);
        
        // TODO: pass AST to backend
        // parser.ast().root()
    }
}

using namespace swizzle;

int main(const int argc, char const * const argv[])
{
    try
    {
        const Config config = parse_config(argc, argv);
        
        const int status = validate_config(config);
        if(status != 0) return status;
        
        process(config);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "FATAL ERROR, exiting" << std::endl;
        return -1;
    }
    
    return 0;
}
