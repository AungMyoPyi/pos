#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

void tab(int numberOfTabs) {
    for (int i = 0; i < numberOfTabs; i++) {
        cout << "\t";
    }
}

void equalSign(int numberOfEqualSign) {
    for (int j = 0; j < numberOfEqualSign; j++) {
        cout << "=";
    }
}

void dashSign(int numberOfDash) {
    for (int k = 0; k < numberOfDash; k++) {
        cout << "-";
    }
}

string toLowerCase(const string &str) {
    string lowerStr = str;
    for (size_t i = 0; i < lowerStr.length(); ++i) {
        lowerStr[i] = tolower(lowerStr[i]);
    }
    return lowerStr;
}

bool isValidPassword(const string &password) {
    if (password.length() < 8) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

class Product {
public:
    string name;
    double price;
    int quantity;

    Product(string name, double price, int quantity) : name(name), price(price), quantity(quantity) {}

    string getName() const { return toLowerCase(name); }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    void displayProduct() const {
        cout << "\n\t\t\t\t\t" << setw(26) << left << name << setw(5) << right << price << setw(26) << right << quantity << endl;
    }
};

class Inventory {
    vector<Product> products;

public:
    bool isEmpty() const {
        return products.empty(); // Returns true if the vector is empty
    }

    bool isProductInInventory(const string& productName) {
        return any_of(products.begin(), products.end(), [&](const Product& product) {
            return product.name == productName;
        });
    }

    bool productExists(const string &name) const {
        for (const auto &product : products) {
            if (product.getName() == name) {
                return true;
            }
        }
        return false;
    }


    void addProduct(const Product &product) {
        products.push_back(product);
        saveInventoryToFile();
    }

    void updateProductQuantity(const string &name, int quantity, bool showMessage = true) {
        for (auto &product : products) {
            if (product.getName() == name) {
                product.setQuantity(quantity);
                if (showMessage) {
                    tab(6);
                    dashSign(46);
                    cout << "\n\t\t\t\t\t\tProduct quantity updated successfully!" << endl;
                    tab(6);
                    dashSign(46);
                }
                saveInventoryToFile();
                return;
            }
        }

    }

    void updateProductPrice(const string &name, double newPrice) {
        for (auto &product : products) {
            if (product.getName() == name) {
                product.setPrice(newPrice);
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tProduct price updated successfully!" << endl;
                tab(6);
                dashSign(46);
                saveInventoryToFile();
                return;
            }
        }
    }

    void deleteProduct(const string &name) {
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].getName() == name) {
                products.erase(products.begin() + i);
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tProduct deleted successfully!" << endl;
                tab(6);
                dashSign(46);
                saveInventoryToFile(); 
                return;
            }
        }
    }

    Product* searchProduct(const string &name) {
        for (auto &product : products) {
            if (product.getName() == name) {
                return &product;
            }
        }
        return nullptr;
    }

    void saveInventoryToFile() {
        ofstream file("inventory.txt");

        if (!file) {
            cerr << "\n\t\t\t\t\t\tUnable to open inventory.txt for writing" << endl;
            return;
        }

        for (const auto& product : products) {
            file << "Name: " << product.getName() << ", Price: " << product.getPrice() << ", Quantity: " << product.getQuantity() << endl;
        }

        file.close();
    }


    void loadInventoryFromFile() {
        ifstream file("inventory.txt");
        if (!file) {
            cerr << "\n\t\t\t\t\t\tUnable to open inventory.txt for reading" << endl;
            return;
        }

        products.clear();
        string line;
        while (getline(file, line)) {
            string name, priceStr, quantityStr;
            double price = 0.0;
            int quantity = 0;

            size_t namePos = line.find("Name: ");
            size_t pricePos = line.find(", Price: ");
            size_t quantityPos = line.find(", Quantity: ");

            if (namePos == string::npos || pricePos == string::npos || quantityPos == string::npos) {
                cerr << "\n\t\t\t\t\t\tInvalid format in line: " << line << endl;
                continue;
            }

            name = line.substr(namePos + 6, pricePos - (namePos + 6));
            priceStr = line.substr(pricePos + 9, quantityPos - (pricePos + 9));
            quantityStr = line.substr(quantityPos + 12);

            try {
                price = stod(priceStr);
                quantity = stoi(quantityStr);
            } catch (const invalid_argument &e) {
                cerr << "\n\t\t\t\t\t\tError converting data: " << e.what() << endl;
                continue;
            } catch (const out_of_range &e) {
                cerr << "\n\t\t\t\t\t\tData out of range: " << e.what() << endl;
                continue;
            }

            products.emplace_back(name, price, quantity);
        }
        file.close();
    }

    string trim(const string &str) {
        auto start = str.find_first_not_of(" \t\n\r");
        auto end = str.find_last_not_of(" \t\n\r");
        return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
    }
 
    void displayInventory() const {
        if (products.empty()) {  // Check if the inventory is empty
            cout << "\t\t\t\t\t\tInventory is empty!" << endl;
            tab(6);
            dashSign(46);
            return;
        } else {
            cout << "\n\n\n";
            tab(4);
            dashSign(80);
            cout << "\n\t\t\t\t\t" << setw(23) << left << "Product Name:" << setw(26) << left << "Product Price($):" << setw(25) << left << "Available Stock:" << endl;
            tab(4);
            dashSign(80);
            for (const auto &product : products) {
                product.displayProduct();
            }
            tab(4);
            dashSign(80);
        }
    }
};

