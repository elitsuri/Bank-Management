#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds {};

//Class Account starts
class Account
{
private:
    long id;
    string m_name;
    float Balance;
    static long nextAccNumber;
    string m_type;
    string m_created_at;
    string m_updated_at;

public:
    Account() {}
    Account(string name, float bal)
    {
        nextAccNumber++;
        id = nextAccNumber;
        m_name = name;
        this->Balance = bal;
    }
    long getAccnumber() { return id; }
    string get_name() { return m_name; }
    float getbalance() { return Balance; }
    string get_created_at() { return m_created_at; }
    string get_updated_at() { return m_updated_at; }

    void Deposit(float amount)
    {
        Balance = Balance + amount;
    }
    void Withdraw(float amount)
    {
        if (Balance - amount < MIN_BALANCE)
        {
            throw InsufficientFunds();
        }
        Balance = Balance - amount;
    }
    static void setLastAccNum(long accNum)
    {
        nextAccNumber = accNum;
    }
    static long getLastAccNum()
    {
        return nextAccNumber;
    }
    friend ofstream& operator<<(ofstream& ofs, Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
    friend ostream& operator<<(ostream& os, Account& acc);

};

long Account::nextAccNumber = 0;

ofstream& operator<<(ofstream& ofs, Account& acc)
{
    ofs << acc.id << endl;
    ofs << acc.m_name << endl;
    ofs << acc.Balance << endl;
    return ofs;
}
ifstream& operator>>(ifstream& ifs, Account& acc)
{
    ifs >> acc.id;
    ifs >> acc.m_name;
    ifs >> acc.Balance;
    return ifs;
}
ostream& operator<<(ostream& os, Account& acc)
{
    os << "Name: " << acc.get_name() << endl;
    os << "Account Number: " << acc.getAccnumber() << endl;
    os << "Balance: " << acc.getbalance() << endl;
    return os;
}
//Class Account ends

//Class Bank starts
class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank()
    {
        Account account;
        ifstream infile("Bank.data");
        if (!infile)
        {
            cout << "Error in opening file! File not found!" << endl;
            return;
        }
        while (!infile.eof())
        {
            infile >> account;
            accounts.insert(pair<long, Account>(account.getAccnumber(), account));
        }
        Account::setLastAccNum(account.getAccnumber());
        infile.close();
    }
    Account OpenAcc(string name, float bal)
    {
        ofstream outfile;
        Account account(name, bal);
        accounts.insert(pair<long, Account>(account.getAccnumber(), account));

        outfile.open("Bank.data", ios::trunc);

        map<long, Account> ::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++)
            outfile << itr->second;

        outfile.close();

