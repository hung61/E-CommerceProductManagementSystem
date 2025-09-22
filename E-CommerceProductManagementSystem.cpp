#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

// Class Discountable: Interface for objects that can apply a discount. 
// Derived classes must define how to calculate the discounted price.
class Discountable{
    public:
        virtual double applyDiscount(double rate) = 0;
};

// Class Product: Represents a regular product.
class Product: public Discountable{
    protected:
        string name; // Product name
        string ID; // Product identifier
        double price; // Original price
        double rate; // Discount percentage (%)
        double discounted; // Price after applying discount
        int amount; // Stock quantity

    public:
        // Constructor to initialize a product with all information
        Product(string _name, string _ID, double _price, double _rate, int _amount): name(_name), ID(_ID), price(_price), rate(_rate), amount(_amount){}

        string getName(){return name;} // Get product name
        virtual double getPrice(){return price;} // Get original price
        double getRate(){return rate;} // Get discount percentage
        int getAmount(){return amount;} // Get stock quantity

        void updateStock(int _amount){amount = _amount;} // Update stock quantity
        
        // Calculate the price after applying a discount
        double applyDiscount(double rate) override{
            discounted = price * (1 - (rate / 100));
            return discounted;
        }

        // Display product information (name, price, discount if applicable)
        virtual void displayInfo(){
            if (amount == 0) cout << "(Out of stock)\n";
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            if (rate != 0.0) cout << "Price (applying " << rate << "%" << " discount): " << applyDiscount(rate) << endl;
            cout << "Amount: " << amount << endl;
        }

        // Equality operator: returns true if the original prices of two products are equal
        bool operator==(Product& other){
            return this->price == other.price;
        }

        // Greater-than operator: returns true if the current product's original price is higher than the other's
        bool operator>(Product& other){
            return this->price > other.price;
        }
};

// Class Electronics: Represents an electronic product, inheriting from Product.
class Electronics: public Product{
    private:
        int power; // Power consumption (W)
        int warrantyTime; // Warranty period (months)
        double extraFee; // Additional fee (e.g., shipping or handling)
        double totalPrice; // Total price (base price + extra fee)

    public:
        // Initialize an electronic product with full details
        Electronics(string _name, string _ID, double _price, double _rate, int _amount, int _power, int _warrantyTime, double _extraFee)
        : Product(_name, _ID, _price, _rate, _amount), power(_power), warrantyTime(_warrantyTime), extraFee(_extraFee){}

        // Get the final price (base price + extra fee)
        double getPrice() override {return price + extraFee;}

        // Calculate the price after applying a discount (including extra fee)
        double applyDiscount(double rate) override {
            totalPrice = price + extraFee;
            discounted = totalPrice * (1 - (rate / 100));
            return discounted;
        }
        
        // Display detailed information about the electronic product
        void displayInfo() override{
            if (amount == 0) cout << "(Out of stock)\n";
            cout << "Name: " << name << endl;
            cout << "Price: " << price + extraFee << endl;
            if (rate != 0.0) cout << "Price (applying " << rate << "%" << " discount): " << applyDiscount(rate) << endl;
            cout << "Power: " << power << endl;
            cout << "Warranty Time: " << warrantyTime << endl;
            cout << "Amount: " << amount << endl;
        }

        // Equality operator: returns true if the total price (base + extra fee) of two products is equal
        bool operator==(Electronics& other){
            return (this->price + this->extraFee) == (other.price + other.extraFee);
        }

        // Greater-than operator: returns true if the total price of the current product is higher than the other
        bool operator>(Electronics& other){
            return (this->price + this->extraFee) > (other.price + other.extraFee);
        }
};

// Class InventoryList: Manages the inventory of a specific product type (template)
template<typename T>
class InventoryList {
    private:
        vector<T> storage; // List of items in stock

    public:
        // Constructor: initializes the inventory with an initial list of items
        InventoryList(vector<T> _s) : storage(_s) {}

        // Get the entire list of items in stock
        vector<T> getStorage() { return storage; }

        // Add an item to the inventory
        void addItem(T& item) { storage.push_back(item); }

        // Remove an item by its name
        void removeItem(T& item) {
            for (int i = 0; i < storage.size(); i++) {
                if (storage[i].getName() == item.getName()) {
                    storage.erase(storage.begin() + i);
                    cout << "Remove item successful!\n";
                }
            }
        }

