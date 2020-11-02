#include <iostream>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

// --------------------------------------- HELPER FUNCTION -------------------------------------------
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

// ---------------------------------------- HASH FUNCTION --------------------------------------------
struct HashValue
{
    int first;
    int second;

    HashValue(int first, int second)
    {
        this->first = first;
        this->second = second;
    }
};

class CuckooHash
{
private:

    //Crate a dynamic array to store KV table
    HashValue** table;

    //Confirm the table size
    int size;

public:

    //Default constructor
    CuckooHash(int tableSize = 10)
    {
        size = tableSize;
        table = new HashValue*[size];

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

    //Create a insert function: Cuckoo Hashing
    bool insert(HashValue x, int loopTime)
    {
        bool isInsert = false;

        //The first position is empty
        if (table[x.first] == nullptr)
        {
            table[x.first] = new HashValue(x.first, x.second);
            isInsert = true;
        }
        //The first position is not empty, look for second position
        else
        {
            //The second position is empty
            if (table[x.second] == nullptr)
            {
                table[x.second] = new HashValue(x.first, x.second);
                isInsert = true;
            }
            //The second position is not empty, insert at the first position 
            else
            {
                //Retrive the previous value stored in the first position

                delete table[x.first];
                table[x.first] = nullptr;

                table[x.first] = new HashValue(x.first, x.second);
            }


        }


    }
};

// ---------------------------------------- MAIN FUNCTION --------------------------------------------

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    //string input = am.get("input");
    //string output = am.get("output");

    //Test
    string input = "input11.txt";
    string output = "output11.txt";

    ifstream inFS;
    ofstream outFS;

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


        cout << binaryToDecimal(10) << endl;
        


    }
    catch (runtime_error & e)
    {
        outFS << e.what() << endl;
    }

    //Close files
    inFS.close();
    outFS.close();

    return 0;
}