        return account;
    }
    Account balanceEnquiry(long accNum)
    {
        map<long, Account> ::iterator itr = accounts.find(accNum);
        return itr->second;
    }
    Account Deposit(long accNum, float amount)
    {
        map<long, Account> ::iterator itr = accounts.find(accNum);
        itr->second.Deposit(amount);
        return itr->second;
    }
    Account Withdraw(long accNum, float amount)
    {
        map<long, Account> ::iterator itr = accounts.find(accNum);
        itr->second.Withdraw(amount);
        return itr->second;
    }
    void CloseAcc(long accNum)
    {
        map<long, Account> ::iterator itr = accounts.find(accNum);
        cout << "ACCOUNT DELETED\n" << itr->second;
        cout << endl;
        accounts.erase(accNum);
    }
    void ShowAllAcc()
    {
        map<long, Account> ::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++)
        {
            cout << "Account: " << itr->first << endl << itr->second.get_created_at() << endl;
        }
    }
    ~Bank()
    {
        ofstream outfile("Bank.data", ios::trunc);
        map<long, Account> ::iterator itr;
        for (itr = accounts.begin(); itr != accounts.end(); itr++)
        {
            outfile << itr->second;
        }
        outfile.close();
    }

};
//Bank class ends
bool find(vector<pair<string, int>> vec_user, string user_name, int password)
{
    for (int i = 0; i < vec_user.size(); i++)
    {
        if (vec_user[i].first == user_name && vec_user[i].second == password)
            return true;
    }

    return false;
}
int main()
{
    Bank b;
    Account acc;
    int choice, c1, password;
    string name, user_name;
    long accountNumber;
    float balance;
    float amount;

    vector<pair<string, int>> Admin;
    vector<pair<string, int>> Manager;
    vector<pair<string, int>> Custumer;

    do
    {
        cout << "\n\tEnter your type:";
        cout << "\n\t1 Custumer";
        cout << "\n\t2 Admin";
        cout << "\n\t3 Manager\n";
        cin >> c1;

    } while (c1 > 3 || c1 < 1);

    cout << "\n\tEnter User name: ";
    cin >> user_name;

    cout << "\n\tEnter Password: ";
    cin >> password;
    if (c1 == 1 && find(Custumer, user_name, password))
    {
        cout << "\n\tSelect one option below: ";
        cout << "\n\t1 Deposit";
        cout << "\n\t2 Withdrawal";
        cout << "\n\t3 Balance Enquiry";
        cout << "\n\t7 Quit";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            cout << "==========DEPOSIT AMOUNT==========" << endl;
            cout << endl;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Balance: ";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << endl << "Amount is Deposited" << endl;
            cout << acc;
            break;
        }
        case 2:
        {
            cout << "==========WITHDRAW AMOUNT==========" << endl;
            cout << endl;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Balance: ";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << endl << "Amount Withdrawn" << endl;
            cout << acc;
            break;
        }
        case 3:
        {
            cout << "==========BALANCE ENQUIRY==========" << endl;
            cout << endl;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            acc = b.balanceEnquiry(accountNumber);
            cout << endl << "Your Account Details: " << endl;
            cout << acc;
            break;
        }
        }
    }

    if (c1 == 1 && find(Admin, user_name, password))
    {
        cout << "<< BANKING SYSTEM by SMJI >>" << endl;

        do
        {
            cout << "\n\tSelect one option below ";
            cout << "\n\t1 Open an Account";
            cout << "\n\t2 Balance Enquiry";
            cout << "\n\t3 Deposit";
            cout << "\n\t4 Withdrawal";
            cout << "\n\t5 Close an Account";
            cout << "\n\t6 Show All Accounts";
            cout << "\n\t7 Quit";
            cout << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
            cout << endl;
            switch (choice)
            {
            case 1:
            {
                cout << "==========OPEN ACCOUNT==========" << endl;
                cout << endl;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter initial balance: ";
                cin >> balance;
                acc = b.OpenAcc(name, balance);
                cout << endl << "Congratulations, your account is created: " << endl;
                cout << acc;
                break;
            }
            case 2:
            {
                cout << "==========BALANCE ENQUIRY==========" << endl;
                cout << endl;
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                acc = b.balanceEnquiry(accountNumber);
                cout << endl << "Your Account Details: " << endl;
                cout << acc;
                break;
            }
            case 3:
            {
                cout << "==========DEPOSIT AMOUNT==========" << endl;
                cout << endl;
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Balance: ";
                cin >> amount;
                acc = b.Deposit(accountNumber, amount);
                cout << endl << "Amount is Deposited" << endl;
                cout << acc;
                break;
            }
            case 4:
            {
                cout << "==========WITHDRAW AMOUNT==========" << endl;
                cout << endl;
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Balance: ";
                cin >> amount;
                acc = b.Withdraw(accountNumber, amount);
                cout << endl << "Amount Withdrawn" << endl;
                cout << acc;
                break;
            }
            case 5:
            {
                cout << "==========CLOSE ACCOUNT==========" << endl;
                cout << endl;
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.CloseAcc(accountNumber);
                cout << endl << "Account is Closed" << endl;
                break;
            }
            case 6:
            {
                cout << "==========ALL ACCOUNTS==========" << endl;
                cout << endl;
                b.ShowAllAcc();
                break;
            }
            case 7:
            {
                break;
            }
            default:
            {
                cout << "Enter the correct choice";
                exit(0);
            }
            }

        } while (choice != 7);
    }

    if (c1 == 1 && find(Manager, user_name, password))
    {
        cout << "<< BANKING SYSTEM by SMJI >>" << endl;

        do
        {
            cout << "\n\tSelect one option below ";
            cout << "\n\t1 Open an Account";
            cout << "\n\t2 Close an Account";
            cout << "\n\t7 Quit";
            cout << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
            cout << endl;
            switch (choice)
            {
            case 1:
            {
                cout << "==========OPEN ACCOUNT==========" << endl;
                cout << endl;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter initial balance: ";
                cin >> balance;
                acc = b.OpenAcc(name, balance);
                cout << endl << "Congratulations, your account is created: " << endl;
                cout << acc;
                break;
            }

            case 2:
            {
                cout << "==========CLOSE ACCOUNT==========" << endl;
                cout << endl;
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.CloseAcc(accountNumber);
                cout << endl << "Account is Closed" << endl;
                break;
            }
            default:
            {
                cout << "Enter the correct choice";
                exit(0);
            }
            }

        } while (choice != 7);

    }
    return 0;
}