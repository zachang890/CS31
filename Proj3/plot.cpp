//
//  main.cpp
//  plot
//
//  Created by Zachary Chang on 10/23/19.
//  Copyright © 2019 Zachary Chang. All rights reserved.
//

#include <string>
#include "grid.h"
#include <iostream>
#include <cctype>

using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
void plotPoints(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int main()
{
    setSize(20, 30);
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
            case 0:
                draw();
                break;
            case 1:
                cout << "Syntax error at position " << position+1 << endl;
                break;
            case 2:
                cout << "Cannot perform command at position " << position+1 << endl;
                break;
            default:
                // It should be impossible to get here.
                cerr << "performCommands returned " << status << "!" << endl;
        }
    }
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
    if(dir != HORIZ && dir != VERT) //check if dir is HORIZ or VERT
    {
        return false;
    }
    
    if(fgbg != FG && fgbg != BG) //check if fgbg is FG or BG
    {
        return false;
    }
    
    if(!isprint(plotChar)) //check if the plotChar is printable
    {
        return false;
    }
    
    if(dir == HORIZ) //this section checks for whether a horizontal line exceeds the left/right grid boundaries
    {
        if(distance < 0)
        {
            if(c + distance < 1)
            {
                return false;
            }
        }
        
        else if(distance >= 0)
        {
            if(c + distance > getCols())
            {
                return false;
            }
        }
    }
    
    else if(dir == VERT) //this section check for whether a vertical line exceeds the top/bottom grid boundaries
    {
        if(distance < 0)
        {
            if(r + distance < 1)
            {
                return false;
            }
        }
        
        else if(distance >= 0)
        {
            if(r + distance > getRows())
            {
                return false;
            }
        }
    }
    plotPoints(r, c, distance, dir, plotChar, fgbg); //if all the above conditions are met, set the characters
    return true;
}

