#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

// Abstract class representing a generic Bill
class AbstractBill {
public:
    virtual void setItem(string item) = 0;
    virtual void setRate(int rate) = 0;
    virtual void setQuant(int quant) = 0;
    virtual string getItem() const = 0;    // Getter method for Item
    virtual int getRate() const = 0;       // Getter method for Rate
    virtual int getQuant() const = 0;      // Getter method for Quantity
    virtual void printDetails() const = 0;
    virtual ~AbstractBill() {}
};

// Base class implementing the abstract class
class Bill : public AbstractBill {
protected:
    string Item;
    int Rate, Quantity;

public:
    Bill() : Item(""), Rate(0), Quantity(0) { }
    Bill(const Bill &other) {  // Copy constructor
        Item = other.Item;
        Rate = other.Rate;
        Quantity = other.Quantity;
    }

    // Setter methods (encapsulation)
    void setItem(string item) override {
        Item = item;
    }
    void setRate(int rate) override {
        Rate = rate;
    }
    void setQuant(int quant) override {
        Quantity = quant;
    }

    // Getter methods
    string getItem() const override {
        return Item;
    }
    int getRate() const override {
        return Rate;
    }
    int getQuant() const override {
        return Quantity;
    }

    // Virtual function (polymorphism)
    void printDetails() const override {
        cout << "Item: " << Item << ", Rate: " << Rate << ", Quantity: " << Quantity << endl;
    }

    // Operator overloading to compare two Bill objects
    bool operator==(const Bill& other) const {
        return (Item == other.Item && Rate == other.Rate && Quantity == other.Quantity);
    }

    // Destructor
    ~Bill() {}
};

// Derived class representing a specialized Bill with additional features
class DiscountedBill : public Bill {
private:
    int discount; // Discount percentage
public:
    DiscountedBill() : Bill(), discount(0) { }

    // Setter for discount
    void setDiscount(int disc) {
        discount = disc;
    }

    // Overriding the virtual function to include discount information (polymorphism)
    void printDetails() const override {
        cout << "Item: " << Item << ", Rate: " << Rate << ", Quantity: " << Quantity << ", Discount: " << discount << "%" << endl;
    }

    // Function to calculate the final price after discount
    int calculateDiscountedPrice() const {
        return Rate * Quantity * (100 - discount) / 100;
    }
};

// Function to add an item to the bill
void addItem(AbstractBill& b) {
    string item;
    int rate, quant;

    cout << "\tEnter Item Name: ";
    cin >> item;
    b.setItem(item);

    cout << "\tEnter Rate Of Item: ";
    cin >> rate;
    b.setRate(rate);

    cout << "\tEnter Quantity Of Item: ";
    cin >> quant;
    b.setQuant(quant);

    ofstream out("Bill.txt", ios::app);
    if (!out) {
        throw runtime_error("\tError: File Can't Open!");
    } else {
        out << b.getItem() << " : " << b.getRate() << " : " << b.getQuant() << endl;
        cout << "\tItem Added Successfully" << endl;
    }
    out.close();
}

// Function to search for an item
void searchItem(const string& itemName) {
    ifstream in("Bill.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string item;
        int rate, quantity;
        char delimiter;

        ss >> item >> delimiter >> rate >> delimiter >> quantity;
        if (item == itemName) {
            found = true;
            cout << "\tFound Item - " << item << " | Rate: " << rate << " | Quantity: " << quantity << endl;
            break;
        }
    }
    if (!found) {
        cout << "\tItem Not Found!" << endl;
    }
    in.close();
}

// Function to update an item
void updateItem(const string& itemName) {
    ifstream in("Bill.txt");
    ofstream out("Bill_temp.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string item;
        int rate, quantity;
        char delimiter;

        ss >> item >> delimiter >> rate >> delimiter >> quantity;
        if (item == itemName) {
            found = true;
            cout << "\tEnter New Rate: ";
            cin >> rate;
            cout << "\tEnter New Quantity: ";
            cin >> quantity;
            out << item << " : " << rate << " : " << quantity << endl;
            cout << "\tItem Updated Successfully!" << endl;
        } else {
            out << line << endl;
        }
    }
    if (!found) {
        cout << "\tItem Not Found!" << endl;
    }
    in.close();
    out.close();
    remove("Bill.txt");
    rename("Bill_temp.txt", "Bill.txt");
}

// Function to delete an item
void deleteItem(const string& itemName) {
    ifstream in("Bill.txt");
    ofstream out("Bill_temp.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string item;
        int rate, quantity;
        char delimiter;

        ss >> item >> delimiter >> rate >> delimiter >> quantity;
        if (item != itemName) {
            out << line << endl;
        } else {
            found = true;
            cout << "\tItem Deleted Successfully!" << endl;
        }
    }
    if (!found) {
        cout << "\tItem Not Found!" << endl;
    }
    in.close();
    out.close();
    remove("Bill.txt");
    rename("Bill_temp.txt", "Bill.txt");
}

// Function to print the bill
void printBill() {
    ifstream in("Bill.txt");
    string line;
    int totalAmount = 0;

    cout << "\t Item | Rate | Quantity | Amount" << endl;
    while (getline(in, line)) {
        stringstream ss(line);
        string item;
        int rate, quantity;
        char delimiter;

        ss >> item >> delimiter >> rate >> delimiter >> quantity;
        int amount = rate * quantity;
        cout << "\t" << item << " | " << rate << " | " << quantity << " | " << amount << endl;
        totalAmount += amount;
    }
    cout << "\t----------------------------" << endl;
    cout << "\t Total Bill: " << totalAmount << endl;
    in.close();
}

int main() {
    AbstractBill* b = new DiscountedBill();

    bool exit = false;
    while (!exit) {
        int val;
        cout << "\n\tWelcome To Super Market Billing System" << endl;
        cout << "\t**************************************" << endl;
        cout << "\t1. Add Item." << endl;
        cout << "\t2. Print Bill." << endl;
        cout << "\t3. Search Item." << endl;
        cout << "\t4. Update Item." << endl;
        cout << "\t5. Delete Item." << endl;
        cout << "\t6. Exit." << endl;
        cout << "\tEnter Choice: ";
        cin >> val;

        try {
            switch (val) {
                case 1:
                    addItem(*b);
                    break;
                case 2:
                    printBill();
                    break;
                case 3: {
                    string itemName;
                    cout << "\tEnter Item Name to Search: ";
                    cin >> itemName;
                    searchItem(itemName);
                    break;
                }
                case 4: {
                    string itemName;
                    cout << "\tEnter Item Name to Update: ";
                    cin >> itemName;
                    updateItem(itemName);
                    break;
                }
                case 5: {
                    string itemName;
                    cout << "\tEnter Item Name to Delete: ";
                    cin >> itemName;
                    deleteItem(itemName);
                    break;
                }
                case 6:
                    exit = true;
                    cout << "\tGoodbye!" << endl;
                    break;
                default:
                    cout << "\tInvalid Choice!" << endl;
            }
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
        }
    }
    delete b; // Clean up the dynamically allocated memory
    return 0;
}
