#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

const int MAX = 100;

enum class MainMenu { SORT = 1, SEARCH, BALANCE_RANGE, QUIT };
enum class SortField { FIRST_NAME = 1, LAST_NAME, ACCOUNT_ID, AVAILABLE_BALANCE, PRESENT_BALANCE, SSN, EMAIL, BACK };

class BankAccount {
public:
    string firstName, lastName, accountID, ssn, email;
    double availableBalance, presentBalance;

    void print() const {
        cout << "Name: " << firstName << " " << lastName << endl;
        cout << "Account ID: " << accountID << endl;
        cout << "Available Balance: $" << availableBalance << endl;
        cout << "Present Balance: $" << presentBalance << endl;
        cout << "SSN: " << ssn << endl;
        cout << "Email: " << email << endl;
        cout << "-----------------------------" << endl;
    }
};

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void loadRecords(BankAccount accounts[], BankAccount* pAccounts[], int& count) {
    ifstream inFile("accounts.txt");
    count = 0;
    while (inFile >> accounts[count].firstName >> accounts[count].lastName >> accounts[count].accountID
        >> accounts[count].availableBalance >> accounts[count].presentBalance
        >> accounts[count].ssn >> accounts[count].email) {
        pAccounts[count] = &accounts[count];
        count++;
    }
    inFile.close();
}

void sortAccounts(BankAccount* pAccounts[], int count, SortField field) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            bool toSwap = false;
            switch (field) {
            case SortField::FIRST_NAME:
                toSwap = pAccounts[j]->firstName > pAccounts[j + 1]->firstName;
                break;
            case SortField::LAST_NAME:
                toSwap = pAccounts[j]->lastName > pAccounts[j + 1]->lastName;
                break;
            case SortField::ACCOUNT_ID:
                toSwap = pAccounts[j]->accountID > pAccounts[j + 1]->accountID;
                break;
            case SortField::AVAILABLE_BALANCE:
                toSwap = pAccounts[j]->availableBalance > pAccounts[j + 1]->availableBalance;
                break;
            case SortField::PRESENT_BALANCE:
                toSwap = pAccounts[j]->presentBalance > pAccounts[j + 1]->presentBalance;
                break;
            case SortField::SSN:
                toSwap = pAccounts[j]->ssn > pAccounts[j + 1]->ssn;
                break;
            case SortField::EMAIL:
                toSwap = pAccounts[j]->email > pAccounts[j + 1]->email;
                break;
            default:
                break;
            }
            if (toSwap) {
                BankAccount* temp = pAccounts[j];
                pAccounts[j] = pAccounts[j + 1];
                pAccounts[j + 1] = temp;
            }
        }
    }
}

void sortMenu(BankAccount* pAccounts[], int count) {
    int choice;
    do {
        cout << "\nSort by:\n1. First Name\n2. Last Name\n3. Account ID\n4. Available Balance\n5. Present Balance\n6. SSN\n7. Email\n8. Back to Main\nChoice: ";
        cin >> choice;
        SortField field = static_cast<SortField>(choice);
        if (field == SortField::BACK) break;
        sortAccounts(pAccounts, count, field);
        for (int i = 0; i < count; ++i)
            pAccounts[i]->print();
    } while (true);
}

void searchAccounts(BankAccount accounts[], int count) {
    string query;
    cout << "\nEnter search term (ID, name, email, SSN): ";
    cin >> query;
    query = toLower(query);
    bool found = false;

    for (int i = 0; i < count; ++i) {
        string fields[] = {
            toLower(accounts[i].accountID),
            toLower(accounts[i].firstName),
            toLower(accounts[i].lastName),
            toLower(accounts[i].email),
            toLower(accounts[i].ssn)
        };
        for (const string& field : fields) {
            if (field.find(query) != string::npos) {
                accounts[i].print();
                found = true;
                break;
            }
        }
    }
    if (!found) cout << "No matching records found.\n";
}

void searchByBalance(BankAccount accounts[], int count) {
    double target;
    cout << "\nEnter target balance: ";
    cin >> target;
    double lower = target * 0.8;
    double upper = target * 1.2;
    cout << "Searching for balances between $" << lower << " and $" << upper << endl;

    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (accounts[i].availableBalance >= lower && accounts[i].availableBalance <= upper) {
            accounts[i].print();
            found = true;
        }
    }
    if (!found) cout << "No records found in balance range.\n";
}

int main() {
    BankAccount accounts[MAX];
    BankAccount* pAccounts[MAX];
    int count = 0;

    loadRecords(accounts, pAccounts, count);

    int choice;
    do {
        cout << "\nMain Menu:\n1. Sort\n2. Search\n3. Search by Balance Range\n4. Quit\nChoice: ";
        cin >> choice;
        MainMenu menu = static_cast<MainMenu>(choice);
        switch (menu) {
        case MainMenu::SORT:
            sortMenu(pAccounts, count);
            break;
        case MainMenu::SEARCH:
            searchAccounts(accounts, count);
            break;
        case MainMenu::BALANCE_RANGE:
            searchByBalance(accounts, count);
            break;
        case MainMenu::QUIT:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}