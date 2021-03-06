#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "deposit_manager.h"

#include <boost/program_options.hpp>

using DepositManager = deposit_manager::DepositManager;

DepositManager::Config ReadConfiguration(int argc, char* argv[]) {
  DepositManager::Config config;

  using namespace boost::program_options;

  options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")("list,l",
                                                       "Show all securities")(
      "securities-filter,s",
      value<std::vector<std::string>>(&config.securities),
      "Set securities filter")(
      "deposit,d", value<double>(&config.deposit)->default_value(0.0), "")(
      "free,f", value<double>(&config.deposit_free)->default_value(0.0), "")(
      "broker-tax,b", value<double>(&config.tax_broker)->default_value(0.003),
      "")("exchange-tax,e",
          value<double>(&config.tax_exchange)->default_value(0.0001),
          "")("risk-level,r",
              value<double>(&config.risk_level)->default_value(0.01), "");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help")) {
    std::cout << desc;
    throw std::exception();
  }

  if (config.deposit <= 0.0 || config.deposit_free <= 0.0) {
    throw std::runtime_error(
        "deposit  and free deposit contains incorrect values");
  }

  if (config.deposit_free > config.deposit) {
    throw std::runtime_error(
        "free deposit should not exceed the size of the main deposit");
  }

  config.show_list = vm.count("list");

  config.tax_broker = std::clamp(config.tax_broker, 0.0, 1.0);
  config.tax_exchange = std::clamp(config.tax_exchange, 0.0, 1.0);
  config.risk_level = std::clamp(config.risk_level, 0.0, 1.0);

  std::cout << "\nProgram start with parameters:\nDeposit: " << config.deposit
            << "\nFree deposit: " << config.deposit_free
            << "\nRisk level: " << config.risk_level
            << "\nBroker tax: " << config.tax_broker
            << "\nExchange tax: " << config.tax_exchange << "\n"
            << std::endl;

  return config;
}

using namespace std;

int main(int argc, char* argv[]) {
  try {
    const auto config = ReadConfiguration(argc, argv);

    DepositManager deposit_manager;
    deposit_manager.Run(config);

  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
