#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <locale>  // ��� ������������ �����

using namespace std;

// ���� ��� ����������
class Transaction {
public:
    string category;
    double amount;
    string date;

    Transaction(string cat, double amt, string dt) : category(cat), amount(amt), date(dt) {}
};

// ������� ���� ��� ����������� �������
class FinancialAccount {
protected:
    double balance;
    vector<Transaction> transactions;

    // ������� ��� ��������� ������� ����
    string getCurrentDate() {
        time_t now = time(0);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);  // ������������� ctime_s ��� �������
        buffer[strlen(buffer) - 1] = '\0';  // ��������� ������ ������ ������ �����
        return string(buffer);
    }

public:
    FinancialAccount() : balance(0) {}  // ���������� balance � �������� ����

    virtual void showBalance() = 0;
    virtual void deposit(double amount) = 0;
    virtual void purchase(double amount, const string& category) = 0;
    virtual void saveTransaction(const Transaction& transaction) = 0;
};

// ���� ��� �������� ������
class DebitCard : public FinancialAccount {
public:
    DebitCard() : FinancialAccount() {}  // ��������� ����������� �������� ����� ��� ����������� balance

    void showBalance() override {
        cout << "������ �� �������� ������: " << balance << " ���" << endl;
    }

    void deposit(double amount) override {
        balance += amount;
        cout << "������ ���� ����������: " << balance << " ���" << endl;
    }

    void purchase(double amount, const string& category) override {
        if (balance >= amount) {
            balance -= amount;
            string date = getCurrentDate();
            Transaction transaction(category, amount, date);
            saveTransaction(transaction);
            cout << "�������� ������! ����� " << amount << " ��� �� ������� � �������: " << category << endl;
        }
        else {
            cout << "����������� ����� �� ������!" << endl;
        }
    }

    void saveTransaction(const Transaction& transaction) override {
        ofstream file("transactions.txt", ios::app);
        if (file.is_open()) {
            file << transaction.date << " | ��������: " << transaction.category << " | ����: " << transaction.amount << endl;
            file.close();
        }
        else {
            cout << "�� ������� ������� ���� ��� ������!" << endl;
        }
    }
};

// ���� ��� �������
class Wallet : public FinancialAccount {
public:
    Wallet() : FinancialAccount() {}  // ��������� ����������� �������� ����� ��� ����������� balance

    void showBalance() override {
        cout << "������ � �������: " << balance << " ���" << endl;
    }

    void deposit(double amount) override {
        balance += amount;
        cout << "������ ���� ����������: " << balance << " ���" << endl;
    }

    void purchase(double amount, const string& category) override {
        if (balance >= amount) {
            balance -= amount;
            string date = getCurrentDate();
            Transaction transaction(category, amount, date);
            saveTransaction(transaction);
            cout << "�������� ������! ����� " << amount << " ��� �� ������� � �������: " << category << endl;
        }
        else {
            cout << "����������� ����� � �������!" << endl;
        }
    }

    void saveTransaction(const Transaction& transaction) override {
        ofstream file("transactions.txt", ios::app);
        if (file.is_open()) {
            file << transaction.date << " | ��������: " << transaction.category << " | ����: " << transaction.amount << endl;
            file.close();
        }
        else {
            cout << "�� ������� ������� ���� ��� ������!" << endl;
        }
    }
};

// ���� ��� ������
class Product {
public:
    string name;
    double price;
    Product(string n, double p) : name(n), price(p) {}
};

// ���� ��� ��������� ��������� ����������
class Shop {
public:
    vector<Product> products;

    Shop() {
        // ����������� ������
        products.push_back(Product("�������", 10000));
        products.push_back(Product("��������", 5000));
        products.push_back(Product("�����", 800));
    }

    void showProducts() {
        cout << "������� ������:" << endl;
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << " - " << products[i].price << " ���" << endl;
        }
    }

    void purchaseProduct(int productIndex, FinancialAccount* account) {
        if (productIndex < 1 || productIndex > products.size()) {
            cout << "������� ���� ������!" << endl;
            return;
        }
        double price = products[productIndex - 1].price;
        string category = "������ ������";
        account->purchase(price, category);
    }
};

// ���� ��� ������������
class Cryptocurrency {
public:
    double balance;

    Cryptocurrency() : balance(0) {}

    void showBalance() {
        cout << "������ ������������: " << balance << " BTC" << endl;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "������ ���� ����������: " << balance << " BTC" << endl;
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            cout << "�������� " << amount << " BTC" << endl;
        }
        else {
            cout << "����������� �����!" << endl;
        }
    }

    void transfer(string address, double amount) {
        if (balance >= amount) {
            balance -= amount;
            ofstream file("crypto_transactions.txt", ios::app);
            if (file.is_open()) {
                file << "���������� �� " << address << " | ����: " << amount << " BTC" << endl;
                file.close();
            }
            else {
                cout << "�� ������� ������� ���� ��� ������!" << endl;
            }
            cout << "���������� ������ ���������!" << endl;
        }
        else {
            cout << "����������� ����� ��� ��������!" << endl;
        }
    }
};

// ������� �������
int main() {
    // ������������ ����� ��� �������� ���������� �������
    setlocale(LC_ALL, "ukr");

    int choice;
    FinancialAccount* account = nullptr;
    Cryptocurrency crypto;

    cout << "������� ��� �������: \n1. �������� ������\n2. ��������\n3. ������������\n";
    cin >> choice;

    switch (choice) {
    case 1:
        account = new DebitCard();
        break;
    case 2:
        account = new Wallet();
        break;
    case 3:
        account = nullptr;  // ������������ - ������� ����
        break;
    default:
        cout << "������� ����!" << endl;
        return 0;
    }

    if (account != nullptr) {
        while (true) {
            int action;
            cout << "\n������� ��:\n1. ��������� �������\n2. ������ �����\n3. �����\n";
            cin >> action;

            if (action == 1) {
                double amount;
                cout << "������ ���� ��� ����������: ";
                cin >> amount;
                account->deposit(amount);
            }
            else if (action == 2) {
                Shop shop;
                shop.showProducts();
                int productChoice;
                cout << "������� �����: ";
                cin >> productChoice;
                shop.purchaseProduct(productChoice, account);
            }
            else if (action == 3) {
                break;
            }
            else {
                cout << "������� ����!" << endl;
            }
        }
        delete account;
    }

    // ���� ������� ������������
    else {
        while (true) {
            int cryptoAction;
            cout << "\n������� �� ��� ������������:\n1. ��������� ������\n2. ������� ������������\n3. ���������� �� ������\n4. �����\n";
            cin >> cryptoAction;

            if (cryptoAction == 1) {
                double amount;
                cout << "������ ���� ��� ����������: ";
                cin >> amount;
                crypto.deposit(amount);
            }
            else if (cryptoAction == 2) {
                double amount;
                cout << "������ ���� ��� ���������: ";
                cin >> amount;
                crypto.withdraw(amount);
            }
            else if (cryptoAction == 3) {
                string address;
                double amount;
                cout << "������ ������ ��� ��������: ";
                cin >> address;
                cout << "������ ���� ��� ��������: ";
                cin >> amount;
                crypto.transfer(address, amount);
            }
            else if (cryptoAction == 4) {
                break;
            }
            else {
                cout << "������� ����!" << endl;
            }
        }
    }

    return 0;
}
