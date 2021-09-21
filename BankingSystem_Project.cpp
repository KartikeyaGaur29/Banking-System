#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

// Minimum Balance of 500
#define MIN_BALANCE 500

// Class Insufficient Funds
class InsufficientFunds
{
};
// End of Class Insufficient Funds

// Class Account
class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }
    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};
long Account::NextAccountNumber = 0;
// Members of Class Account
Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}
void Account::Deposit(float amount)
{
    balance += amount;
}
void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}
void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}
long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}
ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}
ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}
ostream &operator<<(ostream &os, Account &acc)
{
    os << "\n\t First Name:" << acc.getFirstName() << endl;
    os << "\n\t Last Name:" << acc.getLastName() << endl;
    os << "\n\t Account Number:" << acc.getAccNo() << endl;
    os << "\n\t Balance:" << acc.getBalance() << endl;
    return os;
}
// End of Class Account

// Class Bank
class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};
// Members of Class Bank
Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        cout << "\n\t Error in Opening! File Not Found!!" << endl;
        return;
    }
    while (!infile.eof())
    {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccountNumber(account.getAccNo());
    infile.close();
}
Account Bank::OpenAccount(string fname, string lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}
Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    return itr->second;
}
Account Bank::Deposit(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}
Account Bank::Withdraw(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}
void Bank::CloseAccount(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    cout << "\n\t Account Deleted" << itr->second;
    accounts.erase(accountNumber);
}
void Bank::ShowAllAccounts()
{
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout << "\n\t Account " << itr->first << endl
             << itr->second << endl;
    }
}
Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
}
// End of Class Bank

// Main Function
int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "\n\t\t\t\t  ***  Banking System  ***  " << endl;
    do
    {
        cout << "\n\t Select one option below ";
        cout << "\n\t 1 Open an Account";
        cout << "\n\t 2 Balance Enquiry";
        cout << "\n\t 3 Deposit";
        cout << "\n\t 4 Withdrawal";
        cout << "\n\t 5 Close an Account";
        cout << "\n\t 6 Show All Accounts";
        cout << "\n\t 7 Quit";
        cout << "\n\n Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "\n\n\t\t Opening a new Account......";
            cout << "\n\n\t Enter your First Name : ";
            cin >> fname;
            cout << "\n\t Enter your Last Name : ";
            cin >> lname;
            cout << "\n\t Enter your Initil Balance : ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << endl
                 << "\n\n\tCongradulation! Your Account is Created" << endl;
            cout << acc;
            break;
        case 2:
            cout << "\n\n\t\t Balance Enquiry being Processed......";
            cout << "\n\n\t Enter your Account Number : ";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << endl
                 << "\n\t Your Account Details are : " << endl;
            cout << acc;
            break;
        case 3:
            cout << "\n\n\t\t Depositing to your Account.......";
            cout << "\n\n\t Enter your Account Number : ";
            cin >> accountNumber;
            cout << "\n\t Enter the amount to be Deposited : ";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << endl
                 << "\n\t Amount is Deposited" << endl;
            cout << acc;
            break;
        case 4:
            cout << "\n\n\t\t Withdrawing from your Account.......";
            cout << "\n\n\t Enter Account Number:";
            cin >> accountNumber;
            cout << "\n\t Enter Balance:";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << endl
                 << "\n\t Amount Withdrawn" << endl;
            cout << acc;
            break;
        case 5:
            cout << "\n\n\t\t Closing an Account........";
            cout << "\n\n\t Enter Account Number:";
            cin >> accountNumber;
            b.CloseAccount(accountNumber);
            cout << endl
                 << "\n\t Account is Closed" << endl;
            cout << acc;
            break;
        case 6:
            cout << "\n\n\t\t Showing All the Accounts.......";
            b.ShowAllAccounts();
            break;
        case 7:
            cout << "\n\n\t\t Quitting the Program......";
            break;
        default:
            cout << "\nEnter corret choice";
            exit(0);
        }
    } while (choice != 7);
    return 0;
}
// End of Main Function
