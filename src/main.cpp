#include <utility>

#include <iostream>
#include <libconfig.h++>
#include <iomanip>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include "Server.h"

using namespace std;
using namespace libconfig;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

// Пример, читающий конфигурационный файл 'example.cfg' и выводит его записи

fs::path config_path;

void on_config(fs::path path) {
    config_path = std::move(path);
}

void RunServer(const Config &config, boost::asio::io_service &io_service) {
    const Setting &root = config.getRoot();
    string socketPath;

    const Setting &daemon = root["application"]["daemon"];

    if (!daemon["server"].lookupValue("socket", socketPath)) {
        std::cerr << "cannot get server socket config" << endl;
        return;
    }

    std::remove(socketPath.c_str());
    Server s(io_service, socketPath);

    std::cout << "server started on " << socketPath << endl;

    io_service.run();
}


int main(int argc, const char *argv[]) {
    try {
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("config,c", po::value<fs::path>()->notifier(on_config)->default_value("/etc/nrf_proxy.cfg"),
                 "Configuration file path");

        po::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
            return (EXIT_SUCCESS);
        }
    }
    catch (const po::error &ex) {
        std::cerr << ex.what() << '\n';
        return (EXIT_FAILURE);
    }

    Config cfg;

    if (!fs::exists(config_path)) {
        std::cerr << "config file does not exists: " << config_path << endl;
        exit(1);
    }

    fs::ifstream f{config_path};

    std::string data((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
    f.close();

    try {
        cfg.readString(data);
    }
    catch (const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return (EXIT_FAILURE);
    }

    boost::asio::io_service io_service;

    boost::thread socketServerThread(RunServer, std::ref(cfg), std::ref(io_service));
    socketServerThread.join();

    return (EXIT_SUCCESS);
}