#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <set>
#include <limits>
#include <utility>

using namespace std;

// Class representing a menu item
class MenuItem {
public:
    int id;           // Unique identifier for the menu item
    string name;      // Name of the menu item
    string category;  // Category of the menu item (e.g., Vegetarian, Vegan)
    double price;     // Price of the menu item
    int stock;        // Quantity available in stock

    // Constructor to initialize a menu item
    MenuItem(int id, const string& name, const string& category, double price, int stock)
        : id(id), name(name), category(category), price(price), stock(stock) {}

    // Converts menu item details to a single CSV string
    string toString() const {
        stringstream ss;
        ss << id << "," << name << "," << category << ","
           << fixed << setprecision(2) << price << "," << stock;
        return ss.str();
    }

    // Displays the menu item details in a user-friendly format
    void display() const {
        cout << "ID: " << id << " | " << name << " (" << category << ") | $"
             << fixed << setprecision(2) << price << " | Available: " << stock << '\n';
    }
};

// Class representing a customer
class Customer {
public:
    int id;          // Unique identifier for the customer
    string name;     // Name of the customer
    string contact;  // Contact details of the customer

    // Constructor to initialize a customer
    Customer(int id, const string& name, const string& contact)
        : id(id), name(name), contact(contact) {}

    // Converts customer details to a single CSV string
    string toString() const {
        stringstream ss;
        ss << id << "," << name << "," << contact;
        return ss.str();
    }
};

// Main system class to manage the restaurant operations
class RestaurantSystem {
private:
    vector<MenuItem> menu;        // List of menu items
    vector<Customer> customers;   // List of customers
    string menuFile = "menu.txt";         // File to save menu data
    string customerFile = "customers.txt"; // File to save customer data

    // Loads menu items from the file
    void loadMenuFromFile() {
        ifstream file(menuFile);
        if (!file) {
            cout << "Menu file not found. Creating a new one...\n";
            createSampleMenu(); // Create a sample menu if the file is missing
            return;
        }
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, name, category, priceStr, stockStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, stockStr, ',');

