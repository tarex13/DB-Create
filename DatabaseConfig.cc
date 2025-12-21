export module DatabaseConfig;

import <iostream>;
import <memory>;
import <map>;
import <vector>;
import <string>;

using namespace std;

// Base class for Row with a virtual destructor
class BaseRow
{
public:
    virtual ~BaseRow() = 0;       // Virtual destructor
};

BaseRow::~BaseRow() {}

// Templated Row class
template <typename T>
class Row : public BaseRow
{                // Inherit from BaseRow
    string type; // Type of the Row
    T value;

public:
    Row(string type, T value) : type{type}, value{value} {};
    Row(string type) : type{type} {}; // Default constructor
    ~Row() {};

    // Value Setter Function
    void setValue(T value)
    {
        this->value = value; // Use 'this' to resolve ambiguity
    }
};

// Utility function to validate allowed types
bool typeAllowed(string type)
{
    return (type == "Integer" || type == "Boolean" ||
            type == "Float" || type == "Char" ||
            type == "String");
}

export class Database
{
    string owner;// owner username
    string dbName;// database name
    string dbId; //database identification unique - we could use uuid too ig
    map<string, string> column;               // This contains information about the columns being made in the database. Contains column name and type
    vector<vector<unique_ptr<BaseRow>>> rows; // Store each row as BaseRow pointers

    // This function is used for creating the rows
    unique_ptr<BaseRow> createRow(string type)
    {
        if (type == "Integer")
        {
            return make_unique<Row<int>>(type); // Create Row<int>
        }
        else if (type == "Boolean")
        {
            return make_unique<Row<bool>>(type); // Create Row<bool>
        }
        else if (type == "Float")
        {
            return make_unique<Row<float>>(type); // Create Row<float>
        }
        else if (type == "Char")
        {
            return make_unique<Row<char>>(type); // Create Row<char>
        }
        else if (type == "String")
        {
            return make_unique<Row<string>>(type); // Create Row<string>
        }
        return nullptr; // Safety return but should technically not be reached
    }

public:

    Database(string owner, string dbName):owner{owner}, dbName{dbName}{
        dbId = owner + "_" + dbName;
    }

    string identify(string type = "id"){
        if(type == "id"){
            return dbId;
        }else if(type == "owner"){
            return owner;
        }else if(type == "name"){
            return dbName;
        }
        return "";
    }

    void setDBName(string newName){
        dbName = newName;
    }

    // title represents column name/title, type represents column type(lowercase) e.g integer
    // return types: false means the add was invalid maybe because of duplicate column name, or wrong column type, true is add was successful
    // #alteration: we could also include column allowances like can this column be empty for example
    bool addColumn(string title, string type)
    {
        if (!typeAllowed(type) || column.count(title) == 1)
        {
            return false;
        }
        column[title] = type;
        unique_ptr<BaseRow> add = createRow(type);

        // Resize rows vector and add the new row
        rows.push_back(vector<unique_ptr<BaseRow>>());
        rows.back().emplace_back(std::move(add));

        return true;
    }

    // This function performs the modify options given in modifyDB(inmain)
    bool modify(int i){
        switch (i)
        {
        case 1:{
            string dbName;
            string type;
            cout << "Enter new column name(or 'return' to go to Modify Menu): ";
            cin >> dbName;
            if(dbName == "return"){
                return false;
            }
            cout << "Next choose from one of the column types below" << endl;
            cout << "1. String\n2. Integer\n3. Boolean\n4. Float\n5. Char\n" << endl;
            cout << "Enter the Column type, as above(case sensitive): "
            cin >> type;
            return addColumn(dbName, type);// later we look at the logic of where we ask the user whether they want to provide a default fill or maybe a bulk fill for current data in the database 
        }
        case 2:{
            // Add the new row
            break;
        }
        case 3:{
            for(auto it = rows.begin(); i != rows.end(); i++){
                if((*it))
            }
            break;
        }
        case 4:{
            break;
        }
        case 5:{
            break;
        }
        case 6:{
            break;
        }
        case 7:{
            break;
        }
        case 8:{
            break;
        }
        default:
            break;
        }
    }

    // This is the overriden version with type included
    // fill represents a value for bulk fillings
    template <typename T>
    bool addColumn(string title, string type, T fill)
    {
        if (!typeAllowed(type) || column.count(title) == 1)
        {
            return false;
        }
        column[title] = type;
        unique_ptr<BaseRow> add = createRow(type);
        static_cast<Row<T> *>(add.get())->setValue(fill); // Cast to correct Row type
        rows.emplace_back(std::move(add));
        return true;
    }
};
