#include <iostream>
#include <fstream>      
#include <string>       
#include <vector>      
#include <limits> 
using namespace std;     
class User {
public:
    string username;
     string hashedPassword;
      User(const string& user, const string& hashedPass)
        : username(user), hashedPassword(hashedPass) {}
    static User readFromFile(ifstream& ifs) {
        string user, pass;
        ifs >> user >> pass;
        return User(user, pass);
    }
};
string hashPassword(const string& password) {
    return "--" + password + "...."; 
}
class UserManager {
private:
    string userDataFile;
    vector<User> users; 
    void loadUsers() {
        users.clear(); 
        ifstream infile(userDataFile);
        if (infile.is_open()) {
            string user_temp, pass_temp;
            while (infile >> user_temp >> pass_temp) {
                users.emplace_back(user_temp, pass_temp);
            }
            infile.close();
        } else {
            cout << "User data file '" << userDataFile << "' not found or could not be opened. A new one will be created." << endl;
        }
    }
    void saveUsers() const {
        ofstream outfile(userDataFile); 
        if (outfile.is_open()) {
            for (const auto& user : users) {
                outfile << user.username << " " << user.hashedPassword << endl;
            }
            outfile.close();
        } else {
            cout << "Error: Could not open user data file '" << userDataFile << "' for writing." << endl;
        }
    }
public:
    UserManager(const string& filename) : userDataFile(filename) {
        loadUsers();  
    }
    ~UserManager() {
        saveUsers(); 
    }
    bool registerUser(const string& username, const string& password) {
        if (username.empty() || password.empty()) {
            cout << "Error: Username and password cannot be empty." << endl;
            return false;
        }
        if (username.length() < 8) {
            cout << "Error: Username must be at least 3 characters long." << endl;
            return false;
        }
        if (password.length() < 6) {
            cout << "Error: Password must be at least 6 characters long." <<endl;
            return false;
        }
        for (const auto& user : users) {
            if (user.username == username) {
                cout << "Error: Username '" << username << "' already exists. Please choose a different one." <<endl;
                return false;
            }
        }
        string hashedPassword = hashPassword(password);
        users.emplace_back(username, hashedPassword);
        saveUsers();
        return true;
    }
    bool loginUser(const string& username, const string& password) {
        if (username.empty() || password.empty()) {
            cout << "Error: Username and password cannot be empty for login." << endl;
            return false;
        }
        string hashedInputPassword = hashPassword(password);
        for (const auto& user : users) {
            if (user.username == username) {
                if (user.hashedPassword == hashedInputPassword) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return false;
    }
};
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
int main() {
    UserManager userManager("user_credentials.txt");
    int choice;
    string username;
    string password;
    do {
        cout << "\n--- User Management System ---" <<endl;
        cout << "1. Register" <<endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" <<endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." <<endl;
            clearInputBuffer(); 
            choice = 0; 
            continue;
        }
        clearInputBuffer(); 
        switch (choice) {
            case 1:
                cout << "Enter desired username: ";
                getline(cin,username);
                cout << "Enter desired password: ";
                getline(cin, password);
                if (userManager.registerUser(username, password)) {
                    cout << "Registration successful!" <<endl;
                } else {
                }
                break;
            case 2:
                cout << "Enter username: ";
                getline(cin, username);
                 cout << "Enter password: ";
                getline(cin, password);
                if (userManager.loginUser(username, password)) {
                    cout << "Login successful! Welcome, " << username << "!" <<endl;
                } else {
                    cout << "Login failed. Invalid username or password." <<endl;
                }
                break;
            case 3:
                cout << "Exiting program. Goodbye!" <<endl;
                break;
            default:
                cout << "Invalid choice. Please try again." <<endl;
                break;
        }
    } while (choice != 3);

    return 0;
}