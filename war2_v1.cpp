/*
File: war_v4.cpp
Author: Jeremiah Fortune
Created: July 26, 2026 6:16 PM
Purpose: Play War with card names, tie tracking, replay, and file output
*/
//System Libraries
#include <iostream>
#include <iomanip>   
#include <cmath>     
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>    
using namespace std;

int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned>(time(0)));

    //Open a file to record results
    ofstream outfile;
    outfile.open("results.txt");

    //Named Constants
    const int MINRND = 1;      //Fewest rounds allowed
    const int MAXRND = 20;     //Most rounds allowed
    const int NCARDS = 13;     //Number of card ranks
    const int MINCARD = 2;     //Lowest card value
    const int DIVLEN = 30;     //Length of divider line

    //Declare Variables
    int youCard, cpuCard;      //Card values 2 to 14
    int youWins, cpuWins, ties;//Round outcome counts
    int rounds;                //Number of rounds
    int game;                  //Game counter
    char again;                //Play again choice
    string name;               //Player's name
    string line;               //One line read from file
    float winPct;              //Percent of rounds won
    float margin;              //Gap between the two scores
    bool valid;                //Flag for input validation

    //Initialize
    game = 0;

    //Get the player's name
    cout << "Enter your name: ";
    cin >> name;

    //Validate the number of rounds
    valid = false;
    while (valid == false)
    {
        cout << "How many rounds (" << MINRND << " to " << MAXRND << ")? ";
        cin >> rounds;
        if (rounds >= MINRND && rounds <= MAXRND)
        {
            valid = true;
        }
        else
        {
            cout << "Invalid, try again." << endl;
        }
    }

    //Play one or more full games
    do
    {
        game++;
        //Print a divider line
        for (int d = 0; d < DIVLEN; d++)
        {
            cout << "=";
        }
        cout << endl;
        cout << "Game " << game << endl;
        
        youWins = 0;
        cpuWins = 0;
        ties = 0;

        //Play the rounds using a while loop
        int i = 1;
        while (i <= rounds)
        {
            youCard = rand() % NCARDS + MINCARD;
            cpuCard = rand() % NCARDS + MINCARD;

            cout << "Round " << i << ": You have ";
            switch (youCard)
            {
                case 11: cout << "Jack"; break;
                case 12: cout << "Queen"; break;
                case 13: cout << "King"; break;
                case 14: cout << "Ace"; break;
                default: cout << youCard; break;
            }
            cout << ", CPU has ";
            switch (cpuCard)
            {
                case 11: cout << "Jack"; break;
                case 12: cout << "Queen"; break;
                case 13: cout << "King"; break;
                case 14: cout << "Ace"; break;
                default: cout << cpuCard; break;
            }
            cout << endl;

            if (youCard > cpuCard)
            {
                youWins++;
            }
            else if (cpuCard > youCard)
            {
                cpuWins++;
            }
            else
            {
                ties++;
            }
            i++;
        }

        //Show the final score
        cout << "Final score: You " << youWins;
        cout << ", CPU " << cpuWins;
        cout << ", Ties " << ties << endl;

        //Announce the overall winner with a ternary
        cout << "Winner: " << (youWins > cpuWins ? "You" : youWins < cpuWins ? "CPU" : "Tie") << endl;

        //Calculate and display game statistics
        winPct = static_cast<float>(youWins) / rounds * 100; 
        margin = fabs(youWins - cpuWins);
        cout << "Win percent: " << fixed << setprecision(1) << winPct << "%" << endl;
        cout << "Margin:" << setw(6) << margin << endl;

        if (winPct >= 50.0)     //independent if, no else
        {
            cout << "Great job!" << endl;
            if (ties == 0)                    //nested if
            {
                cout << "And a perfect run." << endl;
            }
        }

        //Write this game's results to the file
        outfile << "Game " << game << ": You " << youWins;
        outfile << ", CPU " << cpuWins;
        outfile << ", Ties " << ties << endl;

        cout << "Play again? (y/n): ";
        cin >> again;
    }
    while (again == 'y' || again == 'Y');

    //Close the file
    outfile.close();

    //Read the results back from the file
    ifstream infile;
    infile.open("results.txt");
    cout << "Results for " << name << ":" << endl;
    while (getline(infile, line))
    {
        cout << line << endl;
    }
    infile.close();

    return 0;
}