            int id = stoi(idStr);
            double price = stod(priceStr);
            int stock = stoi(stockStr);
            menu.emplace_back(id, name, category, price, stock);
        }
        file.close();
    }

    // Saves menu items to the file
    void saveMenuToFile() {
        ofstream file(menuFile);
        for (const auto& item : menu) {
            file << item.toString() << '\n';
        }
        file.close();
    }

    // Loads customer data from the file
    void loadCustomersFromFile() {
        ifstream file(customerFile);
        if (!file) {
            cout << "Customer file not found. Creating a new one...\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, name, contact;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, contact, ',');

            int id = stoi(idStr);
            customers.emplace_back(id, name, contact);
        }
        file.close();
    }

    // Saves customer data to the file
    void saveCustomersToFile() {
        ofstream file(customerFile);
        for (const auto& customer : customers) {
            file << customer.toString() << '\n';
        }
        file.close();
    }

    // Creates a sample menu with predefined items
    void createSampleMenu() {
        menu = {
            {1, "Veggie Burger", "Vegetarian", 8.99, 25},
            {2, "Veggie Wrap", "Vegetarian", 7.49, 20},
            {3, "Grilled Cheese Sandwich", "Vegetarian", 5.99, 30},
            {4, "Vegetable Stir Fry", "Vegetarian", 9.49, 18},
            {5, "Vegan Buddha Bowl", "Vegan", 10.99, 15},
            {6, "Vegan Tacos", "Vegan", 8.49, 20},
            {7, "Vegan Quinoa Salad", "Vegan", 9.29, 12},
            {8, "Vegan Chili", "Vegan", 7.99, 25},
            {9, "Gluten-Free Margherita Pizza", "Gluten-Free", 11.49, 10},
            {10, "Grilled Chicken Salad (Gluten-Free)", "Gluten-Free", 10.99, 18}
        };
        saveMenuToFile();
        cout << "Sample menu created and saved to file.\n";
    }

    // Helper to flush leftover newline before getline
    static void flushLine() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    // Constructor initializes the system and loads data from files
    RestaurantSystem() {
        loadMenuFromFile();
        loadCustomersFromFile();
    }

    // Displays the entire menu grouped by category
    void displayMenu() const {
        cout << "\n=== CampusBites Cafeteria Menu ===\n";
        set<string> categories; // To store unique categories
        for (const auto& item : menu) {
            categories.insert(item.category);
        }
        // Display items category-wise
        for (const auto& category : categories) {
            cout << "\nCategory: " << category << '\n';
            for (const auto& item : menu) {
                if (item.category == category) {
                    item.display();
                }
            }
        }
        cout << '\n';
    }

    // Adds a new menu item
    void addMenuItem() {
        int id, stock;
        double price;
        string name, category;

        cout << "Enter Menu Item ID: ";
        cin >> id;
        flushLine();

        cout << "Enter Menu Item Name: ";
        getline(cin, name);

        cout << "Enter Menu Item Category: ";
        getline(cin, category);

        cout << "Enter Menu Item Price: ";
        cin >> price;

        cout << "Enter Menu Item Stock: ";
        cin >> stock;

        menu.emplace_back(id, name, category, price, stock);
        saveMenuToFile();
        cout << "Menu item added successfully and saved to file!\n";
    }

    // Updates an existing menu item
    void updateMenuItem() {
        int id, stock;
        double price;
        string name, category;

        cout << "Enter the ID of the Menu Item to Update: ";
        cin >> id;
        flushLine();

        for (auto& item : menu) {
            if (item.id == id) {
                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Category: ";
                getline(cin, category);
                cout << "Enter New Price: ";
                cin >> price;
                cout << "Enter New Stock: ";
                cin >> stock;

                item.name = name;
                item.category = category;
                item.price = price;
                item.stock = stock;

                saveMenuToFile();
                cout << "Menu item updated successfully and saved to file!\n";
                return;
            }
        }
        cout << "Menu item with ID " << id << " not found.\n";
    }

    // Removes a menu item by ID
    void removeMenuItem() {
        int id;
        cout << "Enter the ID of the Menu Item to Remove: ";
        cin >> id;

        for (size_t i = 0; i < menu.size(); ++i) {
            if (menu[i].id == id) {
                menu.erase(menu.begin() + static_cast<long>(i));
                saveMenuToFile();
                cout << "Menu item removed successfully and saved to file!\n";
                return;
            }
        }
        cout << "Menu item with ID " << id << " not found.\n";
    }

    // Adds a new customer
    void addCustomer() {
        int id;
        string name, contact;

        cout << "Enter Customer ID: ";
        cin >> id;
        flushLine();

        cout << "Enter Customer Name: ";
        getline(cin, name);

        cout << "Enter Customer Contact: ";
        getline(cin, contact);

        customers.emplace_back(id, name, contact);
        saveCustomersToFile();
        cout << "Customer added successfully and saved to file!\n";
    }

    // Removes a customer by ID
    void removeCustomer() {
        int id;
        cout << "Enter the ID of the Customer to Remove: ";
        cin >> id;

        for (size_t i = 0; i < customers.size(); ++i) {
            if (customers[i].id == id) {
                customers.erase(customers.begin() + static_cast<long>(i));
                saveCustomersToFile();
                cout << "Customer removed successfully and saved to file!\n";
                return;
            }
        }
        cout << "Customer with ID " << id << " not found.\n";
    }

    // Allows customers to place an order
    void placeOrder() {
        vector<pair<MenuItem, int>> order; // To store ordered items and their quantities
        double total = 0.0;                 // Total cost of the order
        int itemId, quantity;

        cout << "\nEnter your order (Item ID and Quantity, -1 to stop):\n";
        while (true) {
            cout << "Item ID: ";
            cin >> itemId;
            if (itemId == -1) break;

            cout << "Quantity: ";
            cin >> quantity;

            bool found = false;
            for (auto& item : menu) {
                if (item.id == itemId) {
                    found = true;
                    if (item.stock >= quantity) {
                        order.emplace_back(item, quantity);   // store a snapshot for receipt
                        item.stock -= quantity;               // Deduct from stock
                        total += item.price * quantity;       // Add to total cost
                    } else {
                        cout << "Insufficient stock for " << item.name << ".\n";
                    }
                    break; // break only when we found the item
                }
            }
            if (!found) {
                cout << "Item with ID " << itemId << " not found.\n";
            }
        }

        saveMenuToFile(); // Save updated stock to file

        // Display the order summary
        cout << "\n=== Order Summary ===\n";
        for (const auto& p : order) {
            const MenuItem& itm = p.first;
            int qty = p.second;
            cout << "- " << itm.name << " x" << qty << " @ $" << fixed << setprecision(2) << itm.price << " each\n";
        }
        cout << "Total: $" << fixed << setprecision(2) << total << "\n";
    }
};

int main() {
    RestaurantSystem system;
    int choice;
    do {
        cout << "\n=== Restaurant Management System ===\n";
        cout << "1. Display Menu\n";
        cout << "2. Add Menu Item\n";
        cout << "3. Update Menu Item\n";
        cout << "4. Remove Menu Item\n";
        cout << "5. Add Customer\n";
        cout << "6. Remove Customer\n";
        cout << "7. Place Order\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.displayMenu(); break;
            case 2: system.addMenuItem(); break;
            case 3: system.updateMenuItem(); break;
            case 4: system.removeMenuItem(); break;
            case 5: system.addCustomer(); break;
            case 6: system.removeCustomer(); break;
            case 7: system.placeOrder(); break;
            case 0: cout << "Exiting system...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
