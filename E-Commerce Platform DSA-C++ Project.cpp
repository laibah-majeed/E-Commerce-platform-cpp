#include <iostream>      //Standard console-based input/output.
#include <string>        //Managing text data.
#include <vector>        // Handling lists, key-value pairs (both ordered and unordered).
#include <unordered_map>    //tree type structure
#include <fstream>      //file handling    Reading from and writing to files.
#include <ctime>        //current system time
#include <iomanip>      //Controlling output format
#include <sstream>      //Controlling stream manipulation.

using namespace std;

// User structure
struct User {
    string name;
    string email;
    string phoneNumber;
    string password;
    User* next;
    
    User(const string& name, const string& email, const string& phoneNumber, const string& password)
        : name(name), email(email), phoneNumber(phoneNumber), password(password), next(nullptr) {}
};

// Product structure
struct Product {
    int id;
    string name;
    double price;

    Product(int id, const string& name, double price)
        : id(id), name(name), price(price) {}
};

// CartItem structure
struct CartItem {
    Product* product;
    int quantity;
    CartItem* next;

    CartItem(Product* product, int quantity) : product(product), quantity(quantity), next(nullptr) {}
};

// Order structure
struct Order {
    int orderId;
    string customerName;
    string productName;
    int quantity;
    double totalPrice;
    string phoneNumber;
    string address;
    string dateTime;

    Order(int orderId, const string& customerName, const string& productName, int quantity,
          double totalPrice, const string& phoneNumber, const string& address)
        : orderId(orderId), customerName(customerName), productName(productName), quantity(quantity),
          totalPrice(totalPrice), phoneNumber(phoneNumber), address(address) {
        // Get the current time
        time_t t = time(nullptr);//get systems time
        auto tm = *localtime(&t);//store local time in tm
        ostringstream oss;   // which is used to write data in string 
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");//formate the time for human readability
        dateTime = oss.str();//stores string datetime
    }
};

// Binary Tree Node for Orders
struct OrderNode {
    Order order;
    OrderNode* left;
    OrderNode* right;

    OrderNode(const Order& o) : order(o), left(nullptr), right(nullptr) {}
};

// OrderManager class using Binary Search Tree
class OrderManager {
private:
    OrderNode* root;
    int orderIdCounter;

    void insertOrder(OrderNode*& node, const Order& order) {
        if (!node) {
            node = new OrderNode(order);
        } else if (order.orderId < node->order.orderId) {
            insertOrder(node->left, order);
        } else {
            insertOrder(node->right, order);
        }
    }
    
    void inOrderTraversal(OrderNode* node) {
        if (!node) return;
        inOrderTraversal(node->left);
        displayReceipt(node->order);
        inOrderTraversal(node->right);
    }

public:
    OrderManager() : root(nullptr), orderIdCounter(1) {}

    void placeOrder(const string& customerName, const string& productName, int quantity,
                    double totalPrice, const string& phoneNumber, const string& address) {
        Order newOrder(orderIdCounter++, customerName, productName, quantity, totalPrice, phoneNumber, address);
        insertOrder(root, newOrder);
        saveOrderToFile(newOrder);
    }

    void saveOrderToFile(const Order& order) {
        ofstream orderFile("orders.txt", ios::app);
        if (orderFile) {
            orderFile << "\t~| Order ID: " << order.orderId << "\n"
                      << "\t~| Date and Time: " << order.dateTime << "\n"
                      << "\t~| Customer Name: " << order.customerName << "\n"
                      << "\t~| Product: " << order.productName << "\n"
                      << "\t~| Quantity: " << order.quantity << "\n"
                      << "\t~| Total Price: " << order.totalPrice << "\n"
                      << "\t~| Phone Number: " << order.phoneNumber << "\n"
                      << "\t~| Address: " << order.address << "\n\n";
            orderFile.close();
        } else {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t=============================\n";
            cout << "\t\t\t\t\t\t\t\tFailed to save order to file.\n";
            cout << "\t\t\t\t\t\t\t\t=============================\n";
        }
    }

