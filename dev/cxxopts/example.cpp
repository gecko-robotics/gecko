#include <iostream>
#include <stdio.h>
#include <cxxopts.hpp>

using namespace std;

cxxopts::ParseResult parse(int argc, char* argv[])
{
  try
  {
    cxxopts::Options options(argv[0], "geckocore runs the gecko hub that matches topics to processes");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    bool apple = false;

    options
      // .allow_unrecognised_options()
      .add_options()
      ("a,address", "multicast IP address", cxxopts::value<std::string>()->default_value("1.1.1.1"))
      ("d,delay", "delay time between prints", cxxopts::value<float>()->default_value("3.0"))
      ("k,key", "geckocore key", cxxopts::value<std::string>()->default_value("local"))
      ("p,port", "multicast port number", cxxopts::value<int>()->default_value("11311"))
      ("v,verbose", "verbose", cxxopts::value<bool>()->default_value("false"))
      ("h,help", "prints help message")
    ;

    // options.add_options("Group")
    //   ("c,compile", "compile")
    //   ("d,drop", "drop", cxxopts::value<std::vector<std::string>>());

    // options.parse_positional({"input", "output", "positional"});

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    return result;

  } catch (const cxxopts::OptionException& e)
  {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}

int main(int argc, char* argv[])
{
    auto result = parse(argc, argv);
    string addr = result["address"].as<string>();
    int port = result["port"].as<int>();
    string key = result["key"].as<string>();

    printf("Geckocore [%s] listening on %s:%d\n",
        key.c_str(),
        addr.c_str(),
        port);

    return 0;
}
