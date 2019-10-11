#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}

int main(int argc, char** argv){
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("compression,c", po::value<int>(), "set compression level")
        ("pi", po::value<float>()->default_value(3.14f), "Pi")
        ("age", po::value<int>()->notifier(on_age), "Age")
        ("string,s", po::value<string>(), "string entry")
    ;

    po::positional_options_description p;
    p.add("string", 1);
    p.add("age", 2);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(),vm);
    }
    catch(...){
        
    }

    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        cout << "Compression level was set to "
     << vm["compression"].as<int>() << ".\n";
    } else {
        cout << "Compression level was not set.\n";
    }

    if (vm.count("string")){
        cout << "string: " << vm["string"].as<string>() << endl;
    }

    return 0;
}