    void displayReceipt(const Order& order) {
        cout << "\t~| Order ID: " << order.orderId << endl;
        cout << "\t~| Date and Time: " << order.dateTime << endl;
        cout << "\t~| Customer Name: " << order.customerName << endl;
        cout << "\t~| Product Name: " << order.productName << endl;
        cout << "\t~| Quantity: " << order.quantity << endl;
        cout << "\t~| Product Price: $" << fixed << setprecision(2) << order.totalPrice / order.quantity << endl;
        cout << "\t~| Total: $" << fixed << setprecision(2) << order.totalPrice << endl;
        cout << "\t~| Phone Number: " << order.phoneNumber << endl;
        cout << "\t~| Address: " << order.address << endl;
        cout << "\n\n\n\n\t\t\t\t\t\t\t\t=====================================" << endl;
        cout << "\t\t\t\t\t\t\t\t***Thank you for shopping with us!***" << endl;
        cout << "\t\t\t\t\t\t\t\t=====================================" << endl;
    }

    void displayOrderHistory() {
        if (root) {
            cout << "\n\n\t\t\t\t\t\t\t\tOrder History:\n";
            inOrderTraversal(root);
        } else {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t=============================\n";
            cout << "\t\t\t\t\t\t\t\t***No order history found.***\n";
            cout << "\t\t\t\t\t\t\t\t=============================\n";
        }
    }
};

// Shop structure
struct Shop {
    string name;
    vector<Product> products;//shows list of products

    Shop(const string& name) : name(name) {}
};

// ShopManager class using graph representation (unordered map)
class ShopManager {
public:
    unordered_map<string, Shop*> shops;

    void addShop(const string& name) {
        shops[name] = new Shop(name);
    }

    void addProductToShop(const string& shopName, int id, const string& name, double price) {
        if (shops.find(shopName) != shops.end()) {
            shops[shopName]->products.emplace_back(id, name, price);//add new product to the shop map
        }
    }

    void listShops() {
        cout << "\n\n\n\n\t======================" << endl;
        cout << "\t***Available Shops:***" << endl;
        cout << "\t======================" << endl;

        int index = 1;
        for (const auto& shop : shops) { 
            cout << "\t" << index++ << ". " << shop.second->name << "\n";//first shope name and second is shops object
        }
    }

    Shop* getShopByIndex(int index) {
        if (index < 1 || index > shops.size()) {
            return nullptr;
        }
        auto it = shops.begin();
        advance(it, index - 1);//Move 1 step forward
        return it->second;
    }
};

// UserManager class
class UserManager {
private:
    User* head;

public:
    UserManager() : head(nullptr) {}

    void registerUser(const string& name, const string& email, const string& phoneNumber, const string& password) {
        User* newUser = new User(name, email, phoneNumber, password);
        if (!head) {
            head = newUser;
        } 
		else {
            User* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newUser;
        }
        cout << "\n\n\n\n\t\t\t\t\t\t\t\t=============================" << endl;
        cout << "\t\t\t\t\t\t\t\tUser registered successfully!" << endl;
        cout << "\t\t\t\t\t\t\t\t=============================" << endl;
    }
};

// CartManager class
class CartManager {
private:
    CartItem* cart;

public:
    CartManager() : cart(nullptr) {}

    void addToCart(Product* product, int quantity) {
        CartItem* newItem = new CartItem(product, quantity);
        newItem->next = cart;
        cart = newItem;
        cout << "\n\n\n\n\t\t\t\t\t\t\t\t===================================" << endl;
        cout << "\t\t\t\t\t\t\t\tProduct added to cart successfully!" << endl;
        cout << "\t\t\t\t\t\t\t\t===================================" << endl;
    }

    void removeFromCart(CartItem* itemToRemove) {
        if (!cart) return; // Empty cart

        if (cart == itemToRemove) {
            // Remove the head of the cart
            cart = cart->next;
            delete itemToRemove;
            return;
        }

        // Find the previous item
        CartItem* current = cart;
        while (current->next && current->next != itemToRemove) {
            current = current->next;
        }

        if (current->next) {
            current->next = current->next->next; // Remove the item
            delete itemToRemove;
        }
    }

