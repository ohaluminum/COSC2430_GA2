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