void plotPoints(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
    int absDistance;
    int trueC;
    int trueR;
    if(distance < 0) //if distance is negative, still traverse the grid left to right/top to bottom
    {
        absDistance = distance * -1;
        trueC = c - absDistance;
        trueR = r - absDistance;
    }
    else
    {
        absDistance = distance;
        trueC = c;
        trueR = r;
    }
    if(fgbg == FG) //if setting characters in foreground, doesn't matter if characters already exist
    {
        if(dir == HORIZ)
        {
            for(int i = 0; i <= absDistance; i++)
            {
                setChar(r, trueC+i, plotChar);
            }
        }
        else if (dir == VERT)
        {
            for(int i = 0; i <= absDistance; i++)
            {
                setChar(trueR+i, c, plotChar);
            }
        }
    }
    
    else if (fgbg == BG) //ask program to watch for characters that already exist
    {
        if(dir == HORIZ)
        {
            for(int i = 0; i <= absDistance; i++)
            {
                if(getChar(r, trueC+i) == ' ')
                {
                    setChar(r, trueC+i, plotChar);
                }
            }
        }
        
        else if(dir == VERT)
        {
            for(int i = 0; i <= absDistance; i++)
            {
                if(getChar(trueR+i, c) == ' ')
                {
                    setChar(trueR+i, c, plotChar);
                }
            }
        }
    }
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    //currentRow and currentColumn track the current position of the pen and are always reset at each function call
    int currentRow = 1;
    int currentColumn = 1;
    string distance = "";
    int distanceTravel = 0; //distance the pen needs to travel
    int whileCount = 0; //tracks where the for loop should check next for a command
    bool commandError = false;
    bool syntaxError = false;
    
    if(commandString == "") //check for blank input and perform no instructions
    {
        return 0;
    }
    
    for(int i = 0; i < commandString.size(); i = i + whileCount + 1)
    {
        whileCount = 0;
        if(toupper(commandString[i]) == 'H') //check if the command is H or h
        {
            for(int k = i; k < i + 3 && k < commandString.size() - 1 && (isdigit(commandString[k+1]) || commandString[k+1] == '-'); k++) //run only if the integer is of length 3 or less, the command letter is not the last character of the string, and only if the integer contains digits or '-'
            {
                
                if(isdigit(commandString[k+1]) || (commandString[i+1] == '-'))
                {
                    if(!isdigit(commandString[k+1]) && k+1 != i+1) //check that only the FIRST char after the command letter is a '-'
                    {
                        badPos = k + 1;
                        syntaxError = true;
                        break;
                    }
                    distance = distance + commandString[k+1]; //add the character value to the distance string
                }
                
                else //if any other characters are found, just return a syntax error
                {
                    badPos = k+1;
                    syntaxError = true;
                    break;
                }
                whileCount++; //add to whileCount to track next command
            }
            
            if(whileCount == 0) //if the next character is not an integer, return a syntax error
            {
                badPos = i+1;
                syntaxError = true;
                break;
            }
            
            if(i == commandString.size() - 1) //if the command letter is the last character in the string
            {
                badPos = i + 1;
                syntaxError = true;
                break;
            }
            
            distanceTravel = stoi(distance); //convert the distance string to an integer value
            
            if(!(plotLine(currentRow, currentColumn, distanceTravel, HORIZ, plotChar, mode))) //the plotLine function will not actually plot until right before it returns true
            {
                if(commandError == false)
                {
                    badPos = i;
                    commandError = true;
                }
            }
            distance = ""; //reset the distance string
            currentColumn = currentColumn + distanceTravel; //track the column value
            distanceTravel = 0; //reset the distance integer
        }
        
        else if(toupper(commandString[i]) == 'V') //same as above, checking for a vertical line command
        {
            whileCount = 0;
            for(int k = i; k < i + 3 && k < commandString.size() - 1 && (isdigit(commandString[k+1]) || commandString[k+1] == '-'); k++)
            {
                if(isdigit(commandString[k+1]) || (commandString[i+1] == '-'))
                {
                    if(!isdigit(commandString[k+1]) && k+1 != i+1)
                    {
                        badPos = k + 1;
                        syntaxError = true;
                        break;
                    }
                    distance = distance + commandString[k+1];
                }
                
                else
                {
                    badPos = k;
                    syntaxError = true;
                    break;
                }
                whileCount++;
            }
            
            if(whileCount == 0)
            {
                badPos = i+1;
                syntaxError = true;
                break;
            }
            
            if(i == commandString.size() - 1)
            {
                badPos = i + 1;
                syntaxError = true;
                break;
            }
            
            distanceTravel = stoi(distance); //convert the distance string to an integer value
            
            if(!(plotLine(currentRow, currentColumn, distanceTravel, VERT, plotChar, mode)))
            {
                if(commandError == false)
                {
                    badPos = i;
                    commandError = true;
                }
            }
            distance = "";
            currentRow = currentRow + distanceTravel;
            distanceTravel = 0;
        }
        
        else if(toupper(commandString[i]) == 'B') //check if there is a BG command
        {
            if(isprint(commandString[i+1])) //verify the validity of the input character
            {
                plotChar = commandString[i+1]; //set the plotChar to the new character and set new mode
                mode = BG;
            }
            
            else //anything else after B that is non-printable is not allowed
            {
                badPos = i + 1;
                syntaxError = true;
                break;
            }
            whileCount++;
        }
        
        else if(toupper(commandString[i]) == 'F') //same as above, but checking for FG command
        {
            if(isprint(commandString[i+1]))
            {
                plotChar = commandString[i+1];
                mode = FG;
            }
            
            else
            {
                badPos = i + 1;
                syntaxError = true;
                break;
            }
            whileCount++;
        }
        
        else if(toupper(commandString[i]) == 'C') //check for the clear command
        {
            if(toupper(commandString[i+1]) == 'H' || toupper(commandString[i+1]) == 'V' || toupper(commandString[i+1]) == 'B' || toupper(commandString[i+1]) == 'F' || i+1 == commandString.size()) //only perform this if the next value is a command or blank
            {
                clearGrid(); //total reset
                currentRow = 1;
                currentColumn = 1;
                plotChar = '*';
                mode = FG;
            }
            
            else
            {
                badPos = i + 1;
                syntaxError = true;
                break;
            }
        }
        
        else
        {
            badPos = i;
            syntaxError = true;
            break;
        }
    }
    if(syntaxError)
    {
        return 1;
    }
    
    else if(commandError)
    {
        return 2;
    }
    return 0; //draw the graph after iterating through the whole command line
}