    CartItem* getCartItems() {
        return cart;
    }

    void clearCart() {
        while (cart) {
            CartItem* nextItem = cart->next;
            delete cart;
            cart = nextItem;
        }
    }
};

// Application class to integrate all components
class Application {
private:
    UserManager userManager;
    CartManager cartManager;
    OrderManager orderManager;
    ShopManager shopManager;

public:
    Application() {
        shopManager.addShop("Sports Shop");
        shopManager.addProductToShop("Sports Shop", 1, "Soccer Ball", 400);
        shopManager.addProductToShop("Sports Shop", 2, "Tennis Racket", 1200);
        shopManager.addProductToShop("Sports Shop", 3, "Bat/Ball", 1800);
        shopManager.addProductToShop("Sports Shop", 4, "Football", 3200);
        
        shopManager.addShop("Electronics");
        shopManager.addProductToShop("Electronics", 5, "Sony FX30", 45500);
        shopManager.addProductToShop("Electronics", 6, "Canon EOS 90D", 289000);
        shopManager.addProductToShop("Electronics", 7, "Lenovo Laptop", 44000);
        shopManager.addProductToShop("Electronics", 8, "HP Core i7 Laptop", 89000);
        
        shopManager.addShop("Fast Food");
        shopManager.addProductToShop("Fast Food", 9, "Burger", 350);
        shopManager.addProductToShop("Fast Food", 10, "Pizza", 1200);
        shopManager.addProductToShop("Fast Food", 11, "Shawarma", 400);
        shopManager.addProductToShop("Fast Food", 12, "Sandwich", 200);
    }

    void registerUser() {
        string name, email, phoneNumber, password, confirmPassword;
        cout << "\n\t=============\n";
        cout << "\tRegister User\n";
        cout << "\t=============\n";
        cout << "\t~| Enter Name: ";
        getline(cin, name);//get input with spaces
        cout << "\t~| Enter Email: ";
        getline(cin, email);
        cout << "\t~| Enter Phone Number: ";
        getline(cin, phoneNumber);

        while (true) {
            cout << "\t~| Enter Password: ";
            getline(cin, password);
            cout << "\t~| Confirm Password: ";
            getline(cin, confirmPassword);

            if (password == confirmPassword) {
                break;
            } else {
                cout << "\n\n\n\n\t\t\t\t\t\t\t\t===============================================\n";
                cout << "\t\t\t\t\t\t\t\t***Passwords do not match. Please try again.***\n";
                cout << "\t\t\t\t\t\t\t\t===============================================\n";
            }
        }

        userManager.registerUser(name, email, phoneNumber, password);
    }

    void selectShopAndAddToCart() {
        shopManager.listShops(); 
        
        int shopSelection;
        cout << "\n\n\n\n\t\t\t\t\t\t\t\t==============================" << endl;
        cout << "\t\t\t\t\t\t\t\t***Select a shop by number:***" << endl;
        cout << "\t\t\t\t\t\t\t\t==============================" << endl;
        cin >> shopSelection;

        Shop* selectedShop = shopManager.getShopByIndex(shopSelection);
        if (!selectedShop) {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t====================" << endl;
            cout << "\t\t\t\t\t\t\t\t***Shop not found***" << endl;
            cout << "\t\t\t\t\t\t\t\t====================" << endl;
            return;
        }

        cout << "\n\tProducts in " << selectedShop->name << ":\n";
        for (const Product& product : selectedShop->products) {
            cout << "\tProduct ID: " << product.id << ", Name: " << product.name << ", Price: " << product.price << "\n";
        }

        int id, quantity;
        cout << "\n\tEnter product ID to add to cart: ";
        cin >> id;

        Product* selectedProduct = nullptr;
        for (Product& product : selectedShop->products) {
            if (product.id == id) {
                selectedProduct = &product;
                break;
            }
        }

        if (!selectedProduct) {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t=======================" << endl;
            cout << "\t\t\t\t\t\t\t\t***Product not found***" << endl;
            cout << "\t\t\t\t\t\t\t\t=======================" << endl;
            return;
        }

        cout << "\n\tEnter quantity: ";
        cin >> quantity;

        cartManager.addToCart(selectedProduct, quantity);
    }