class User {
    string username;
    string password;
    bool isAdmin;

public:
    User(string username, string password, bool isAdmin) : username(username), password(password), isAdmin(isAdmin) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    bool getIsAdmin() const { return isAdmin; }

    bool authenticate(string password) const {
        return this->password == password;
    }

    void setUsername(const string &newUsername) { username = newUsername; }
    void setPassword(const string &newPassword) { password = newPassword; }
};

class UserManager {
    vector<User> users;
    vector<pair<string, string>> subAdminAccounts;
    int subAdminCount = 0;

public:
    UserManager() {
        // Default admin account
        users.push_back(User("admin", "Admin@123", true));
    }

    vector<pair<string, string>> getSubAdminAccounts() const {
        return subAdminAccounts;
    }

    void registerUser(const string &username, const string &password, bool isAdmin = false) {
        if (!isUsernameAvailable(username)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tUsername already exists!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        if (isAdmin) {
            if (!canAddAdmin()) {
                tab(6);
                dashSign(46);
                cout << "\t\t\t\t\t\tCannot add more sub-admin accounts. \n\t\t\t\t\t\tMaximum limit reached." << endl;
                tab(6);
                dashSign(46);
                return;
            }
            addSubAdminCore(username, password);
            subAdminCount++;
        } else {
            users.push_back(User(username, password, isAdmin));
        }  
    }

    User* authenticateUser(const string &username, const string &password) {
        for (auto &user : users) {
            if (user.getUsername() == username && user.authenticate(password)) {
                return &user;
            }
        }
        return nullptr;
    }

    void deleteUser(const string &username) {
        if (username == "admin") {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tCannot delete the default admin!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i].getUsername() == username) {
                if (users[i].getIsAdmin()) {
                    deleteSubAdmin(username);
                    subAdminCount--;
                }
                users.erase(users.begin() + i);
                return;
            }
        }
        tab(6);
        dashSign(46);
        cout << "\n\t\t\t\t\t\tUser not found!" << endl;
        tab(6);
        dashSign(46);
    }

    void loadSubAdmins() {
        ifstream file("sub_admins.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string username, password;
            getline(ss, username, ',');
            getline(ss, password);
            if (isUsernameAvailable(username)) {
                subAdminAccounts.push_back(make_pair(username, password));
                users.push_back(User(username, password, true)); // Add to users list
                subAdminCount++;
            }
        }

        file.close();
    }

    void displayUsers() const {
        cout << "\n\t\t\t\t\t\tCustomers:\n";
        for (const auto& user : users) {
            if (!user.getIsAdmin()) { 
                cout << "\n\t\t\t\t\t\tUsername: " << user.getUsername() << ", Role: Customer" << endl;
            }
        }
        tab(6);
        dashSign(46);

        cout << "\n\n\t\t\t\t\t\tSub-Admins:\n";
        if (!subAdminAccounts.empty()) {
            for (const auto& subAdmin : subAdminAccounts) {
                cout << "\n\t\t\t\t\t\tUsername: " << subAdmin.first << ", Role: Sub-Admin" << endl;
            }
        } 
        tab(6);
        dashSign(46);
    }


    bool addSubAdminCore(const string& username, const string& password) {
        // Ensure that duplicate entries are not added
        if (!isUsernameAvailable(username)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin already exists!" << endl;
            tab(6);
            dashSign(46);
            return false;
        }
        
        // Add sub-admin to in-memory structures
        subAdminAccounts.push_back(make_pair(username, password));
        subAdminCount++;

        // Append to file
        ofstream file("sub_admins.txt", ios::app);
        if (file.is_open()) {
            file << username << "," << password << endl;
            file.close();
        } else {
            cout << "\n\t\t\t\t\t\tUnable to open file for writing." << endl;
            return false;
        }

        return true;
    }

    bool canAddAdmin() const {
        // Limit total admin count including sub-admins to 4 (1 default admin + 3 sub-admins)
        return subAdminCount < 4; // Adjusted to allow only 3 sub-admins
    }

    void deleteSubAdmin(const string& username) {
        auto it = remove_if(subAdminAccounts.begin(), subAdminAccounts.end(), [&username](const auto& admin) {
            return admin.first == username;
        });

        if (it != subAdminAccounts.end()) {
            subAdminAccounts.erase(it, subAdminAccounts.end());
            subAdminCount--;

            // Remove from users list
            auto userIt = remove_if(users.begin(), users.end(), [&username](const auto& user) {
                return user.getUsername() == username;
            });

            if (userIt != users.end()) {
                users.erase(userIt, users.end());
            }

            // Remove from the file
            ifstream file("sub_admins.txt");
            ofstream tempFile("temp.txt");
            string line;

            while (getline(file, line)) {
                if (line.substr(0, line.find(',')) != username) {
                    tempFile << line << endl;
                }
            }

            file.close();
            tempFile.close();

            remove("sub_admins.txt");
            rename("temp.txt", "sub_admins.txt");

            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin deleted successfully!" << endl;
            tab(6);
            dashSign(46);
        } else {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin not found." << endl;
            tab(6);
            dashSign(46);
        }
    }

    bool isUsernameAvailable(const string &username) const {
        for (const auto &user : users) {
            if (user.getUsername() == username) {
                return false;
            }
        }
        return true;
    }
};

