//
//  main.cpp
//  Programming Assignment 4
//
//  Created by Zachary Chang on 11/4/19.
//  Copyright © 2019 Zachary Chang. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main() {

    string h[7] = { "mick", "marie", "fiona", "rudy", "", "gordon", "lindsey" };
    assert(lookup(h, 7, "gordon") == 5);
    assert(lookup(h, 7, "fiona") == 2);
    assert(lookup(h, 2, "fiona") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "mick", "marie", "lindsey", "nancy" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "mick?" && g[3] == "nancy?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "lindsey?" && g[3] == "marie?");
    
    string e[4] = { "fiona", "rudy", "", "gordon" };
    assert(subsequence(h, 7, e, 4) == 2);
    
    string d[5] = { "marie", "marie", "marie", "rudy", "rudy" };
    assert(countRuns(d, 5) == 2);
    
    string f[3] = { "lindsey", "fiona", "mike" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "lindsey");
    
    assert(separate(h, 7, "lindsey") == 3);
    
    cout << "All tests succeeded" << endl;
//TEST appendToAll
//
//    string names1[9] = {"carl", "joe", "bob", "zeke", "harold", "ryan", "terry", "zeke", "jaylen"};
//    int j = appendToAll(names1, 4, "!!!");
//    cout << j << endl;
//    for (int i = 0; i < 9; i++)
//    {
//        cout << names1[i] << endl;
//    }
    
//TEST lookup
//    string officeholders[5] = { "donald", "lindsey", "mike", "adam", "nancy" };
//    int j = lookup(names1, 9, "al");
//    cout << j << endl;
    
//TEST positionOfMax
//    string persons[6] = {"carl", "abe", "marie", "z", "zebra", "lol"};
//    int k = positionOfMax(names1, 3);
//    cout << k << endl;
    
//TEST rotateLeft
//    string politician[5] = { "mike", "donald", "lindsey", "nancy", "adam" };
//    int m = rotateLeft(names1, 6, 3);
//    for (int i = 0; i < 9; i++)
//    {
//        cout << names1[i] << endl;
//    }
//
//    cout << m << endl;
    
//    string names2[8] = {"joe", "bob", "bob", "carl", "carl", "carl", "zak", "harold"};
//TEST  countRuns
//    string d[9] = {
//        "rudy", "adam", "mike", "mike", "fiona", "fiona", "fiona", "mike", "joe"
//    };
//    int p = countRuns(names2, 8);
//    cout << p << endl;
    
//TEST flip
//    string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
//    int q = flip(names1, 9);
//    cout << q << endl;
//    for (int i = 0; i < 9; i++)
//    {
//        cout << names1[i] << endl;
//    }
//    string names3[9] = {"bob", "larry", "amy", "jack", "chris", "ronald", "willis", "George", "joe"};
//    string names4[6] = {"bob", "larry", "amy", "jack", "harold", "marvin"};
    
//TEST differ
//    string folks[7] = { "adam", "", "fiona", "mike", "rudy", "nancy", "donald" };
//    string group[6] = { "adam", "", "fiona", "donald", "mike", "rudy" };
//    int r = differ(names3, 9, names4, 6);  //  returns 3
//    int s = differ(folks, 2, group, 3);
//    cout << r << endl;
    
//    string names5[9] = {"bob", "larry", "amy", "jack", "chris", "ronald", "larry", "amy", "jack"};
//    string names6[3] = {"larry", "amy", "jack"};
    
    
//TEST subsequence
//    string names[10] = {"carl", "andy"};
//    string names1[10] = { "carl", "andy", "yo"};
//    int t = subsequence(names5, 9, names6, 3);
//    string names2[10] = { "gordon", "mick" };
//    int u = subsequence(names, 0, names2, 2);
//
//    cout << t << endl;
    
//    string names7[8] = {"bob", "larry", "amy", "jack", "chris", "ronald", "greg", "darren"};
//    string names8[4] = {"greg", "chris", "darren", "jack"};
    
//TEST lookupAny
//    string names[10] = { "gordon", "joe", "nancy", "mick", "adam", "lindsey" };
//    string set1[10] = { "donald", "adam", "mick", "marie" };
//    int v = lookupAny(names7, 8, names8, 4);
//    string set2[10] = { "rudy", "fiona" };
//    int w = lookupAny(names, 6, set2, 2);
//
//    cout << v << endl;
//    cout << w << endl;
    
//TEST separate
//    string persons[6] = { "donald", "lindsey", "marie", "rudy", "fiona", "adam" };
//    int x = separate(names7, 8, "chris");
//
//    for (int i = 0; i < 8; i++)
//    {
//        cout << names7[i] << endl;
//    }
//
//    cout << x << endl;
//
//    string persons2[4] = { "marie", "nancy", "lindsey", "mike" };
//    int y = separate(persons2, 4, "mike");
//
//    for (int i = 0; i < 4; i++)
//    {
//        cout << persons2[i] << endl;
//    }
//
//    cout << y << endl;

}