    void viewCartItems() {
        CartItem* cart = cartManager.getCartItems();
        if (!cart) {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t========================" << endl;
            cout << "\t\t\t\t\t\t\t\t***Your cart is empty***" << endl;
            cout << "\t\t\t\t\t\t\t\t========================" << endl;  
            return;
        }
        cout << "\n\n\n\n\t\t\t\t\t\t\t\t=======================" << endl;
        cout << "\t\t\t\t\t\t\t\t***Your Cart Items:***" << endl;
        cout << "\t\t\t\t\t\t\t\t======================" << endl;
        CartItem* temp = cart;
        int index = 1;
        while (temp) {
            cout << "\t" << index++ << ". Product: " << temp->product->name
                 << ", Quantity: " << temp->quantity
                 << ", Total: " << fixed << setprecision(2) << (temp->product->price * temp->quantity) << "\n";
            temp = temp->next;
        }
    }

    void placeSelectedOrder(){
        viewCartItems(); // Show current cart items
        CartItem* cart = cartManager.getCartItems();
        if (!cart) {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t==========================" << endl;
            cout << "\t\t\t\t\t\t\t\t***Your cart is empty.***" << endl;
            cout << "\t\t\t\t\t\t\t\t==========================" << endl;
            return;
        }

        // Count how many items are in the cart to determine selection limit
        int itemCount = 0;
        for (CartItem* temp = cart; temp; temp = temp->next) {
            itemCount++;
        }

        int orderChoice;
        cout << "\n\tSelect the number of the product you wish to order (1 to " << itemCount << "): ";
        cin >> orderChoice;

        // Validate selection
        if (orderChoice < 1 || orderChoice > itemCount) {
            cout << "\n\tInvalid selection!\n";
            return;
        }

        CartItem* selectedCartItem = cart;
        // Traverse to the selected cart item
        for (int i = 1; i < orderChoice && selectedCartItem != nullptr; i++) {
            selectedCartItem = selectedCartItem->next;
        }

        if (!selectedCartItem) {
            cout << "\n\tInvalid selection!\n";
            return;
        }

        // Gather delivery information
        string name, phone, address;
        cout << "\t~| Enter your Name: ";
        cin.ignore(); // Ignore leftover new line
        getline(cin, name);
        cout << "\t~| Enter your Phone Number: ";
        getline(cin, phone);
        cout << "\t~| Enter your Address: ";
        getline(cin, address);

        // Show the checkout summary
        cout << "\n\tOrder Summary:\n";
        cout << "\t~| Customer Name: " << name << endl;
        cout << "\t~| Product: " << selectedCartItem->product->name << endl;
        cout << "\t~| Quantity: " << selectedCartItem->quantity << endl;
        cout << "\t~| Total Price: $" << fixed << setprecision(2) 
             << selectedCartItem->product->price * selectedCartItem->quantity << endl;
        cout << "\t~| Phone Number: " << phone << endl;
        cout << "\t~| Address: " << address << endl;

        // Confirming the order
        char confirm;
        cout << "\n\tDo you want to confirm this order? (Y/N): ";
        cin >> confirm;
        if (tolower(confirm) == 'y') {
            orderManager.placeOrder(name, selectedCartItem->product->name, selectedCartItem->quantity,
                                    selectedCartItem->product->price * selectedCartItem->quantity, phone, address);
            
            // Remove the ordered item from the cart
            cartManager.removeFromCart(selectedCartItem);
            
            cout << "\n\tOrder placed successfully!\n";
        } else {
            cout << "\n\tOrder has been canceled.\n";
        }
    }