class Transaction {
    vector<int> quantities;

public:
    vector<Product> cart;
    double totalAmount = 0.0;

    void addToCart(Product &product, int quantity, Inventory &inventory) {
        if (product.getQuantity() < quantity) {
            cout << "\n\t\t\t\t\t\tNot enough stock available!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        // Add to cart
        cart.push_back(Product(product.getName(), product.getPrice(), quantity));
        totalAmount += product.getPrice() * quantity;

        // Update inventory only once
        product.setQuantity(product.getQuantity() - quantity);
        inventory.updateProductQuantity(product.getName(), product.getQuantity(), false);  // This saves the inventory to file
    }


    void displayCart() const {
        if (cart.empty() || totalAmount == 0) {
            cout << "\n\t\t\t\t\t\tThe cart is empty!\n" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        for (const auto &product : cart) {
            cout << "\n\t\t\t\t\t\tName: " << product.getName() << ", Price: $" << product.getPrice() << ", Quantity: " << product.getQuantity() << endl;
            tab(6);
            dashSign(46);
        }
        cout << "\n\t\t\t\t\t\tTotal: $" << totalAmount << endl;
        tab(6);
        dashSign(46);
    }

    void removeItemFromCart(Inventory &inventory) {
        // Check if the cart is empty
        if (cart.empty()) {
            cout << "\t\t\t\t\t\tThe cart is empty!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        string name;
        int quantity;

        cout << "\n\t\t\t\t\t\tREMOVE PRODUCT FROM CART:";
        cout << "\n\t\t\t\t\t\tEnter product name: ";
        cin.ignore();
        getline(cin, name);

        string lowerName = toLowerCase(name);

        for (size_t i = 0; i < cart.size(); ++i) {
            if (cart[i].getName() == lowerName) {
                
                do {
                    cout << "\t\t\t\t\t\tEnter quantity to remove: ";
                    cin >> quantity;

                    if (quantity > cart[i].getQuantity()) {
                        tab(6);
                        dashSign(46);
                        cout << "\n\t\t\t\t\t\tCannot remove more than the quantity in the cart!" << endl;
                        cout << "\n\t\t\t\t\t\tYou have only '" << cart[i].getQuantity() << "' in the cart!" << endl;
                        tab(6);
                        dashSign(46);
                        cout << "\n";
                    }
                } while (quantity > cart[i].getQuantity());

                // Update the total amount
                totalAmount -= cart[i].getPrice() * quantity;
                cart[i].setQuantity(cart[i].getQuantity() - quantity);

                // If the cart item quantity is now zero, remove it from the cart
                if (cart[i].getQuantity() == 0) {
                    cart.erase(cart.begin() + i);
                }

                // Update inventory to add back the removed quantity
                Product* product = inventory.searchProduct(lowerName);
                if (product) {
                    product->setQuantity(product->getQuantity() + quantity);
                    inventory.updateProductQuantity(product->getName(), product->getQuantity(), false);  // This already calls saveInventoryToFile()
                }

                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tProduct removed from cart." << endl;
                tab(6);
                dashSign(46);
                return;
            }
        }
        tab(6);
        dashSign(46);
        cout << "\n\t\t\t\t\t\tProduct not found in cart!" << endl;
        tab(6);
        dashSign(46);
    }

    void processSale() {
        if (cart.empty()) {
            cout << "\t\t\t\t\t\tYour cart is empty." << endl;
            tab(6);
            dashSign(46);
            return; // Exit the function if the cart is empty
        }

        int paymentMethod;
        double cashInput;

        cout << "\n\t\t\t\t\t\tCHECKOUT:";
        cout << "\n\n\t\t\t\t\t\tChoose Payment Method:\n\n\t\t\t\t\t\t1. Cash\n\t\t\t\t\t\t2. Card\n\t\t\t\t\t\t3. Digital Pay " << endl;
        tab(6);
        dashSign(46);

        cout << "\n\t\t\t\t\t\tEnter your choice: ";
        cin >> paymentMethod;
        tab(6);
        dashSign(46);

        switch (paymentMethod) {
            case 1:
                cout << "\n\t\t\t\t\t\tCash payment selected. \n\t\t\t\t\t\tPlease provide the cash amount." << endl;
                tab(6);
                dashSign(46);

                // Keep asking for the correct amount until the user enters the exact total
                cout << "\n\t\t\t\t\t\tTotal amount: $" << totalAmount << endl;
                do {
                    cout << "\n\t\t\t\t\t\tEnter cash amount: $";
                    cin >> cashInput;
                    tab(6);
                    dashSign(46);

                    if (cashInput < totalAmount) {
                        cout << "\n\t\t\t\t\t\tInsufficient cash. \n\t\t\t\t\t\tPlease provide the correct amount." << endl;
                        tab(6);
                        dashSign(46);
                    } 

                    if (cashInput >= totalAmount) {
                        double change = cashInput - totalAmount;
                        cout << "\n\t\t\t\t\t\tPayment successful... \n\t\t\t\t\t\tChange: $" << change << endl;
                        tab(6);
                        dashSign(46);
                    }
                } while (cashInput < totalAmount); 

                break;

            case 2:
                cout << "\n\t\t\t\t\t\tCard payment selected. \n\t\t\t\t\t\tProcessing card payment..." << endl;
                tab(6);
                dashSign(46);
                break;
            case 3:
                cout << "\n\t\t\t\t\t\tOnline payment selected. \n\t\t\t\t\t\tRedirecting to payment gateway..." << endl;
                tab(6);
                dashSign(46);
                break;
            default:
                cout << "\n\t\t\t\t\t\tInvalid payment method selected. Please try again." << endl;
                tab(6);
                dashSign(46);
                return;
        }

        cout << "\n\t\t\t\t\t\tProcessing sale..." << endl;
        tab(6);
        dashSign(46);

        ofstream receipt("receipt.txt");
        receipt << "Receipt:\n";
        for (const auto &product : cart) {
            receipt << "Product: " << product.getName() << ", Quantity: " << product.getQuantity() << ", Price: $" << product.getPrice() << endl;
        }
        receipt << "\nTotal: $" << totalAmount << endl;
        receipt.close();
        cout << "\n\t\t\t\t\t\tReceipt saved to receipt.txt" << endl;
        tab(6);
        dashSign(46);
        cart.clear();
        quantities.clear();
        totalAmount = 0.0;
    }
};

class POS {
    Inventory inventory;
    UserManager userManager;  
    Transaction transaction;
    User* currentUser = nullptr;
    vector<pair<string, string>> subAdminAccounts;

public:
    POS() {
        inventory.loadInventoryFromFile();
    }

    ~POS() {
        inventory.saveInventoryToFile();
    }

    void run() {
        int choice;

        tab(6);
        equalSign(46);
        cout << "\n\t\t\t\t\t\t\t  WELCOME TO THE POS SYSTEM \n\t\t\t\t\t\t\t\t Created by\n\t\t\t\t\t\t       DYNAMIC PENTAGRAM (C++ Group-IV)\n";
        tab(6);
        equalSign(46);
        
        while (true) {
            cout << "\n\n\n\n";
            tab(6);
            equalSign(46);
            cout << "\n\t\t\t\t\t\t\t        POS Main Menu:\n";
            tab(6);
            equalSign(46);
            cout << "\n\n";

            cout << "\t\t\t\t\t\t1. Login\n";
            cout << "\t\t\t\t\t\t2. Register\n";
            cout << "\t\t\t\t\t\t3. Exit\n";

            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tEnter your choice: ";
            cin >> choice;
            tab(6);
            dashSign(46);
            cout << "\n";

            switch (choice) {
                case 1: login(); break;
                case 2: registerUser(); break;
                case 3: return;
                default: cout << "\t\t\t\t\t\tInvalid choice! Please try again." << endl;
                tab(6);
                dashSign(46);
            }
        }
    }

private:
    void login() {
        userManager.loadSubAdmins();

        string username, password;
        cout << "\n\t\t\t\t\t\tLOG IN:\n";
        cout << "\t\t\t\t\t\tEnter username: ";
        cin.ignore();
        getline(cin, username);
        cout << "\t\t\t\t\t\tEnter password: ";
        getline(cin, password);

        User* user = userManager.authenticateUser(username, password);
        if (user) {
            currentUser = user;
            if (user->getIsAdmin()) {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tLogged in successfully....." << endl;
                tab(6);
                dashSign(46);
                adminMenu();
            } else {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tLogged in successfully....." << endl;
                tab(6);
                dashSign(46);
                customerMenu();
            }
        } else {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tInvalid username or password!" << endl;
            tab(6);
            dashSign(46);
        }
    }

    void registerUser() {
        string username, password, confirmPassword;

        cout << "\n\n\t\t\t\t\t\tREGISTER:";
        cout << "\n\t\t\t\t\t\tEnter username: ";
        cin.ignore();
        getline(cin, username);

        if (!userManager.isUsernameAvailable(username)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tUsername is already taken. \n\t\t\t\t\t\tPlease choose another one." << endl;
            tab(6);
            dashSign(46);
            return;
        }

        do {
            cout << "\t\t\t\t\t\tEnter password: ";
            getline(cin, password);

            if (!isValidPassword(password)) {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tPassword must be at least 8 characters long and \n\t\t\t\t\t\tinclude at least one uppercase letter, one lowercase letter, \n\t\t\t\t\t\tone digit, and one special character." << endl;
                tab(6);
                dashSign(46);
                cout << "\n";
            } else {
                break;
            }
        } while (true);

        do {
            // Confirm password
            cout << "\t\t\t\t\t\tConfirm password: ";
            getline(cin, confirmPassword);

            if (password != confirmPassword) {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tPasswords do not match. Please try again." << endl;
                tab(6);
                dashSign(46);
                cout << "\n";
            }
        } while (password != confirmPassword);

        // Register as customer only
        tab(6);
        dashSign(46);
        cout << "\n\t\t\t\t\t\tCustomer registered successfully....." << endl;
        tab(6);
        dashSign(46);
        userManager.registerUser(username, password, false);
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\n\n\n\n";
            tab(6);
            equalSign(46);
            cout << "\n\t\t\t\t\t\t\t\t ADMIN MENU:\n";
            tab(6);
            equalSign(46);
            cout << "\n\n";
            cout << "\t\t\t\t\t\t1. Add Product\n";
            cout << "\t\t\t\t\t\t2. Update Product Price\n";
            cout << "\t\t\t\t\t\t3. Update Product Quantity\n";
            cout << "\t\t\t\t\t\t4. Delete Product\n";
            cout << "\t\t\t\t\t\t5. Display Inventory\n"; 
            cout << "\t\t\t\t\t\t6. Add Sub-Admin\n";
            cout << "\t\t\t\t\t\t7. Delete Sub-Admin\n";
            cout << "\t\t\t\t\t\t8. Display Users\n";
            cout << "\t\t\t\t\t\t9. Logout\n";

            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tEnter your choice: ";
            cin >> choice;
            tab(6);
            dashSign(46);
            cout << "\n";

            if (choice == 6 || choice == 7) {
                if (currentUser->getUsername() != "admin") {
                    cout << "\t\t\t\t\t\tInvalid choice! \n\t\t\t\t\t\tYou do not have permission to complete that action." << endl;
                    tab(6);
                    dashSign(46);
                    cout << "\n\n\n";
                    continue;
                }
            }

            switch (choice) {
                case 1: addProduct(); break;
                case 2: updateProduct(); break;
                case 3: updateProductQuantity(); break;
                case 4: deleteProduct(); break;
                case 5: inventory.displayInventory(); break;
                case 6: if (currentUser->getUsername() == "admin") addSubAdminInteractive(); break;
                case 7: if (currentUser->getUsername() == "admin") deleteSubAdmin(); break;
                case 8: userManager.displayUsers(); break;
                case 9: currentUser = nullptr; break;
                tab(6);
                dashSign(46);
                default: cout << "\t\t\t\t\t\tInvalid choice! Please try again." << endl;
                tab(6);
                dashSign(46);
                cout << "\n\n\n";
            }
        } while (choice != 9);
    }

    void customerMenu() {
        int choice;
        do {
            cout << "\n\n\n\n";
            tab(6);
            equalSign(46);
            cout << "\n\t\t\t\t\t\t\t\tCUSTOMER MENU:\n";
            tab(6);
            equalSign(46);
            cout << "\n\n";
            cout << "\t\t\t\t\t\t1. Display Products\n";
            cout << "\t\t\t\t\t\t2. Add Product to Cart\n";
            cout << "\t\t\t\t\t\t3. Remove Product from Cart\n";
            cout << "\t\t\t\t\t\t4. View Cart\n";
            cout << "\t\t\t\t\t\t5. Checkout\n";
            cout << "\t\t\t\t\t\t6. Logout\n";
            
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tEnter your choice: ";
            cin >> choice;
            tab(6);
            dashSign(46);
            cout << "\n";

            switch (choice) {
                case 1: inventory.displayInventory(); break;
                case 2: addProductToCart(); break;
                case 3: removeProductFromCart(); break;
                case 4: transaction.displayCart(); break;
                case 5: transaction.processSale(); break;
                case 6: currentUser = nullptr; break;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 6);
    }

    void addProductToCart() {
        // Check if the inventory is empty
        if (inventory.isEmpty()) {
            cout << "\t\t\t\t\t\tThe products are currently out of stock \n\t\t\t\t\t\tand will be available soon... \n\n\t\t\t\t\t\tPlease come back later." << endl;
            tab(6);
            dashSign(46);
            return; // Exit the function if the inventory is empty
        }

        string name;
        int quantity;

        cout << "\n\t\t\t\t\t\tADD PRODUCT TO CART:";
        cout << "\n\t\t\t\t\t\tEnter product name: ";
        cin.ignore();
        getline(cin, name);

        string lowerProductName = toLowerCase(name);

        // Search for the product in the inventory
        Product* product = inventory.searchProduct(lowerProductName);

        // Check if the product exists in the inventory and has stock
        if (!product) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tProduct not found in inventory!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        if (product->getQuantity() == 0) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tThe product is currently out of stock!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        // Check if the product is already in the cart
        bool productFoundInCart = false;
        for (size_t i = 0; i < transaction.cart.size(); ++i) {
            if (transaction.cart[i].getName() == lowerProductName) {
                productFoundInCart = true;

                // Ask for the additional quantity
                int additionalQuantity;
                cout << "\n\t\t\t\t\t\tThe product is already in the cart with quantity: " << transaction.cart[i].getQuantity() << endl;
                tab(6);
                dashSign(46);
                cout << "\n";
                do {
                    cout << "\t\t\t\t\t\tEnter additional quantity: ";
                    cin >> additionalQuantity;

                    // Check if the total requested quantity is available
                    if (additionalQuantity > product->getQuantity()) {
                        tab(6);
                        dashSign(46);
                        cout << "\n\t\t\t\t\t\tInsufficient stock!" << endl;
                        cout << "\n\t\t\t\t\t\tThe product '" << product->getName() << "' is available only for '" 
                            << product->getQuantity() << "' more." << endl;
                    }

                    tab(6);
                    dashSign(46);
                    cout << "\n";
                } while (additionalQuantity > product->getQuantity());

                // Update the cart item with the additional quantity
                transaction.cart[i].setQuantity(transaction.cart[i].getQuantity() + additionalQuantity);
                transaction.totalAmount += product->getPrice() * additionalQuantity;

                // Update inventory
                product->setQuantity(product->getQuantity() - additionalQuantity);
                inventory.updateProductQuantity(product->getName(), product->getQuantity(), false);  // This saves the inventory to file

                cout << "\t\t\t\t\t\tProduct added successfully!" << endl;
                tab(6);
                dashSign(46);
                return;
            }
        }

        // If the product was not found in the cart, proceed to add it
        if (!productFoundInCart) {
            do {
                cout << "\t\t\t\t\t\tEnter quantity: ";
                cin >> quantity;

                // Check if the requested quantity is available
                if (quantity > product->getQuantity()) {
                    tab(6);
                    dashSign(46);
                    cout << "\n\t\t\t\t\t\tInsufficient stock!" << endl;
                    cout << "\n\t\t\t\t\t\tThe product '" << product->getName() << "' is available only for '" 
                        << product->getQuantity() << "'." << endl;
                    tab(6);
                    dashSign(46);
                    cout << "\n";
                }
            } while (quantity > product->getQuantity());

            // Add product to the cart using the Transaction's method
            transaction.addToCart(*product, quantity, inventory);
        }

        tab(6);
        dashSign(46);
        cout << "\n\t\t\t\t\t\tProduct added to cart." << endl;
        tab(6);
        dashSign(46);
    }

    void removeProductFromCart() {
        transaction.removeItemFromCart(inventory);
    }

    void addProduct() {
        string name;
        double price;
        int quantity;

        cout << "\n\t\t\t\t\t\tADD PRODUCT TO INVENTORY:";
        cout << "\n\t\t\t\t\t\tEnter product name: ";
        cin.ignore();
        getline(cin, name);

        string addLower = toLowerCase(name);

        if (inventory.isProductInInventory(addLower)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tThe product is already in the inventory. \n\t\t\t\t\t\tConsider updating price or quantity." << endl;
            tab(6);
            dashSign(46);
            return;
        }

        cout << "\t\t\t\t\t\tEnter price: ";
        cin >> price;

        cout << "\t\t\t\t\t\tEnter quantity: ";
        cin >> quantity;

        inventory.addProduct(Product(addLower, price, quantity));
        
        tab(6);
        dashSign(46);
        cout << "\n\t\t\t\t\t\tProduct added successfully!" << endl;
        tab(6);
        dashSign(46);
    }

    void updateProduct() {
        string name;
        double newPrice;

        cout << "\n\t\t\t\t\t\tUPDATE PRODUCT PRICE:";
        cout << "\n\t\t\t\t\t\tEnter product name to update price: ";
        cin.ignore();
        getline(cin, name);

        string lowerCase = toLowerCase(name);

        // Check if the product exists in the inventory
        if (!inventory.productExists(lowerCase)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tProduct not found!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        cout << "\t\t\t\t\t\tEnter new price: ";
        cin >> newPrice;

        inventory.updateProductPrice(lowerCase, newPrice);
    }

    void updateProductQuantity() {
        string name;
        int newQuantity;

        cout << "\n\t\t\t\t\t\tUPDATE PRODUCT QUANTITY:";
        cout << "\n\t\t\t\t\t\tEnter product name to update quantity: ";
        cin.ignore();
        getline(cin, name);

        string toLower = toLowerCase(name);

        // Check if the product exists in the inventory
        if (!inventory.productExists(toLower)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tProduct not found!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        cout << "\t\t\t\t\t\tEnter new quantity: ";
        cin >> newQuantity;

        inventory.updateProductQuantity(toLower, newQuantity, true);
    }

    void deleteProduct() {
        string name;

        cout << "\n\t\t\t\t\t\tDELETE PRODUCT:";
        cout << "\n\t\t\t\t\t\tEnter product name to delete: ";
        cin.ignore();
        getline(cin, name);

        string deleteLower = toLowerCase(name);

        if (!inventory.productExists(deleteLower)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tProduct not found!" << endl;
            tab(6);
            dashSign(46);
            return;
        }

        inventory.deleteProduct(deleteLower);
    }

  public:

    bool isUsernameAvailable(const string& username) {
        // Check in subAdminAccounts first
        bool isInSubAdmin = find_if(subAdminAccounts.begin(), subAdminAccounts.end(),
                        [&username](const auto& admin) { return admin.first == username; }) != subAdminAccounts.end();

        // Check in the users list via userManager
        bool isInUsers = !userManager.isUsernameAvailable(username);

        // Username is unavailable if it exists in either list
        return !(isInSubAdmin || isInUsers);
    }


    // User interaction version of addSubAdmin
    void addSubAdminInteractive() {
        if (userManager.getSubAdminAccounts().size() >= 3) {
            cout << "\t\t\t\t\t\tCannot add more sub-admin accounts. \n\t\t\t\t\t\tMaximum limit of 3 reached." << endl;
            tab(6);
            dashSign(46);
            return;
        }

        string username, password, confirmPassword;

        // Get username input
        cout << "\n\t\t\t\t\t\tREGISTER SUB-ADMIN:";
        cout << "\n\t\t\t\t\t\tEnter username for sub-admin: ";
        cin.ignore();
        getline(cin, username);

        // Check username availability
        if (!isUsernameAvailable(username)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tUsername already taken. Please choose another one." << endl;
            tab(6);
            dashSign(46);
            return;
        }

        // Get and validate password
        do {
            cout << "\t\t\t\t\t\tEnter password: ";
            getline(cin, password);

            if (!isValidPassword(password)) {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tPassword must be at least 8 characters long and \n\t\t\t\t\t\tinclude at least one uppercase letter, one lowercase letter, \n\t\t\t\t\t\tone digit, and one special character." << endl;
                tab(6);
                dashSign(46);
                cout << "\n";
            } else {
                break;
            }
        } while (true);

        do {
            // Confirm password
            cout << "\t\t\t\t\t\tConfirm password: ";
            getline(cin, confirmPassword);

            if (password != confirmPassword) {
                tab(6);
                dashSign(46);
                cout << "\n\t\t\t\t\t\tPasswords do not match. Please try again." << endl;
                tab(6);
                dashSign(46);
                cout << "\n";
            }
        } while (password != confirmPassword);

        // Call the core function to add the sub-admin
        if (userManager.addSubAdminCore(username, password)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin registered successfully..." << endl;
            tab(6);
            dashSign(46);
        }
    }

    void addSubAdminProgrammatic(const string& username, const string& password) {
        if (userManager.getSubAdminAccounts().size() >= 3) {
            cout << "\n\t\t\t\t\t\tCannot add more sub-admin accounts. \n\t\t\t\t\t\tMaximum limit of 3 reached." << endl;
            tab(6);
            dashSign(46);
            return;
        }

        if (userManager.addSubAdminCore(username, password)) {
            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin added successfully..." << endl;
            tab(6);
            dashSign(46);
        }
    }

    void deleteSubAdmin() {
        
        string username;
        
        // Your existing logic to delete a sub-admin account.
        cout << "\n\t\t\t\t\t\tDELETE SUB-ADMIN:";
        cout << "\n\t\t\t\t\t\tEnter sub-admin username to delete: ";
        cin.ignore();
        getline(cin, username);

        userManager.deleteSubAdmin(username);

        auto it = remove_if(subAdminAccounts.begin(), subAdminAccounts.end(), [&username](const auto& admin) {
            return admin.first == username;
        });

        if (it != subAdminAccounts.end()) {
            subAdminAccounts.erase(it, subAdminAccounts.end());

            // Remove the sub-admin from the text file.
            ifstream file("sub_admins.txt");
            ofstream tempFile("temp.txt");
            string line;

            while (getline(file, line)) {
                if (line.substr(0, line.find(" ")) != username) {
                    tempFile << line << endl;
                }
            }

            file.close();
            tempFile.close();

            // Replace the original file with the updated file.
            remove("sub_admins.txt");
            rename("temp.txt", "sub_admins.txt");

            tab(6);
            dashSign(46);
            cout << "\n\t\t\t\t\t\tSub-admin deleted successfully!" << endl;
            tab(6);
            dashSign(46);
        } 
    }
};

int main() {
    POS pos;
    pos.run();
    return 0;
}
