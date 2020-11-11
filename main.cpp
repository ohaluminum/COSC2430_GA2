#include <iostream>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

//Group Member: Xiaoyuan Xu, Lingwei Kong, Lejing Huang
//Contribution: 1/3, 1/3, 1/3

// -------------------- HELPER FUNCTION ----------------------
int binaryToDecimal(int binary)
{
    int decimal = 0;
    int lastDigit = 0;

    //Initialize base value to 1 (2 ^ 0 = 1)
    int base = 1;

    while (binary != 0)
    {
        //Store the last digit
        lastDigit = binary % 10;

        //Get rid of the last digit
        binary = binary / 10;

        decimal += base * lastDigit;
        base = base * 2;
    }

    return decimal;
}

// ------------------- HASH FUNCTION -----------------------
struct HashValueSet
{
    //Use to store two hash values
    int first;
    int second;

    //Use to store current position
    int pos;

    HashValueSet(int first = 0, int second = 0, int pos = 0)
    {
        this->first = first;
        this->second = second;
        this->pos = pos;
    }
};

class CuckooHash
{
private:

    //Crate a dynamic array to store KV table
    HashValueSet** table;

    //Confirm the table size
    int size;

    //Maximum loop time: 10000
    const int MAXTIME = 10000;

public:

    //Default constructor
    CuckooHash(int tableSize = 10)
    {
        size = tableSize;
        table = new HashValueSet * [size];

        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }

    //Default destructor
    ~CuckooHash()
    {
        //Delete inner pointer
        for (int i = 0; i < size; i++)
        {
            delete table[i];
            table[i] = nullptr;
        }

        //Delete outer pointer
        delete[] table;
        table = nullptr;
    }

    //Getter and Setter
    int getSize()
    {
        return size;
    }

    void setSize(int size)
    {
        this->size = size;
    }

    //Create a insert function: Cuckoo Hashing
    bool insert(HashValueSet x, int count)
    {
        //Check if the function was called exceed maximum loop time, return the false value 
        if (count == MAXTIME)
        {
            return false;
        }

        //The first position is empty
        if (table[x.first] == nullptr)
        {
            table[x.first] = new HashValueSet(x.first, x.second, 1);
            return true;
        }
        //The first position is not empty, look for second position
        else
        {
            //The second position is empty
            if (table[x.second] == nullptr)
            {
                table[x.second] = new HashValueSet(x.first, x.second, 2);
                return true;
            }

            //The second position is not empty, insert at the first position 
            else
            {
                //Retrive the previous value that stored in the first position
                HashValueSet temp(table[x.first]->first, table[x.first]->second, table[x.first]->pos);

                //Delete the value that stored in the first position
                delete table[x.first];
                table[x.first] = nullptr;

                //Assign the new value to the first position
                table[x.first] = new HashValueSet(x.first, x.second, 1);

                //Recursion: Place the previous value into the alternative position
                return insertHelper(temp, count + 1);
            }
        }
    }

    //Create a insert helper function: Cuckoo Hashing
    bool insertHelper(HashValueSet x, int count)
    {
        //Check if the function was called exceed maximum loop time, return the false value 
        if (count == MAXTIME)
        {
            return false;
        }

        //Enforce this value to insert into the alternative position
        //Case 1: Kick out from the first position
        if (x.pos == 1)
        {
            //The second position is empty
            if (table[x.second] == nullptr)
            {
                table[x.second] = new HashValueSet(x.first, x.second, 2);
                return true;
            }

            //The second position is not empty
            else
            {
                //Retrive the previous value that stored in the second position
                HashValueSet temp(table[x.second]->first, table[x.second]->second, table[x.second]->pos);

                //Delete the value that stored in the second position
                delete table[x.second];
                table[x.second] = nullptr;

                //Assign the new value to the second position
                table[x.second] = new HashValueSet(x.first, x.second, 2);

                //Recursion: Place the previous value into the alternative position
                return insertHelper(temp, count + 1);
            }
        }

        //Case 2: Kick out from the second position
        else
        {
            //The first position is empty
            if (table[x.first] == nullptr)
            {
                table[x.first] = new HashValueSet(x.first, x.second, 1);
                return true;
            }

            //The second position is not empty
            else
            {
                //Retrive the previous value that stored in the first position
                HashValueSet temp(table[x.first]->first, table[x.first]->second, table[x.first]->pos);

                //Delete the value that stored in the first position
                delete table[x.first];
                table[x.first] = nullptr;

                //Assign the new value to the first position
                table[x.first] = new HashValueSet(x.first, x.second, 1);

                //recursion: Place the previous value into the alternative position
                return insertHelper(temp, count + 1);
            }
        }
    }
};

