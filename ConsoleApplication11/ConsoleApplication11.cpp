#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <locale>  // Для налаштування локалі

using namespace std;

// Клас для транзакції
class Transaction {
public:
    string category;
    double amount;
    string date;

    Transaction(string cat, double amt, string dt) : category(cat), amount(amt), date(dt) {}
};

// Базовий клас для фінансового рахунку
class FinancialAccount {
protected:
    double balance;
    vector<Transaction> transactions;

    // Функція для отримання поточної дати
    string getCurrentDate() {
        time_t now = time(0);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);  // Використовуємо ctime_s для безпеки
        buffer[strlen(buffer) - 1] = '\0';  // Видаляємо зайвий символ нового рядка
        return string(buffer);
    }

public:
    FinancialAccount() : balance(0) {}  // Ініціалізуємо balance в базовому класі

    virtual void showBalance() = 0;
    virtual void deposit(double amount) = 0;
    virtual void purchase(double amount, const string& category) = 0;
    virtual void saveTransaction(const Transaction& transaction) = 0;
};

// Клас для дебетової картки
class DebitCard : public FinancialAccount {
public:
    DebitCard() : FinancialAccount() {}  // Викликаємо конструктор базового класу для ініціалізації balance

    void showBalance() override {
        cout << "Баланс на дебетовій картці: " << balance << " грн" << endl;
    }

    void deposit(double amount) override {
        balance += amount;
        cout << "Баланс після поповнення: " << balance << " грн" << endl;
    }

    void purchase(double amount, const string& category) override {
        if (balance >= amount) {
            balance -= amount;
            string date = getCurrentDate();
            Transaction transaction(category, amount, date);
            saveTransaction(transaction);
            cout << "Операція успішна! Знято " << amount << " грн за покупку в категорії: " << category << endl;
        }
        else {
            cout << "Недостатньо коштів на картці!" << endl;
        }
    }

    void saveTransaction(const Transaction& transaction) override {
        ofstream file("transactions.txt", ios::app);
        if (file.is_open()) {
            file << transaction.date << " | Категорія: " << transaction.category << " | Сума: " << transaction.amount << endl;
            file.close();
        }
        else {
            cout << "Не вдалося відкрити файл для запису!" << endl;
        }
    }
};

// Клас для гаманця
class Wallet : public FinancialAccount {
public:
    Wallet() : FinancialAccount() {}  // Викликаємо конструктор базового класу для ініціалізації balance

    void showBalance() override {
        cout << "Баланс в гаманці: " << balance << " грн" << endl;
    }

    void deposit(double amount) override {
        balance += amount;
        cout << "Баланс після поповнення: " << balance << " грн" << endl;
    }

    void purchase(double amount, const string& category) override {
        if (balance >= amount) {
            balance -= amount;
            string date = getCurrentDate();
            Transaction transaction(category, amount, date);
            saveTransaction(transaction);
            cout << "Операція успішна! Знято " << amount << " грн за покупку в категорії: " << category << endl;
        }
        else {
            cout << "Недостатньо коштів в гаманці!" << endl;
        }
    }

    void saveTransaction(const Transaction& transaction) override {
        ofstream file("transactions.txt", ios::app);
        if (file.is_open()) {
            file << transaction.date << " | Категорія: " << transaction.category << " | Сума: " << transaction.amount << endl;
            file.close();
        }
        else {
            cout << "Не вдалося відкрити файл для запису!" << endl;
        }
    }
};

// Клас для товару
class Product {
public:
    string name;
    double price;
    Product(string n, double p) : name(n), price(p) {}
};

// Клас для управління товарними операціями
class Shop {
public:
    vector<Product> products;

    Shop() {
        // Ініціалізація товарів
        products.push_back(Product("Ноутбук", 10000));
        products.push_back(Product("Смартфон", 5000));
        products.push_back(Product("Клава", 800));
    }

    void showProducts() {
        cout << "Доступні товари:" << endl;
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " грн" << endl;
        }
    }

    void purchaseProduct(int productIndex, FinancialAccount* account) {
        if (productIndex < 1 || productIndex > products.size()) {
            cout << "Невірний вибір товару!" << endl;
            return;
        }
        double price = products[productIndex - 1].price;
        string category = "Купівля товару";
        account->purchase(price, category);
    }
};

// Клас для криптовалюти
class Cryptocurrency {
public:
    double balance;

    Cryptocurrency() : balance(0) {}

    void showBalance() {
        cout << "Баланс криптовалюти: " << balance << " BTC" << endl;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Баланс після поповнення: " << balance << " BTC" << endl;
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "Виведено " << amount << " BTC" << endl;
        }
        else {
            cout << "Недостатньо коштів!" << endl;
        }
    }

    void transfer(string address, double amount) {
        if (balance >= amount) {
            balance -= amount;
            ofstream file("crypto_transactions.txt", ios::app);
            if (file.is_open()) {
                file << "Транзакція на " << address << " | Сума: " << amount << " BTC" << endl;
                file.close();
            }
            else {
                cout << "Не вдалося відкрити файл для запису!" << endl;
            }
            cout << "Транзакція успішно завершена!" << endl;
        }
        else {
            cout << "Недостатньо коштів для переказу!" << endl;
        }
    }
};

// Головна функція
int main() {
    // Налаштування локалі для підтримки українських символів
    setlocale(LC_ALL, "ukr");

    int choice;
    FinancialAccount* account = nullptr;
    Cryptocurrency crypto;

    cout << "Виберіть тип рахунку: \n1. Дебетова картка\n2. Гаманець\n3. Криптовалюта\n";
    cin >> choice;

    switch (choice) {
    case 1:
        account = new DebitCard();
        break;
    case 2:
        account = new Wallet();
        break;
    case 3:
        account = nullptr;  // Криптовалюта - окремий клас
        break;
    default:
        cout << "Невірний вибір!" << endl;
        return 0;
    }

    if (account != nullptr) {
        while (true) {
            int action;
            cout << "\nВиберіть дію:\n1. Поповнити рахунок\n2. Купити товар\n3. Вийти\n";
            cin >> action;

            if (action == 1) {
                double amount;
                cout << "Введіть суму для поповнення: ";
                cin >> amount;
                account->deposit(amount);
            }
            else if (action == 2) {
                Shop shop;
                shop.showProducts();
                int productChoice;
                cout << "Виберіть товар: ";
                cin >> productChoice;
                shop.purchaseProduct(productChoice, account);
            }
            else if (action == 3) {
                break;
            }
            else {
                cout << "Невірний вибір!" << endl;
            }
        }
        delete account;
    }

    // Якщо вибрана криптовалюта
    else {
        while (true) {
            int cryptoAction;
            cout << "\nВиберіть дію для криптовалюти:\n1. Поповнити баланс\n2. Вивести криптовалюту\n3. Переказати на адресу\n4. Вийти\n";
            cin >> cryptoAction;

            if (cryptoAction == 1) {
                double amount;
                cout << "Введіть суму для поповнення: ";
                cin >> amount;
                crypto.deposit(amount);
            }
            else if (cryptoAction == 2) {
                double amount;
                cout << "Введіть суму для виведення: ";
                cin >> amount;
                crypto.withdraw(amount);
            }
            else if (cryptoAction == 3) {
                string address;
                double amount;
                cout << "Введіть адресу для переказу: ";
                cin >> address;
                cout << "Введіть суму для переказу: ";
                cin >> amount;
                crypto.transfer(address, amount);
            }
            else if (cryptoAction == 4) {
                break;
            }
            else {
                cout << "Невірний вибір!" << endl;
            }
        }
    }

    return 0;
}
