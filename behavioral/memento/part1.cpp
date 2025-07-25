#include <iostream>
#include <memory>
#include <vector>

class Query {
 public:
  explicit Query(int amount) : amount_(amount) {}

  virtual ~Query() = default;

  int amount() const {
    return amount_;
  }

 private:
  int amount_;
};

class DepositQuery : public Query {
 public:
  explicit DepositQuery(int amount) : Query(amount) {}
};

class WithdrawQuery : public Query {
 public:
  explicit WithdrawQuery(int amount) : Query(amount) {}
};

class Transaction {
 public:
  void AddQuery(Query* query) {
    queries_.emplace_back(query);
  }

  const std::vector<std::unique_ptr<Query>>& queries() const {
    return queries_;
  }

 private:
  std::vector<std::unique_ptr<Query>> queries_;
};

class BankAccount {
 public:
  class Memento {
   private:
    friend class BankAccount;

    Memento(int balance) : balance_(balance) {}

    int balance() const {
      return balance_;
    }

    int balance_;
  };

  BankAccount(int balance = 0) : balance_(balance) {}

  bool ProcessTransaction(const Transaction& tx) {
    for (const auto& query : tx.queries()) {
      if (auto deposit = dynamic_cast<DepositQuery*>(query.get())) {
        Deposit(deposit->amount());
      } else if (auto withdraw = dynamic_cast<WithdrawQuery*>(query.get())) {
        if (!Withdraw(withdraw->amount())) {
          return false;  // Transaction failed
        }
      }
    }
    return true;  // Transaction successful
  }

  Memento Save() const {
    return Memento(balance_);
  }

  void Restore(const Memento& memento) {
    balance_ = memento.balance();
  }

  /*
    Balance is hidden from the 3rd party.
    Here, however, we provide a method to get the balance for demonstration purposes.
  */
  int balance() const {
    return balance_;
  }

 private:
  void Deposit(int amount) {
    balance_ += amount;
  }

  /*
  You can withdraw money from the account if the amount is less than or equal to the current
  balance.
  */
  bool Withdraw(int amount) {
    if (amount > balance_) {
      return false;
    }
    balance_ -= amount;
    return true;
  }

 private:
  int balance_;
};

int main() {
  /*
    Let's assume a 3rd party (Care-taker) handles transactions for a bank account.

    The balance of the bank account is hidden from the 3rd party.

    The 3rd party can only make deposits and withdrawals through a transaction object.

    If the transaction is successful, the state of the bank account is not restored.
    If the transaction fails, the state of the bank account is restored to the previous state.

    The 3rd party relies on the memento of the bank account to restore the state in case of a
    failure. The memento is created before the transaction is handled.

    The 3rd party can restore the state of the bank account without knowing the details of the bank
    account.
  */

  {
    BankAccount account;
    Transaction tx;
    tx.AddQuery(new DepositQuery(100));
    tx.AddQuery(new WithdrawQuery(50));

    std::cout << "Initial balance: $" << account.balance() << "\n";
    auto memento = account.Save();
    if (account.ProcessTransaction(tx)) {
      std::cout << "Transaction successful\n";
    } else {
      std::cout << "Transaction failed\n";
    }
    std::cout << "Final balance: $" << account.balance() << "\n";

    // As the transaction was successful, you do not need to restore the state.
  }
  std::cout << "----------------------------------\n";
  {
    BankAccount account(49);
    Transaction tx;
    tx.AddQuery(new DepositQuery(100));
    tx.AddQuery(new WithdrawQuery(150));  // This will fail

    std::cout << "Initial balance: $" << account.balance() << "\n";
    auto memento = account.Save();
    if (account.ProcessTransaction(tx)) {
      std::cout << "Transaction successful\n";
    } else {
      std::cout << "Transaction failed\n";
    }
    std::cout << "Final balance (Wrong): $" << account.balance() << "\n";

    std::cout << "Restoring the internal state...\n";
    account.Restore(memento);
    std::cout << "Final balance (Rolled Back): $" << account.balance() << "\n";
  }
  return 0;
}