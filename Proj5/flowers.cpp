//
//  main.cpp
//  Programming Assignment 5
//
//  Created by Zachary Chang on 11/10/19.
//  Copyright © 2019 Zachary Chang. All rights reserved.
//

#include "utilities.h" //COMMENT THIS CODE BOIS
#include <iostream>
#include <cstring>
using namespace std;

const char WORDFILENAME[] = "/Users/zacharychang/words.txt"; //Define the path of the file containing the words
const int MAXWORDS = 10000; //MAXWORD limit

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum);
int flower(const char words[][7], int wordnum, char trialWord[]); //return the number of flowers
int bee(const char words[][7], int wordnum, char trialWord[]); //return the number of bees
int getMysteryLength(const char words[][7], int wordnum);

int main()
{
    char wordList[MAXWORDS][MAXWORDLEN + 1]; //define the array of c-strings and fill it
    int nWords = getWords(wordList, 10000, WORDFILENAME);
    
    if (nWords < 1) //check that at least 1 word was put in the array
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 1;
    }
    
    cout << "How many rounds do you want to play? "; //request the number of rounds
    int nRounds;
    cin >> nRounds;
    
    cin.ignore(10000, '\n');
    
    if (nRounds < 1) //check that the number of rounds is positive
    {
        cout << "The number of rounds must be positive." << endl;
        return 1;
    }
    
    int roundScore = 0; //the score for each round
    int sum = 0; //used to calculate the average
    double average;
    double round;
    int min = 0; //define min and max score values
    int max = 0;
    
    cout.setf(ios::fixed); //round all values to two decimal points
    cout.setf(ios::showpoint);
    cout.precision(2);
    
    for (int i = 0; i < nRounds; i++) //iterate through the number of rounds
    {
        round = i + 1; //actual round number
        cout << endl; //leave extra line
        int randPos = randInt(0, nWords - 1); //choosing the random word
        int wordLength = 0;
        wordLength = getMysteryLength(wordList, randPos); //find the lengths of the mystery word and the trial word
        cout << "Round " << i + 1 << endl; //display round number and the length of the mystery word
        cout << "The mystery word is " << wordLength << " letters long." << endl;
        roundScore = playOneRound(wordList, nWords, randPos); //let the user play
        
        if (roundScore == 1) //report the user's score
        {
            cout << "You got it in 1 try." << endl;
        }
        
        else
        {
            cout << "You got it in " << roundScore << " tries." << endl;
        }
        sum = sum + roundScore; //calculate the average
        average = sum/round;
        
        if (roundScore < min) //check for lower score
        {
            min = roundScore;
        }
        
        if (roundScore > max) //check for higher score
        {
            max = roundScore;
        }
        
        if (i == 0) //min and max are the first score after Round 1
        {
            min = roundScore;
            max = roundScore;
        }
        cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << endl;
    }
}

