#include <iostream>
#include "ArgumentManager.h"
using namespace std;

// ---------------------------------------- MAIN FUNCTION --------------------------------------------

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    //Get the filename of argument name "input" and "output" and "command"
    //string input = am.get("input");
    //string output = am.get("output");

    //Test
    string input = "input33.txt";
    string output = "output33.txt";
    string command = "command33.txt";

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

        string originalLine = "";
        string newLine = "";

        expressionList EList;
        stack ReservedStack;

        // ----------------------------------- ADD EXPRESSION -------------------------------------

        while (getline(inFS, originalLine))
        {
            //Check if the line is empty
            if (originalLine.empty())
            {
                continue;
            }

            newLine = removeSpace(originalLine);
            EList.addAtEnd(newLine);
        }

        //Close input file
        inFS.close();

        //Open command file
        inFS.open(command);

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

        string commandLine;
        string commandType;
        string commandObject;
        bool hasMoreCommand;
        int numOfRemoval = 0;
        string deletedExp;
        int insertPosition = 0;

        while (getline(inFS, commandLine))
        {
            //Check if the line is empty
            if (commandLine.empty())
            {
                continue;
            }

            hasMoreCommand = hasParentheses(commandLine);

            if (hasMoreCommand)
            {
                inSS.clear();
                inSS.str(commandLine);

                getline(inSS, commandType, '(');
                getline(inSS, commandObject, ')');

                //Task 1: convertList()
                if (commandType == "convertList ")
                {
                    EList.convertList(commandObject, inSS);
                }

                //Task 2: removeList()
                else if (commandType == "removeList ")
                {
                    //Detemine the number of deleted expression
                    if (commandObject == "prefix" || commandObject == "postfix")
                    {
                        numOfRemoval = EList.searchExpression(commandObject, inSS);
                    }
                    else if (commandObject == "all")
                    {
                        numOfRemoval = EList.getSize();
                    }
                    else
                    {
                        numOfRemoval = 1;
                    }

                    for (int i = 0; i < numOfRemoval; i++)
                    {
                        deletedExp = EList.removeList(commandObject, inSS);
                    }
                }

                //Task 5: pushReserve()
                else if (commandType == "pushReserve ")
                {
                    //Detemine the number of deleted expression
                    if (commandObject == "prefix" || commandObject == "postfix")
                    {
                        numOfRemoval = EList.searchExpression(commandObject, inSS);
                    }
                    else if (commandObject == "all")
                    {
                        numOfRemoval = EList.getSize();
                    }
                    else
                    {
                        numOfRemoval = 1;
                    }

                    for (int i = 0; i < numOfRemoval; i++)
                    {
                        deletedExp = EList.removeList(commandObject, inSS);

                        //Check if delete an expression
                        if (deletedExp != "")
                        {
                            ReservedStack.push(deletedExp);
                        }
                    }
                }

                //Task 6: popReserve()
                else if (commandType == "popReserve ")
                {
                    insertPosition = stoi(commandObject);

                    if (!ReservedStack.isEmpty())
                    {
                        deletedExp = ReservedStack.top();
                        ReservedStack.pop();

                        EList.addAtMid(deletedExp, insertPosition);
                    }
                }

                //Task 11:  emptyReserve()
                else if (commandType == "emptyReserve ")
                {
                    insertPosition = stoi(commandObject);

                    while (!ReservedStack.isEmpty())
                    {
                        deletedExp = ReservedStack.top();
                        ReservedStack.pop();

                        EList.addAtMid(deletedExp, insertPosition);
                    }
                }
            }

            else
            {
                //Task 3: printList()
                if (commandLine == "printList")
                {
                    EList.printList(outFS);
                }

                //Task 4: printListBackwards()
                else if (commandLine == "printListBackwards")
                {
                    EList.printListBackwards(outFS);
                }

                //Task 7: flipReserve()
                else if (commandLine == "flipReserve")
                {
                    flipReserve(ReservedStack);
                }

                //Task 8: printReserveSize()
                else if (commandLine == "printReserveSize")
                {
                    ReservedStack.printReserveSize(outFS);
                }

                //Task 9: convertReserve()
                else if (commandLine == "convertReserve")
                {
                    convertReserve(ReservedStack, inSS);
                }

                //Task 10: printReserveTop()
                else if (commandLine == "printReserveTop")
                {
                    ReservedStack.printReserveTop(outFS);
                }
            }
        }

        //Testing purpose
        expression* temp = EList.getHead();

        while (temp != nullptr)
        {
            cout << temp->info << endl;
            temp = temp->next;
        }

        cout << endl;

        deletedExp = EList.removeAtMid(3);

        //Testing purpose
        temp = EList.getTail();

        while (temp != nullptr)
        {
            cout << temp->info << endl;
            temp = temp->prev;
        }
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