    void placeOrder() {
        viewCartItems();
        char choice;
        cout << "\n\tDo you want to place a specific order (Y/N)? ";
        cin >> choice;
        if (tolower(choice) == 'y') {
            placeSelectedOrder();
        } else {
            cout << "\n\n\n\n\t\t\t\t\t\t\t\t=======================" << endl;
            cout << "\t\t\t\t\t\t\t\t***No order placed..***" << endl;
            cout << "\t\t\t\t\t\t\t\t=======================" << endl;
        }
    }

    void viewOrderHistory() {
        orderManager.displayOrderHistory();
    }
};

int main() {
    cout << "\n\n\n\n\t\t\t\t\t\t\t\t=====================================" << endl;
    cout << "\t\t\t\t\t\t\t\tWELCOME TO THE E-COMMERCE APPLICATION" << endl; 
    cout << "\t\t\t\t\t\t\t\t=====================================" << endl;

    Application app;

    // Variable to check if user is registered
    bool userRegistered = false;

    // User registration loop
    while (!userRegistered) {
        cout << "\n\t\t\t\t\t\t\t\t============================" << endl;
        cout << "\t\t\t\t\t\t\t\tE-COMMERCE User Registration" << endl;
        cout << "\t\t\t\t\t\t\t\t============================" << endl;
        app.registerUser();
        userRegistered = true; // Set to true after registration

        system("pause"); // Wait for user input before continuing
    }

    int choice;

    do {
        system("CLS");//it clears the screen
        cout << "\n\t\t\t\t\t\t\t\t======================================" << endl;
        cout << "\t\t\t\t\t\t\t\tLet's Enjoy The E-Commerce Application" << endl;
        cout << "\t\t\t\t\t\t\t\t======================================" << endl;

        cout << "\t\t\t\t\t\t|\t\t\t\tMAIN MENU\t\t\t\t|" << endl;
        cout << "\t\t\t\t\t\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" << endl;
        cout << "\t\t\t\t\t\t|\t\t\t1 ~| View Products and Add to Cart              |\n" << endl;
        cout << "\t\t\t\t\t\t|\t\t\t2 ~| Place Order                                |\n" << endl;
        cout << "\t\t\t\t\t\t|\t\t\t3 ~| View Order History                         |\n" << endl;
        cout << "\t\t\t\t\t\t|\t\t\t4 ~| Exit                                       |\n" << endl;
        cout << "\t\t\t\t\t\t\t\t~| Enter your choice from (1 to 4): ";

        cin >> choice;
        cin.ignore();  // to ignore the newline character

        switch (choice) {
            case 1:
                system("CLS"); 
                cout << "\n\t\t\t\t\t\t\t\t========================================" << endl;
                cout << "\t\t\t\t\t\t\t\tE-COMMERCE View Products and Add to Cart" << endl;
                cout << "\t\t\t\t\t\t\t\t========================================" << endl;
                app.selectShopAndAddToCart();
                system("pause");
                break;

            case 2:
                system("CLS"); 
                cout << "\n\t\t\t\t\t\t\t\t======================" << endl;
                cout << "\t\t\t\t\t\t\t\tE-COMMERCE Place Order" << endl;
                cout << "\t\t\t\t\t\t\t\t======================" << endl;
                app.placeOrder();
                system("pause"); 
                break;

            case 3:
                system("CLS"); 
                cout << "\n\t\t\t\t\t\t\t\t=============================" << endl;
                cout << "\t\t\t\t\t\t\t\tE-COMMERCE View Order History" << endl;
                cout << "\t\t\t\t\t\t\t\t=============================" << endl;
                app.viewOrderHistory();
                system("pause"); 
                break;

            case 4:
                cout << "\n\t\t\t\t\t\t\t\t=====================================" << endl;
                cout << "\t\t\t\t\t\t\t\tExiting the E-COMMERCE Application..." << endl;
                cout << "\t\t\t\t\t\t\t\t=====================================" << endl;
                return 0;

            default:
                cout << "\n\t\t\t\t\t\t\t\t=====================================" << endl;
                cout << "\t\t\t\t\t\t\t\tInvalid choice. Please try again..." << endl;
                cout << "\t\t\t\t\t\t\t\t===================================" << endl;
                system("pause");
        }
    } while (choice != 4);

    return 0;
}