        // Find the position of an item by name (returns its index if found or -1 if not found)
        int searchItem(string itemName) {
            for (int i = 0; i < storage.size(); i++) {
                if (storage[i].getName() == itemName) return i;
            }
            return -1;
        }
};

// Class ShoppingCart: Manages the user's shopping cart
template<typename T>
class ShoppingCart{
    private:
        vector<pair<T, int>>& chosenProductList; // List of selected products with their quantities

    public:
        // Initialize the shopping cart with an existing product list
        ShoppingCart(vector<pair<T, int>>& _chosenProductList): chosenProductList(_chosenProductList){}
    
        // Return the list of products currently in the cart
        vector<pair<T, int>> getChosenProductList(){return chosenProductList;}

        // Calculate the total value of the cart (apply discount if the product has one)
        double calculateTotal(){
            double result = 0;
            for (auto p : chosenProductList) {
                if (p.first.getRate() == 0.0) result += (p.first.getPrice() * p.second); // Case without discount
                else result += (p.first.applyDiscount(p.first.getRate()) * p.second); // Case with discount
            }
            return result;
        }

        // Display detailed information of all products in the cart
        void displayCart(){
            for (auto p : chosenProductList){
                cout << "Name: " << p.first.getName() << endl;
                cout << "Price: " << p.first.getPrice() << endl;
                if (p.first.getRate() != 0.0) cout << "Price (applying " << p.first.getRate() << "%" << " discount): " << p.first.applyDiscount(p.first.getRate()) << endl;
                cout << "Quantity: " << p.second << endl;

                if (p.second != 1){
                    cout << "Subtotal: ";
                    if (p.first.getRate() == 0.0) cout << p.first.getPrice() * p.second;
                    else cout << p.first.applyDiscount(p.first.getRate()) * p.second;   
                    cout << endl;
                }

                cout << endl;
            }
        }
        
        // Operator += : Add a new product to the cart
        ShoppingCart& operator+=(const pair<T, int> &item){
            chosenProductList.push_back(item);
            return *this;
        }

        // Search for a product in the cart by name, return its index if found or -1 if not found
        int searchItem(string itemName){
            for (int i = 0; i < chosenProductList.size(); i++){
                if (chosenProductList[i].first.getName() == itemName) return i;
            }
            return -1;
        }

        // Operator -= : Remove a product from the cart
        ShoppingCart& operator-=(pair<T, int> &item){
            for (int i = 0; i < chosenProductList.size(); i++){
                if (chosenProductList[i].first.getName() == item.first.getName()){
                    chosenProductList.erase(chosenProductList.begin() + i);
                    break;
                }
            }
            return *this;
        }
};

// Class Order: Represents an order
class Order{
    private:
        vector<pair<Product, int>> orderedProduct; // List of ordered regular products
        vector<pair<Electronics, int>> orderedElectronicProduct; // List of ordered electronic products

    public:
        Order() = default; // Default constructor
        
        // Create an order from the product cart, electronic cart and update the stock quantity
        void createOrder(ShoppingCart<Product> productCart, ShoppingCart<Electronics> electronicCart){
            orderedProduct = productCart.getChosenProductList();
            orderedElectronicProduct = electronicCart.getChosenProductList();

            for (auto p : orderedProduct){
                int newAmount = p.first.getAmount() - p.second;
                p.first.updateStock(newAmount);
            }

            for (auto ep : orderedElectronicProduct){
                int newAmount = ep.first.getAmount() - ep.second;
                ep.first.updateStock(newAmount);
            }
        }

        // Display order details: product list, prices, and total
        void displayOrder(ShoppingCart<Product> productCart, ShoppingCart<Electronics> electronicCart){
            cout << "ORDER DETAILS:\n";

            productCart.displayCart();
            electronicCart.displayCart();

            double total = productCart.calculateTotal() + electronicCart.calculateTotal();
            cout << "Total: " << total << endl;
        }
};

