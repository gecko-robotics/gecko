
#include <iostream>

#include "cxxopts.hpp"

using namespace std;

cxxopts::ParseResult
parse(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], "topics");
  try
  {
    // cxxopts::Options options(argv[0], "topics");
    options
      .positional_help("[input output]")
      .show_positional_help()
      // .add_options()
      // ("wtf", "Positional arguments", cxxopts::value<std::vector<std::string>>())
      ;

    // bool apple = false;

    options
      .allow_unrecognised_options()
      .add_options()
      // ("a,apple", "an apple", cxxopts::value<bool>(apple))
    //   ("b,bob", "Bob")
    //   ("t,true", "True", cxxopts::value<bool>()->default_value("true"))
    //   ("f, file", "File", cxxopts::value<std::vector<std::string>>(), "FILE")
    //   ("i,input", "Input", cxxopts::value<std::string>())
    //   ("o,output", "Output file", cxxopts::value<std::string>()
    //       ->default_value("a.out")->implicit_value("b.def"), "BIN")
      // ("positional",
      //   "Positional arguments: these are the arguments that are entered "
      //   "without an option", cxxopts::value<std::vector<std::string>>())
    //   ("long-description",
    //     "thisisareallylongwordthattakesupthewholelineandcannotbebrokenataspace")
      ("help", "Print help")
    //   ("int", "An integer", cxxopts::value<int>(), "N")
    //   ("float", "A floating point number", cxxopts::value<float>())
    //   ("option_that_is_too_long_for_the_help", "A very long option")
    // #ifdef CXXOPTS_USE_UNICODE
    //   ("unicode", u8"A help option with non-ascii: à. Here the size of the"
    //     " string should be correct")
    // #endif
    ;

    options.add_options("pub")
      // .positional_help("[stuff]")
        ("something", "help")
        // ("positional", "publish once", cxxopts::value<bool>()->default_value("true"))
        ("r,rate", "rate in Hz", cxxopts::value<int>()->default_value("1"))
        ;

    options.add_options("echo")
        ("g", "publish once", cxxopts::value<bool>()->default_value("true"))
        // ("input", "Positional arguments", cxxopts::value<std::vector<std::string>>())
        ("input", "Positional arguments", cxxopts::value<std::vector<std::string>>())
        ("output", "Positional arguments", cxxopts::value<std::vector<std::string>>())
        ;

    options.parse_positional({"input", "output"});

    auto result = options.parse(argc, argv);
    //
    if (result.count("help"))
    {
      std::cout << options.help({"", "pub", "echo"}) << std::endl;
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
    auto arguments = result.arguments();
    // std::cout << "Saw " << arguments.size() << " arguments" << std::endl;
    cout << result.count("input")  <<  endl;
    auto ss = result["input"].as<std::vector<std::string>>();
    for (auto const& s: ss) cout << s << endl;

    cout << result.count("output")  <<  endl;
    auto sss = result["output"].as<std::vector<std::string>>();
    for (auto const& s: sss) cout << s << endl;

    return 0;
}