int playOneRound(const char words[][MAXWORDLEN+1], int nWords, int wordnum)
{
    if (nWords < 1 || wordnum < 0 || wordnum >= nWords) //verify that nWords and wordnum have proper input
    {
        return -1;
    }
    
    bool solved = false;
    bool longEnough = false;
    bool lowerCase = true;
    bool wordFound = false;
    int score = 0;
    int flowers;
    int bees;
    char trialWord[101]; //make room for user input
    
    while (solved == false) //run until the mystery word has been found
    {
        longEnough = false; //always reset the status of whether or not the input c-string is valid
        lowerCase = true;
        wordFound = false;
        cout << "Trial word: ";
        cin.getline(trialWord, 101); //get the user to input
        
        for (int i = 0; i < 7; i++) //check that the user input is long enough
        {
            if (trialWord[i] == '\0' && i >= 4)
            {
                longEnough = true;
                break;
            }
            
            else if (trialWord[i] == '\0' && i < 4)
            {
                longEnough = false;
                break;
            }
        }
        
        if (longEnough == false) //report an error, do not increment score
        {
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        
        for (int i = 0; trialWord[i] != '\0'; i++) //check that the whole c-string is comprised of lowercase letters
        {
            if (isalpha(trialWord[i]))
            {
                if (islower(trialWord[i]))
                {
                    continue;
                }
            }
            lowerCase = false;
        }
        
        if (lowerCase == false) //report an error, do not increment score
        {
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        
        for (int i = 0; i < nWords; i++) //check for whether the trialWord exists in the word list
        {
            for (int k = 0; k < 7; k++)
            {
                if (trialWord[k] != words[i][k])
                {
                    wordFound = false;
                    break;
                }
                
                if ((trialWord[k] == words[i][k]) && (trialWord[k] == '\0'))
                {
                    wordFound = true;
                    break;
                }
                
            }
            
            if(wordFound == true) //break the outer loop
            {
                break;
            }
        }
        
        if (wordFound == false) //report an error, do not increment score
        {
            cout << "I don't know that word." << endl;
            continue;
        }
        
        score++; //increment score if the user gives valid input
        flowers = flower (words, wordnum, trialWord); //find the number of flowers
        
        if (flowers == -10) //if flower returns -10, this means the user solved the mystery
        {
            solved = true;
        }
        
        else //if mystery is not solved, go on to find the number of bees and print trial result
        {
            bees = bee (words, wordnum, trialWord);
            cout << "Flowers: " << flowers << ", Bees: " << bees << endl;
        }
    }
    solved = false;
    return score;
}

int flower (const char words[][7], int wordnum, char trialWord[]) //function to find the number of flowers
{
    int flowers = 0;
    int lengthActual = 0; //length of mystery word
    
    lengthActual = getMysteryLength(words, wordnum); //find the length of the mystery word
    
    for (int i = 0; i < lengthActual; i++) //increment flowers everytime one is found
    {
        if (trialWord[i] == '\0' || words[wordnum][i] == '\0')
        {
            break;
        }
        if (words[wordnum][i] == trialWord[i])
        {
            flowers++;
        }
    }
    
    if (flowers == lengthActual) //if all chars are flowers, tell playOneRound that the mystery is solved
    {
        return -10;
    }
    return flowers;
}
                    
int bee (const char words[][7], int wordnum, char trialWord[]) //find the number of bees in the trial word
{
    int bees = 0;
    int lengthActual = 0;
    int lengthTrial = 0;
    int actualTracker[10]; //track already paired chars in the mystery word
    int trialTracker[10]; //track already paired chars in the mystery word
    
    for (int i = 0; i < 10; i++) //set all values in both trackers to 0
    {
        actualTracker[i] = 0;
    }
    
    for (int i = 0; i < 10; i++)
    {
        trialTracker[i] = 0;
    }
    
    lengthActual = getMysteryLength(words, wordnum); //find the length of the mystery word
    
    for (int i = 0; i < 7; i++) //find the length of the trial word
    {
        lengthTrial = i;
        if (trialWord[i] == '\0')
        {
            break;
        }
    }
    
    for (int i = 0; i < lengthActual; i++) //find the flowers, and mark the flowers by assigning value 1 in the arrays
    {
        if (trialWord[i] == '\0' || words[wordnum][i] == '\0')
        {
            break;
        }
        if (words[wordnum][i] == trialWord[i])
        {
            actualTracker[i] = 1;
            trialTracker[i] = 1;
        }
    }
    
    for (int i = 0; i < lengthActual; i++) //the current char is a bee if both chars are equal and not paired up yet
    {
        for (int j = 0; j < lengthTrial; j++)
        {
            if (actualTracker[i] == 1) //move on if the char is already paired
            {
                break;
            }
            
            if (words[wordnum][i] == trialWord[j])
            {
                if (trialTracker[j] != 1)
                {
                    bees++;
                    trialTracker[j] = 1;
                    actualTracker[i] = 1;
                }
            }
        }
    }
    return bees;
}

int getMysteryLength(const char words[][7], int wordnum)
{
    int lengthActual = 0;
    for (int i = 0; i < 7; i++) //find the lengths of the mystery word and the trial word
    {
        lengthActual = i;
        if (words[wordnum][i] == '\0')
        {
            break;
        }
    }
    return lengthActual;
}