int main(){
    // Create sample regular products
    Product pA("Book A", "P001", 50.0, 10.0, 20);      
    Product pB("Book B", "P002", 35.0, 5.0, 10);
    Product pC("Notebook C", "P003", 15.0, 0.0, 30);   
    Product pD("Pen D", "P004", 5.0, 0.0, 100);       
    Product pE("Backpack X", "P005", 60.0, 0.0, 15);
    Product pF("Water Bottle", "P006", 18.0, 5.0, 50);
    Product pG("Desk Lamp", "P007", 80.0, 8.0, 0);     
    Product pH("Office Chair", "P008", 120.0, 12.0, 25);

    // Store all regular products in a vector
    vector<Product> productStorage = {pA, pB, pC, pD, pE, pF, pG, pH};
    
    // Create sample electronic products
    Electronics ePhone("Phone X", "E001", 800.0, 15.0, 5, 20, 12, 50.0);     
    Electronics eLaptop("Laptop Z", "E002", 1200.0, 10.0, 3, 65, 24, 80.0);
    Electronics eHeadphone("Headphone H", "E003", 150.0, 0.0, 15, 5, 6, 10.0);
    Electronics eCamera("Camera C", "E004", 500.0, 5.0, 7, 10, 12, 30.0);
    Electronics eTablet("Tablet T", "E005", 600.0, 12.0, 8, 15, 18, 40.0);
    Electronics eSmartwatch("Smartwatch W", "E006", 200.0, 0.0, 20, 3, 6, 15.0);
    Electronics eSpeaker("Speaker S", "E007", 120.0, 0.0, 10, 4, 12, 8.0);
    Electronics eConsole("Console C", "E008", 400.0, 7.0, 0, 25, 24, 25.0);  // out of stock

    // Store all electronic products in a vector
    vector<Electronics> electronicsStorage = {ePhone, eLaptop, eHeadphone, eCamera, eTablet, eSmartwatch, eSpeaker, eConsole};
        
    // Initialize inventories
    InventoryList<Product> productInventory(productStorage); // Inventory of regular products
    InventoryList<Electronics> electronicInventory(electronicsStorage); // Inventory of electronics

    // Initialize carts
    vector<pair<Product, int>> pCartList; // Regular product cart 
    vector<pair<Electronics, int>> eCartList; // Electronic product cart 

    ShoppingCart<Product> productCart(pCartList); // Shopping cart for regular products
    ShoppingCart<Electronics> electronicCart(eCartList);  // Shopping cart for electronics

    // Ask the user to choose a role
    cout << "Choose your role:\n";
    cout << "1. Customer\n";
    cout << "2. Manager\n";
    cout << "Choose: ";
    int roleChoice; 
    cin >> roleChoice;
    cout << "=================\n";

    if (roleChoice == 1){  
        // Get the list of regular products and electronic products from inventory
        vector<Product> pStorage = productInventory.getStorage();
        vector<Electronics> eStorage = electronicInventory.getStorage();

        // Loop to allow the customer to search and select products multiple times
        while (true){
            cout << "Enter product you want to buy: ";
            cin.ignore(); // Clear leftover characters in the input buffer before reading a string
            string itemName; getline(cin, itemName); // Enter the name of the product the user wants to buy
            cout << endl;

            // Search for the product in inventory (returns -1 if not found)
            int productIdx = productInventory.searchItem(itemName);
            int electronicProductIdx = electronicInventory.searchItem(itemName);

            // Display product information if found
            if (productIdx != -1) pStorage[productIdx].displayInfo();
            else if (electronicProductIdx != -1) eStorage[electronicProductIdx].displayInfo();
            else cout << "No results found for " << itemName << endl; // Product not found

            // Ask the user whether to add the product to the cart
            string answer1 = "None";
            while (true & (productIdx != -1 || electronicProductIdx != -1)){
                cout << "Would you want to add this product to your cart? (Yes/No)\n";
                cout << "Your answer: ";
                cin >> answer1;
                if (answer1 == "Yes" || answer1 == "No") break; // Valid input
                else cout << "Invalid answer. Enter your answer again\n";
            }

            // If the user enters Yes, add the product to the cart
            if (answer1 == "Yes") {
                if (productIdx != -1){ // For regular products
                    int productAmount = pStorage[productIdx].getAmount(); // Get stock quantity
                    if (productAmount != 0){ // Check if product is in stock
                        cout << "Enter number of products you want to buy: ";

                        int userAmount;
                        // Validate purchase quantity
                        while (true){
                            cin >> userAmount;
                            if (userAmount <= 0 || userAmount > productAmount) 
                                cout << "Error. Enter again\n";
                            else break;
                        }

                        // Add product and quantity to the shopping cart
                        pair<Product, int> addingProduct = {pStorage[productIdx], userAmount};
                        productCart += addingProduct;
                        cout << "Add to the cart successful!\n";

                    } else cout << "This product is out of stock\n"; // Out of stock case

                } else { // For electronic products
                    int eProductAmount = eStorage[electronicProductIdx].getAmount(); // Get stock quantity
                    if (eProductAmount != 0){
                        cout << "Enter number of products you want to buy: ";

                        int userAmount;
                        // Validate purchase quantity
                        while (true){
                            cin >> userAmount;
                            if (userAmount <= 0 || userAmount > eProductAmount) 
                                cout << "Error. Enter again\n";
                            else break;
                        }

                        // Add electronic product to the cart
                        pair<Electronics, int> addingElectronicProduct = {eStorage[electronicProductIdx], userAmount};
                        electronicCart += addingElectronicProduct;
                        cout << "Add to the cart successful!\n";

                    } else cout << "This product is out of stock\n"; // Out of stock case
                }
            }

            // Ask the user whether to continue searching for other products
            string answer2;
            bool isContinue = true;
            while (true){
                cout << "\nWould you want to look for other products? (Yes/No)\n";
                cout << "Your answer: ";
                cin >> answer2;
                if (answer2 == "Yes" || answer2 == "No"){
                    if (answer2 == "No") isContinue = false; // User chooses not to continue
                    else cout << endl;
                    break;
                } else cout << "Invalid answer. Enter your answer again\n";
            }
            if (!isContinue) break; // Exit loop if user selects No
        }

        // If both regular product cart and electronic product cart are empty, end the program
        if (productCart.getChosenProductList().size() == 0 & electronicCart.getChosenProductList().size() == 0) return 0;

        cout << "=====================================================\n";
        cout << "List of all products in the cart:\n";
        while (true){
            // Display the cart for regular products and electronic products
            productCart.displayCart();
            electronicCart.displayCart();
            double total = productCart.calculateTotal() + electronicCart.calculateTotal();
            cout << "Total: " << total << endl; // Total price of all products in the cart

            // Customer function menu after adding products to the cart
            cout << "Choose function:\n";
            cout << "1. Order product\n"; // Place an order
            cout << "2. Add product\n"; // Add more products to the cart
            cout << "3. Remove product\n";  // Remove a product from the cart
            cout << "4. Compare 2 products\n"; // Compare prices of 2 products
            cout << "Choose: ";
            int choice; cin >> choice;

            if (choice == 1){
                cout << "=====================\n";
                Order newOrder; // Create a new order from the cart
                newOrder.createOrder(productCart, electronicCart); // Save cart items into the order and update inventory quantities
                newOrder.displayOrder(productCart, electronicCart); // Display detailed order information, including products, prices, and total
                return 0; // End the program after placing the order
            } else if (choice == 2){ // Add products to the cart
                while (true){
                    // Enter the name of the product you want to buy
                    cout << "Enter product you want to buy: ";
                    cin.ignore();
                    string itemName; getline(cin, itemName);
                    cout << endl;

                    // Search for the product in the inventory (returns -1 if not found)
                    int productIdx = productInventory.searchItem(itemName);
                    int electronicProductIdx = electronicInventory.searchItem(itemName);

                    // If found, display the product information
                    if (productIdx != -1) pStorage[productIdx].displayInfo();
                    else if (electronicProductIdx != -1) eStorage[electronicProductIdx].displayInfo();
                    else cout << "No results found for " << itemName << endl;

                    // Ask the user if they want to add this product to the cart
                    string answer1 = "None";
                    while (true & (productIdx != -1 || electronicProductIdx != -1)){
                        cout << "Would you want to add this product to your cart? (Yes/No)\n";
                        cout << "Your answer: ";
                        cin >> answer1;
                        if (answer1 == "Yes" || answer1 == "No") break;
                        else cout << "Invalid answer. Enter your answer again\n";
                    }

                    // If the user enters "Yes", add the product to the cart
                    if (answer1 == "Yes") {
                        if (productIdx != -1){ // For regular products
                            int productAmount = pStorage[productIdx].getAmount(); // Get the stock quantity
                            if (productAmount != 0){ // If the item is in stock
                                cout << "Enter number of products you want to buy: ";

                                int userAmount;
                                // Validate the quantity entered by the user
                                while (true){
                                    cin >> userAmount;
                                    if (userAmount <= 0 || userAmount > productAmount)
                                        cout << "Error. Enter again\n";
                                    else break;
                                }

                                // Create a pair (product, quantity purchased) and add it to the cart
                                pair<Product, int> addingProduct = {pStorage[productIdx], userAmount};
                                productCart += addingProduct;
                                cout << "Add to the cart sucessful!\n";

                            } else cout << "This product is out of stock\n"; // If out of stock

                        } else { // For electronic products
                            int eProductAmount = eStorage[electronicProductIdx].getAmount();
                            if (eProductAmount != 0){
                                cout << "Enter number of products you want to buy: ";

                                int userAmount;
                                // Validate the quantity entered by the user
                                while (true){
                                    cin >> userAmount;
                                    if (userAmount <= 0 || userAmount > eProductAmount)
                                        cout << "Error. Enter again\n";
                                    else break;
                                }

                                // Create a pair (electronic product, quantity purchased) and add it to the cart
                                pair<Electronics, int> addingElectronicProduct = {eStorage[electronicProductIdx], userAmount};
                                electronicCart += addingElectronicProduct;
                                cout << "Add to the cart sucessful!\n";

                            } else cout << "This product is out of stock\n"; // Out of stock
                        }
                    }

                    // Ask the user if they want to continue searching for other products
                    string answer2;
                    bool isContinue = true;
                    while (true){
                        cout << "\nWould you want to look for other products? (Yes/No)\n";
                        cout << "Your answer: ";
                        cin >> answer2;
                        if (answer2 == "Yes" || answer2 == "No"){
                            if (answer2 == "No") isContinue = false; // Stop if the user selects No
                            cout << endl;
                            break;
                        } else cout << "Invalid answer. Enter your answer again\n";
                    }
                    if (!isContinue) break; // Exit the loop if the user doesn't want to continue
                }

            } else if (choice == 3){ // Remove a product from the cart
                cout << "\nEnter product name: ";
                cin.ignore();
                string itemName; getline(cin, itemName);

                // Search for the product in the regular product cart
                int findStatus1 = productCart.searchItem(itemName);
                if (findStatus1 == -1){ // If not found in the regular cart, search in the electronics cart
                    int findStatus2 = electronicCart.searchItem(itemName);
                    if (findStatus2 != -1) {
                        // If found in the electronics cart, remove it from the cart
                        electronicCart -= eCartList[findStatus2];
                        cout << "Remove sucessful!\n\n";
                    } else 
                        cout << "No results found for " << itemName << endl; // Not found in either cart
                } else {
                    // If found in the regular product cart, remove it from the cart
                    productCart -= pCartList[findStatus1];
                    cout << "Remove sucessful!\n\n";
                }

            } else if (choice == 4){ // Compare prices of two products in the cart
                // Choose the product type to compare
                cout << "\nChoose type of product to compare:\n";
                cout << "1. Product\n";
                cout << "2. Electronics\n";
                cout << "Choose: ";
                int productTypeChoice; cin >> productTypeChoice;
                cin.ignore();
                cout << endl;

                if (productTypeChoice == 1){
                    // Compare prices between two regular products
                    cout << "Enter first product's name: ";
                    string product1Name; getline(cin, product1Name);

                    // Search for the first product in the regular cart
                    int findStatus1 = productCart.searchItem(product1Name);
                    if (findStatus1 == -1) cout << "No results found for " << product1Name << endl;
                    else {
                        string product2Name;
                        int findStatus2;
                        while (true) {
                            // Enter the second product’s name and make sure it’s not the same as the first one
                            cout << "Enter second product's name: ";
                            getline(cin, product2Name);
                            if (product2Name == product1Name) {
                                cout << "The second product's name duplicates the first one. Enter again\n";
                                continue;
                            }

                            // Search for the second product in the cart
                            findStatus2 = productCart.searchItem(product2Name);
                            if (findStatus2 == -1) cout << "No results found for " << product2Name << endl;
                            break;
                        }

                        // If both products are found in the cart, compare their prices
                        vector<pair<Product, int>> pCart = productCart.getChosenProductList();
                        if (findStatus2 != -1){
                            if (pCart[findStatus1].first == pCart[findStatus2].first) {
                                cout << product1Name << "'s price is equal to " << product2Name << "'s price\n\n";
                            } else if (pCart[findStatus1].first > pCart[findStatus2].first){
                                cout << product1Name << " is more expensive than " << product2Name << endl;
                                cout << endl;
                            } else {
                                cout << product1Name << " is less expensive than " << product2Name << endl;
                                cout << endl;
                            }
                        }   
                    }

                } else if (productTypeChoice == 2){
                    // Compare prices between two electronics
                    cout << "Enter first product's name: ";
                    string product1Name; getline(cin, product1Name);

                    // Search for the first electronic item in the cart
                    int findStatus1 = electronicCart.searchItem(product1Name);
                    if (findStatus1 == -1) cout << "No results found for " << product1Name << endl;
                    else {
                        string product2Name;
                        int findStatus2;
                        while (true) {
                            // Enter the second electronic item's name and ensure it’s not the same as the first one
                            cout << "Enter second product's name: ";
                            getline(cin, product2Name);
                            if (product2Name == product1Name) {
                                cout << "The second product's name duplicates the first one. Enter again\n";
                                continue;
                            }

                            // Search for the second electronic item in the cart
                            findStatus2 = electronicCart.searchItem(product2Name);
                            if (findStatus2 == -1) cout << "No results found for " << product2Name << endl;
                            break;
                        }

                        // If both electronic items are found in the cart, compare their prices
                        vector<pair<Electronics, int>> eCart = electronicCart.getChosenProductList();
                        if (findStatus2 != -1){
                            if (eCart[findStatus1].first == eCart[findStatus2].first) {
                                cout << product1Name << "'s price is equal to " << product2Name << "'s price\n\n";
                            } else if (eCart[findStatus1].first > eCart[findStatus2].first){
                                cout << product1Name << " is more expensive than " << product2Name << endl;
                                cout << endl;
                            } else {
                                cout << product1Name << " is less expensive than " << product2Name << endl;
                                cout << endl;
                            }
                        }
                    }
                } else {
                    cout << "Invalid\n"; // Invalid product type selection
                    return 0;
                }
            } else {
                cout << "Invalid\n"; // Invalid menu option
                return 0;
            }
        }       
    } else if (roleChoice == 2){ // If the user chooses role of manager
        cout << "Choose function:\n";
        cout << "1. Add product\n";
        cout << "2. Remove product\n";
        cout << "Choose: ";
        int choice; cin >> choice;
        cout << "=======================================\n";

        if (choice == 1){ // Add a new product
            // Choose the type of product to add
            cout << "Choose type of product you want to add:\n";
            cout << "1. Product\n";
            cout << "2. Electronic Product\n";
            cout << "Choose: ";
            int productType; cin >> productType;
            if (productType != 1 && productType != 2) {
                cout << "Invalid\n";
                return 0;
            }
            
            // Enter product information
            cout << "Enter product information:\n";
            cin.ignore();
            cout << "Name: ";
            string name; getline(cin, name); // Enter product name
            cout << "ID: ";
            string ID; cin >> ID; // Enter product ID
            cout << "Price: ";
            double price; cin >> price; // Enter product price
            
            double rate;
            string answer;
            while (true){
                cout << "Do you want to apply discount to this product? (Yes/No)\n";
                cout << "Your answer: ";
                cin >> answer;
                if (answer == "Yes" || answer == "No"){
                    if (answer == "Yes") {
                        cout << "Enter rate: ";
                        cin >> rate; // Enter discount rate if any
                    } else rate = 0.0; // No discount applied
                    break;
                } else cout << "Invalid answer. Enter your answer again\n"; // Invalid input case
            }

            cout << "Amount: ";
            int amount; cin >> amount; // Enter stock quantity

            int power;
            int warrantyTime;
            double extraFee; 

            if (productType == 2){ // If the product is electronic
                cout << "Power: ";
                cin >> power; // Enter power
                cout << "Warranty time: ";
                cin >> warrantyTime; // Enter warranty time
                cout << "Extra Fee: ";
                cin >> extraFee; // Enter extra fee (if any)
            }

            // Display the information of the added product
            cout << "Enter product information successful\n";
            cout << "===================================\n";
            cout << "PRODUCT INFORMATION:\n";
            if (productType == 1){ // If the added product is a regular product
                Product newItem(name, ID, price, rate, amount);
                productInventory.addItem(newItem); // Add product to inventory
                productInventory.getStorage().back().displayInfo(); // Display the added product information
            } else { // If the added product is an electronic product
                Electronics newItem(name, ID, price, rate, amount, power, warrantyTime, extraFee);
                electronicInventory.addItem(newItem); // Add electronic product to inventory
                electronicInventory.getStorage().back().displayInfo(); // Display the added electronic product information
            }

        } else if (choice == 2){ // Remove a product from inventory
            cin.ignore();
            cout << "Enter product name: ";
            string productName; getline(cin, productName); // Enter the name of the product to remove
            int findingStatus1 = productInventory.searchItem(productName); // Search for regular product
            if (findingStatus1 != -1) {
                productInventory.removeItem(productInventory.getStorage()[findingStatus1]); // Remove regular product
            } else {
                int findingStatus2 = electronicInventory.searchItem(productName); // Search for electronic product
                if (findingStatus2 != -1) electronicInventory.removeItem(electronicInventory.getStorage()[findingStatus2]); // Remove electronic product
                else cout << "No results found for " << productName << endl; // Case when product not found
            }
        } else cout << "Invalid\n"; // Invalid choice
    } else cout << "Invalid\n";  
}
/*
All test case:
- Test case 1:
+ Input:
1
Desk Lamp
Yes
Yes
Book A
Yes
2
Yes
Phone X
Yes
1
No
2
Backpack X
Yes
1
No
1
+ Output:
Choose your role:
1. Customer
2. Manager
Choose: 1
=================
Enter product you want to buy: Desk Lamp

Name: Desk Lamp
Price: 80
Price (applying 8% discount): 73.6
(Out of stock)
Amount: 0
Would you want to add this product to your cart? (Yes/No)
Your answer: Yes
This product is out of stock

Would you want to look for other products? (Yes/No)
Your answer: Yes

Enter product you want to buy: Book A

Name: Book A
Price: 50
Price (applying 10% discount): 45
Amount: 20
Would you want to add this product to your cart? (Yes/No)
Your answer: Yes
Enter number of products you want to buy: 2
Add to the cart successful!

Would you want to look for other products? (Yes/No)
Your answer: Yes

Enter product you want to buy: Phone X

Name: Phone X
Price: 850
Price (applying 15% discount): 722.5
Power: 20
Warranty Time: 12
Amount: 5
Would you want to add this product to your cart? (Yes/No)
Your answer: Yes
Enter number of products you want to buy: 1
Add to the cart successful!

Would you want to look for other products? (Yes/No)
Your answer: No
=====================================================
List of all products in the cart:
Name: Book A
Price: 50
Price (applying 10% discount): 45
Quantity: 2
Subtotal: 90

Name: Phone X
Price: 850
Price (applying 15% discount): 722.5
Quantity: 1

Total: 812.5
Choose function:
1. Order product
2. Add product
3. Remove product
4. Compare 2 products
Choose: 2

Enter product you want to buy: Backpack X

Name: Backpack X
Price: 60
Amount: 15
Would you want to add this product to your cart? (Yes/No)
Your answer: Yes
Enter number of products you want to buy: 1
Add to the cart successful!

Would you want to look for other products? (Yes/No)
Your answer: No

List of all products in the cart:
Name: Book A
Price: 50
Price (applying 10% discount): 45
Quantity: 2
Subtotal: 90

Name: Phone X
Price: 850
Price (applying 15% discount): 722.5
Quantity: 1

Name: Backpack X
Price: 60
Quantity: 1

Total: 872.5
Choose function:
1. Order product
2. Add product
3. Remove product
4. Compare 2 products
Choose: 1
=====================
ORDER DETAILS:
Name: Book A
Price: 50
Price (applying 10% discount): 45
Quantity: 2
Subtotal: 90

Name: Phone X
Price: 850
Price (applying 15% discount): 722.5
Quantity: 1

Name: Backpack X
Price: 60
Quantity: 1

Total: 872.5
*/