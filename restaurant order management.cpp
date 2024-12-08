#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

// Structure for Menu Items
struct MenuItem {
    int id;
    string name;
    double price;
};

// Structure for Orders
struct Order {
    int orderId;
    vector<int> itemIds; // IDs of menu items
    string customization;
    string status; // "Pending", "Preparing", "Completed"
};

// Global Variables
vector<MenuItem> menu;                  // Menu items
queue<Order> orderQueue;                // Active orders
map<string, int> inventory;             // Inventory
int nextOrderId = 1;                    // Auto-increment Order ID

// Function Prototypes
void loadMenu();
void displayMenu();
void placeOrder();
void processOrders();
void generateBill(const Order& order);
void manageInventory();
void generateSalesReport();
void saveSalesLog(double amount);
void notifyKitchen();

// Main Function
int main() {
    loadMenu(); // Load menu items (simulated)

    int choice;
    while (true) {
        cout << "\n=== Restaurant Order Management System ===\n";
        cout << "1. Display Menu\n";
        cout << "2. Place Order\n";
        cout << "3. View Order Queue\n";
        cout << "4. Process Orders\n";
        cout << "5. Manage Inventory\n";
        cout << "6. Generate Sales Report\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayMenu();
            break;
        case 2:
            placeOrder();
            break;
        case 3:
            cout << "Order queue contains " << orderQueue.size() << " orders.\n";
            break;
        case 4:
            processOrders();
            break;
        case 5:
            manageInventory();
            break;
        case 6:
            generateSalesReport();
            break;
        case 7:
            cout << "Exiting system. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// Function Definitions

// Load Menu (Hardcoded for simplicity)
void loadMenu() {
    menu = {
        {1, "Burger", 5.99},
        {2, "Pizza", 8.99},
        {3, "Pasta", 7.49},
        {4, "Salad", 4.99}
    };
    inventory = {
        {"Burger Patty", 20},
        {"Pizza Dough", 15},
        {"Pasta", 25},
        {"Salad Greens", 30}
    };
}

// Display Menu
void displayMenu() {
    cout << "\n--- Menu ---\n";
    for (const auto& item : menu) {
        cout << item.id << ". " << item.name << " - $" << item.price << "\n";
    }
}

// Place Order
void placeOrder() {
    vector<int> itemIds;
    int itemId;
    string customization;
    char more;

    cout << "Enter item IDs to order (enter -1 to stop): ";
    while (true) {
        cin >> itemId;
        if (itemId == -1) break;
        itemIds.push_back(itemId);
    }
    cin.ignore(); // Clear newline character
    cout << "Any customizations? ";
    getline(cin, customization);

    Order newOrder = { nextOrderId++, itemIds, customization, "Pending" };
    orderQueue.push(newOrder);

    cout << "Order placed successfully! Order ID: " << newOrder.orderId << "\n";
}

// Process Orders
void processOrders() {
    while (!orderQueue.empty()) {
        Order currentOrder = orderQueue.front();
        orderQueue.pop();

        cout << "Processing Order ID: " << currentOrder.orderId << "\n";
        currentOrder.status = "Preparing";
        this_thread::sleep_for(chrono::seconds(2)); // Simulate preparation time

        currentOrder.status = "Completed";
        cout << "Order ID: " << currentOrder.orderId << " is ready!\n";

        generateBill(currentOrder);
    }
}

// Generate Bill
void generateBill(const Order& order) {
    double total = 0.0;
    double taxRate = 0.08; // 8% tax
    cout << "\n--- Bill for Order ID: " << order.orderId << " ---\n";

    for (int id : order.itemIds) {
        cout << menu[id - 1].name << " - $" << menu[id - 1].price << "\n";
        total += menu[id - 1].price;
    }

    total += total * taxRate;
    cout << "Tax (8%): $" << total * taxRate << "\n";
    cout << "Total: $" << total << "\n";

    saveSalesLog(total);
}

// Manage Inventory
void manageInventory() {
    cout << "\n--- Inventory ---\n";
    for (const auto& item : inventory) {
        cout << item.first << ": " << item.second << "\n";
    }

    string ingredient;
    int quantity;
    cout << "Enter ingredient to restock (or -1 to stop): ";
    while (true) {
        cin >> ingredient;
        if (ingredient == "-1") break;
        cout << "Enter quantity: ";
        cin >> quantity;
        inventory[ingredient] += quantity;
    }
}

// Generate Sales Report
void generateSalesReport() {
    ifstream logFile("sales.log");
    if (!logFile.is_open()) {
        cout << "No sales data found.\n";
        return;
    }

    double totalSales = 0.0;
    double amount;
    while (logFile >> amount) {
        totalSales += amount;
    }
    logFile.close();

    cout << "\n--- Sales Report ---\n";
    cout << "Total Sales: $" << totalSales << "\n";
}

// Save Sales Log
void saveSalesLog(double amount) {
    ofstream logFile("sales.log", ios::app);
    if (logFile.is_open()) {
        logFile << amount << "\n";
        logFile.close();
    }
}

// Notify Kitchen (Optional Simulation)
void notifyKitchen() {
    while (!orderQueue.empty()) {
        this_thread::sleep_for(chrono::seconds(5)); // Simulate delay
        cout << "New order ready to be processed.\n";
    }
}
