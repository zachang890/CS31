//
//  fine.cpp
//  Programming Assignment 2
//
//  Created by Zachary Chang on 10/12/19.
//  Copyright © 2019 Zachary Chang. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

int main()
{
    //Obtain the name of the defendant from the user and assign to variable "defendant"
    cout << "Defendant: ";
    string defendant;
    getline(cin, defendant);
    
    //Obtain the amount paid by the defendant from the user and assign to variable "amountPaid"
    cout << "Amount paid (in thousands): ";
    double amountPaid;
    cin >> amountPaid;
    cin.ignore(10000, '\n');
    
    //Ask user whether fake athletic credentials were involved
    cout << "Fake athlete? (y/n): ";
    string fakeAthlete;
    getline(cin, fakeAthlete);
    
    cout << "---" << endl;
    
    //Define the base fine that a defendant must pay
    double fine = 20;
    
    //If the defendant paid $40,000 or less, his/her fine increases by 66% of the paid amount
    if (amountPaid <= 40)
    {
        fine = fine + (0.66 * amountPaid);
    }
    
    /*
      If the defendant paid more than $40,000 but less than or equal to $250,000, his/her fine
      increases by 66% of $40,000 and further increases by 22% of the amount paid that exceeds
      $40,000 if involved with fake credentials, or 10% without fake credentials
    */
    if (amountPaid > 40 && amountPaid <= 250)
    {
        fine = fine + (0.66 * 40);
        if (fakeAthlete == "y")
        {
            fine = fine + (0.22 * (amountPaid - 40));
        }
        
        else
        {
            fine = fine + (0.10 * (amountPaid - 40));
        }
    }
    
    //Same as the case above except now the defendant is fined an extra 14% of how much the amount paid exceeds $250,000
    if (amountPaid > 250)
    {
        fine = fine + (0.66 * 40);
        if (fakeAthlete == "y")
        {
            fine = fine + (0.22 * 210);
        }
        
        else
        {
            fine = fine + (0.10 * 210);
        }
        
        fine = fine + (0.14 * (amountPaid - 250));
    }
    
    //Confine each printed double value to 1 decimal value to the right of the decimal point
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(1);
    
    //Check for errors in any user input; otherwise, print the fine
    if (defendant == "")
    {
        cout << "You must enter a defendant name." << endl;
    }
    
    else if (amountPaid < 0)
    {
        cout << "The amount paid must not be negative." << endl;
    }
    
    else if(!(fakeAthlete == "n" || fakeAthlete == "y"))
    {
        cout << "You must enter y or n." << endl;
    }
    
    else
    {
        cout << "The suggested fine for " << defendant << " is $" << fine << " thousand." << endl;
    }
}