// ------------------- BST -----------------------
int sum(string str)
{
    //Get sum for ASCII code values
    int sum = 0;

    for (int i = 0; i < str.length(); i++)
    {
        sum += int(str[i]);
    }

    return sum;
}

struct node
{
    int info = 0;
    node* left = nullptr;
    node* right = nullptr;
};

class BST
{
private:
    node* root;

public:
    BST()
    {
        root = nullptr;
    }

    node* getRoot()
    {
        return root;
    }

    //Push new node
    void insert(int info, node* root)
    {
        //Empty position
        if (root == nullptr)
        {
            node* temp = new node;
            temp->info = info;

            root = temp;
        }

        //If the new value larger than root value
        else if (info > root->info)
        {
            if (root->right != nullptr)
            {
                insert(info, root->right);
            }
            else
            {
                node* temp = new node;
                temp->info = info;
                root->right = temp;
            }
        }

        //If the new value smaller than root value
        else if (info < root->info)
        {
            if (root->right != nullptr)
            {
                insert(info, root->left);
            }
            else
            {
                node* temp = new node;
                temp->info = info;
                root->left = temp;
            }
        }

        //Can not have duplicate value
        else
        {
            return;
        }
    }

    //Return the search number include the number being search 
    int search(int info, node* root)
    {
        int searchTime = 0;

        if (root == nullptr)
        {
            return searchTime;
        }
        else
        {
            searchTime++;

            if (root->info == info)
            {
                return searchTime;
            }
            else
            {
                if (info < root->info)
                {
                    searchTime += search(info, root->left);
                }
                else if (info > root->info)
                {
                    searchTime += search(info, root->right);
                }
            }
            return searchTime;
        }
    }
};

// --------------------- MAIN FUNCTION ------------------------

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    string input = am.get("input");
    string output = am.get("output");

    //Test
    //string input = "input15.txt";
    //string output = "output15.txt";

    ifstream inFS;
    ofstream outFS;
    istringstream inSS;

    try
    {
        //Open input file and output file  
        inFS.open(input);
        outFS.open(output);

        //Throw exception if the file doesn't exist
        if (!inFS.is_open())
        {
            throw runtime_error("ERROR: File not found");
        }

        //Throw exception if the file is empty
        if (inFS.peek() == EOF)
        {
            throw runtime_error("ERROR: File is empty");
        }

    }
    catch (runtime_error & e)
    {
        outFS << e.what() << endl;
    }

    // ------------------------------ HASH TABLE -----------------------------
    //Number of test cases (pages)
    int pages;
    inFS >> pages;

    //Number of clothes styles    
    int* numOfStyles = new int[pages];

    //Hash table size
    int tableSize = 0;

    //Two hash value (Binary and Decimal)
    int firstB = 0;
    int secondB = 0;
    int firstD = 0;
    int secondD = 0;

    //The hash value set of new cloth
    HashValueSet newCloth;

    //Pointer of hash table
    CuckooHash* hashTable = nullptr;

    bool validLog = true;
    int validPage = -1;

    //Traversal the pages
    for (int i = 0; i < pages; i++)
    {
        inFS >> numOfStyles[i] >> tableSize;

        //Create a instance of CuckooHash
        hashTable = new CuckooHash(tableSize);

        for (int j = 0; j < numOfStyles[i]; j++)
        {
            inFS >> firstB >> secondB;

            //Convert binary to decimal
            firstD = binaryToDecimal(firstB);
            secondD = binaryToDecimal(secondB);

            newCloth.first = firstD;
            newCloth.second = secondD;

            if (hashTable->insert(newCloth, 0) == false)
            {
                validLog = false;
            }
        }

        if (validLog == true)
        {
            validPage = i;
            outFS << "valid inventory log" << endl;
        }
        else
        {
            outFS << "INVALID INVENTORY LOG!!!" << endl;
            validLog = true;
        }

        //Delete the pointer information
        delete hashTable;
        hashTable = nullptr;
    }

    // ---------------------------------- BST -------------------------------------
    string target;
    inFS >> target;

    string str;
    string skip;

    BST* tree = new BST[pages];

    for (int i = 0; i <= validPage; i++)
    {
        if (i < validPage)
        {
            for (int j = 0; j < numOfStyles[i]; j++)
            {
                inFS >> skip;
            }
        }
        else
        {
            for (int j = 0; j < numOfStyles[i]; j++)
            {
                inFS >> str;

                tree[i].insert(sum(str), tree[i].getRoot());
            }

            outFS << tree[i].search(sum(target), tree[i].getRoot());
        }

    }

    //Close files
    inFS.close();
    outFS.close();

    return 0;
}