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

class Database
{
    string owner;// owner username
    string dbName;// database name
    string dbId; //database identification unique
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
