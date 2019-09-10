#include "deposit_manager.h"

#include <iostream>

#include <securities.h>

namespace deposit_manager {

Deposit GetDeposit();
Broker GetBroker();
Securities GetSecurities(const Broker& broker);

void DepositManager::Run() {
  try {
    _deposit = GetDeposit();
    _broker = GetBroker();

    // todo: add loop
    const auto securities = GetSecurities(_broker);

    ShowInformation(securities);

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

Deposit GetDeposit() {
  double deposit = 8876.09, free_deposit = 4300;

  std::cout << "Deposit: ";
  std::cin >> deposit;
  if (std::cin.fail() || deposit <= 0.0) {
    throw std::runtime_error(
        "Incorrect deposit value. Deposit must by greater than 0");
  }

  std::cout << "Free deposit: ";
  std::cin >> free_deposit;
  if (std::cin.fail() || free_deposit < 0.0 || free_deposit > deposit) {
    throw std::runtime_error(
        "Incorrect free deposit value. Free deposit cannot be negative or more "
        "than "
        "deposit.");
  }

  double risk = 0.0;
  std::cout << "Available risk for deposit ( from 0.0 to 1.0 ): ";
  std::cin >> risk;
  if (std::cin.fail() || (risk < 0.0 || risk > 1.0)) {
    throw std::runtime_error("Incorrect risk value. Risk Level Out of Range.");
  }

  Deposit depo(deposit, free_deposit);
  depo.SetRiskLevel(risk);

  return depo;
}

Broker GetBroker() {
  double tax_broker = 0.003;
  double tax_exchange = 0.0001;

  std::cout << "Broker tax: " << tax_broker
            << "(default); Exchange tax: " << tax_exchange << "(default);"
            << std::endl;

  return Broker(tax_broker, tax_exchange);
}

Securities GetSecurities(const Broker& broker) {
  double price = 0.0;
  std::cout << "Securities close price: ";
  std::cin >> price;

  if (std::cin.fail() || price <= 0.0) {
    throw std::runtime_error("Incorrect price. Price must be greater than 0.");
  }

  int lot_size = 1;

  std::cout << "Set lot size (1,10,100,1000,10000): ";
  std::cin >> lot_size;

  if (std::cin.fail() || lot_size < 1) {
    throw std::runtime_error(
        "Incorrect lot size. Lot size must match (1,10,100,1000,10000).");
  }

  Securities securities(price, broker);
  securities.SetLotSize(lot_size);

  return securities;
};

void DepositManager::ShowInformation(const Securities& securities) {
  const auto risk = _deposit.Risk();
  const auto volume = _deposit.AvailableVolume(securities);
  const auto stop_loss = _deposit.StopLossLevel(securities);
  const auto take_profit = _deposit.TakeProfitLevel(securities);
  const auto price = securities.Price();

  const auto profit = (take_profit - securities.BuyPrice()) * volume;

  std::cout << "Price: " << price << "\nVolume: " << volume
            << "\nStop loss: " << stop_loss << "\nTake profit: " << take_profit
            << "\nRisk: " << risk << "\nProfit: " << profit << std::endl;
}

}  // namespace deposit_manager