int appendToAll(string a[], int n, string value)
{
    if (n < 0) //check for valid input
    {
        return -1;
    }
    for (int i = 0; i < n; i++) //for every string, append value to the string
    {
        a[i] = a[i] + value;
    }
    return n;
}

int lookup(const string a[], int n, string target)
{
    if (n < 0) //check for valid input
    {
        return -1;
    }
    for (int i = 0; i < n; i++) //continuously check for if the target is found
    {
        if (a[i] == target)
        {
            return i;
        }
    }
    return -1; //return -1 otherwise
}

int positionOfMax(const string a[], int n)
{
    if (n <= 0) //check for valid input
    {
        return -1;
    }
    
    int posMax = 0;
    for (int i = 1; i < n; i++)
    {
        if(a[i] > a[posMax])
        {
            posMax = i; //continuously edit the poxMax until the index does not change anymore
        }
    }
    return posMax;
}

int rotateLeft(string a[], int n, int pos)
{
    if (n <= 0 || pos < 0) //check for valid input
    {
        return -1;
    }
    
    string target = a[pos]; //define the target string that is to be put at the end of the array
    for(int i = pos; i < n - 1; i++)
    {
        a[i] = a[i+1]; //set each string to the string value after it
    }
    a[n - 1] = target; //set the last string in the array as the target
    
    return pos;
}

int countRuns(const string a[], int n)
{
    if (n < 0) //check for valid input
    {
        return -1;
    }
    
    if (n == 0) //there is no run if there are no values
    {
        return 0;
    }
    
    int runs = 1; //the first value counts
    for (int i = 0; i < n - 1; i++)
    {
        if (a[i+1] != a[i]) //keep checking for when the next value does not equal a previous value
        {
            runs++;
        }
    }
    
    return runs;
}

int flip(string a[], int n)
{
    if (n < 0) //check for valid input
    {
        return -1;
    }
    
    string temp; //set temporary variable
    int indexTwo = n - 1; //indexTwo tracks the index from the END of the array
    for(int i = 0; i < (n/2); i++) //traverse only as far as half the array
    {
        temp = a[i]; //swap the elements
        a[i] = a[indexTwo];
        a[indexTwo] = temp;
        indexTwo--;
    }
    
    return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0) //check for valid input
    {
        return -1;
    }
    
    int upTill; //only traverse up until the first stopping value
    if (n1 < n2)
    {
        upTill = n1;
    }
    
    else
    {
        upTill = n2;
    }
    
    for(int i = 0; i < upTill; i++)
    {
        if (a1[i] != a2[i]) //find the position where they differ
        {
            return i;
        }
    }
    
    return upTill;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0 || n1 < n2) //check for valid input
    {
        return -1;
    }
    
    if (n2 == 0) //an empty array counts as a subsequence at 0
    {
        return 0;
    }
    
    int tentativeReturn = 0; //store a possible return value
    bool broken = false; //tracks if there is a break in a possible subsequence
    bool found = false; //tracks if a subsequence has already been found
    for (int i = 0; i < n1 - n2 + 1; i++) //go up till the length of n2 still fits in the rest of n1
    {
        if (a1[i] == a2[0]) //check for when a value in a1 is equal to the first value in a2
        {
            for (int j = 1; j < n2; j++)
            {
                if (a1[i+j] == a2[j]) //verify that the next value in a1 equals the next value in a2
                {
                    continue;
                }
                
                else
                {
                    broken = true; //notify program that this is not valid subsequence
                    break;
                }
            }
            if (tentativeReturn == 0 && broken != true && found == false)
            {
                tentativeReturn = i; //set a tentative return value
                found = true;
            }
            broken = false;
        }
    }
    
    if (found == false) //if nothing is found
    {
        tentativeReturn = -1;
    }
    return tentativeReturn;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 < 0 || n2 < 0) //verify valid input
    {
        return -1;
    }
    
    if (n1 == 0 || n2 == 0) //cannot find anything if there is nothing
    {
        return -1;
    }
    
    int match = -1;
    bool found = false;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            if (a1[i] == a2[j]) //if a matching value exists, break
            {
                match = i;
                found = true;
                break;
            }
        }
        if (found == true) //break out larger loop
        {
            break;
        }
    }
    
    return match;
}

int separate(string a[], int n, string separator)
{
    if (n < 0) //check for valid input
    {
        return -1;
    }
    
    string temp;
    bool swap = false; //track whether there is still a swap left
    for (int i = 0; i < 10000; i++) //run until there are no more swaps
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (a[j+1] < a[j])
            {
                temp = a[j]; //swap adjacent values
                a[j] = a[j+1];
                a[j+1] = temp;
                swap = true;
            }
        }
        if (swap == false)
        {
            break;
        }
        swap = false;
    }
    
    int returnVal = n;
    for (int i = 0; i < n; i++)
    {
        if (a[i] >= separator) //find the first value that is not less than the separator
        {
            returnVal = i;
            break;
        }
    }
    return returnVal;
}
