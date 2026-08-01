/*
File: war2_v2.cpp
Author: Jeremiah Fortune
Created: August 1, 2026 1:00PM
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


//Function Prototypes
void   header();                        //Print the game banner
string getName();                       //Prompt and return player name
int    getRnds(int, int);               //Validate and return round count
int    drawCrd(int, int);               //Return one random card value
void   cardNam(int);                    //Print a card's name
void   scoreRd(int, int, int &, int &, int &);  //Score one round
bool   perfect(int);                    //True if zero ties
float  winRate(int, int);               //Return win percentage
void   divide(int);                     //Print a divider line


int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned>(time(0)));

    //Display the War game title
    header();

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

    //Initialize
    game = 0;

    //Get the player's name
    name = getName();

    //Validate the number of rounds
    rounds = getRnds(MINRND, MAXRND);

    //Play one or more full games
    do
    {
        game++;
        //Print a divider line
        divide(DIVLEN);
        cout << "Game " << game << endl;
        
        youWins = 0;
        cpuWins = 0;
        ties = 0;

        //Play the rounds using a while loop
        int i = 1;
        while (i <= rounds)
        {
            youCard = drawCrd(NCARDS, MINCARD);
            cpuCard = drawCrd(NCARDS, MINCARD);

            cout << "Round " << i << ": You have ";
            cardNam(youCard);
            cout << ", CPU has ";
            cardNam(cpuCard);
            cout << endl;

            scoreRd(youCard, cpuCard, youWins, cpuWins, ties);
            i++;
        }

        //Show the final score
        cout << "Final score: You " << youWins;
        cout << ", CPU " << cpuWins;
        cout << ", Ties " << ties << endl;

        //Announce the overall winner with a ternary
        cout << "Winner: " << (youWins > cpuWins ? "You" : youWins < cpuWins ? "CPU" : "Tie") << endl;

        //Calculate and display game statistics
        winPct = winRate(youWins, rounds); 
        margin = fabs(youWins - cpuWins);
        cout << "Win percent: " << fixed << setprecision(1) << winPct << "%" << endl;
        cout << "Margin:" << setw(6) << margin << endl;

        if (winPct >= 50.0)     //independent if, no else
        {
            cout << "Great job!" << endl;
            if(perfect(ties))
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


//Print a divider line of the given length
void divide(int len)
{
    for (int d = 0; d < len; d++)
    {
        cout << "=";
    }
    cout << endl;
}

//Print the name of a card, or its number
void cardNam(int card)
{
    switch (card)
    {
        case 11: cout << "Jack"; break;
        case 12: cout << "Queen"; break;
        case 13: cout << "King"; break;
        case 14: cout << "Ace"; break;
        default: cout << card; break;
    }
}

//Prompt for and return the player's name
string getName()
{
    string name;

    cout << "Enter your name: ";
    cin >> name;

    return name;
}

//Validate and return the number of rounds
int getRnds(int lo, int hi)
{
    int rounds;
    bool valid;

    valid = false;

    while(valid == false)
    {
        cout << "How many rounds (" << lo << " to " << hi << ")? ";
        cin >> rounds;

        if(rounds >= lo && rounds <= hi)
        {
            valid = true;
        }
        else
        {
            cout << "Invalid, try again." << endl;
        }
    }

    return rounds;
}

//Generate and return a random card value
int drawCrd(int nrank, int low)
{
    return rand() % nrank + low;
}

//Compare the cards and update the round scores
void scoreRd(int a, int b, int &yw, int &cw, int &t)
{
    if(a > b)
    {
        yw++;
    }
    else if(b > a)
    {
        cw++;
    }
    else
    {
        t++;
    }
}

//Return true when the game has no tied rounds
bool perfect(int t)
{
    return t == 0;
}

//Calculate and return the player's win percentage
float winRate(int wins, int rounds)
{
    return static_cast<float>(wins) / rounds * 100;
}

//Print the title banner for the War card game
void header()
{
    cout << "==============================" << endl;
    cout << "          WAR CARD GAME       " << endl;
    cout << "==============================" << endl;
}
