/*
File: war2_v3.cpp
Author: Jeremiah Fortune
Created: August 1, 2026
Purpose: Add a 52 card deck array and pass arrays to functions
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
void   header();                              //Print the game banner
string getName();                             //Prompt and return player name
int    getRnds(int, int);                     //Validate and return round count
void   fillDck(int [], int);                  //Fill the 52 card deck
void   shufDck(int [], int);                  //Shuffle the 52 card deck
int    drawCrd(int [], int &, int);           //Draw one card from the deck
void   cardNam(int);                          //Print a card's name
void   scoreRd(int, int, int &, int &, int &);//Score one round
bool   perfect(int);                          //True if zero ties
float  winRate(int, int);                     //Return win percentage
void   divide(int);                           //Print a divider line

//Program Execution Begins Here
int main(int argc, char** argv)
{
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
    const int DIVLEN = 30;     //Length of divider line
    const int DEKSIZ = 52;     //Number of cards in a full deck

    //Declare Variables
    int youCard, cpuCard;      //Card values 2 to 14
    int youWins, cpuWins, ties;//Round outcome counts
    int deck[DEKSIZ];          //One dimensional array holding the deck
    int rounds;                //Number of rounds
    int game;                  //Game counter
    int pos;                   //Position of the next card in the deck
    char again;                //Play again choice
    string name;               //Player's name
    string line;               //One line read from file
    float winPct;              //Percent of rounds won
    float margin;              //Gap between the two scores

    //Initialize the game counter
    game = 0;

    //Get the player's name
    name = getName();

    //Validate the number of rounds
    rounds = getRnds(MINRND, MAXRND);

    //Play one or more full games
    do
    {
        //Increase the game counter
        game++;

        //Fill and shuffle a fresh deck for each game
        fillDck(deck, DEKSIZ);
        shufDck(deck, DEKSIZ);
        pos = 0;

        //Print a divider line
        divide(DIVLEN);

        //Display the current game number
        cout << "Game " << game << endl;

        //Reset score counts for the new game
        youWins = 0;
        cpuWins = 0;
        ties = 0;

        //Play the rounds using a while loop
        int i = 1;

        while(i <= rounds)
        {
            //Draw the next two cards from the shuffled deck
            youCard = drawCrd(deck, pos, DEKSIZ);
            cpuCard = drawCrd(deck, pos, DEKSIZ);

            //Display both cards for the round
            cout << "Round " << i << ": You have ";
            cardNam(youCard);
            cout << ", CPU has ";
            cardNam(cpuCard);
            cout << endl;

            //Score the current round
            scoreRd(youCard, cpuCard, youWins, cpuWins, ties);

            //Move to the next round
            i++;
        }

        //Show the final score
        cout << "Final score: You " << youWins;
        cout << ", CPU " << cpuWins;
        cout << ", Ties " << ties << endl;

        //Announce the overall winner with a ternary
        cout << "Winner: "
             << (youWins > cpuWins ? "You" :
                youWins < cpuWins ? "CPU" : "Tie")
             << endl;

        //Calculate and display game statistics
        winPct = winRate(youWins, rounds);
        margin = fabs(youWins - cpuWins);

        cout << "Win percent: "
             << fixed << setprecision(1)
             << winPct << "%" << endl;

        cout << "Margin:"
             << setw(6) << margin << endl;

        //Give encouragement for a winning percentage
        if(winPct >= 50.0)
        {
            cout << "Great job!" << endl;

            //Check if the game had no tied rounds
            if(perfect(ties))
            {
                cout << "And a perfect run." << endl;
            }
        }

        //Write this game's results to the file
        outfile << "Game " << game << ": You " << youWins;
        outfile << ", CPU " << cpuWins;
        outfile << ", Ties " << ties << endl;

        //Ask if the player wants another game
        cout << "Play again? (y/n): ";
        cin >> again;
    }
    while(again == 'y' || again == 'Y');

    //Close the output file
    outfile.close();

    //Open the results file for input
    ifstream infile;
    infile.open("results.txt");

    //Display the saved results
    cout << "Results for " << name << ":" << endl;

    while(getline(infile, line))
    {
        cout << line << endl;
    }

    //Close the input file
    infile.close();

    //Exit the program
    return 0;
}

//Print a divider line of the given length
void divide(int len)
{
    for(int d = 0; d < len; d++)
    {
        cout << "=";
    }

    cout << endl;
}

//Print the name of a card, or its number
void cardNam(int card)
{
    switch(card)
    {
        case 11:
            cout << "Jack";
            break;

        case 12:
            cout << "Queen";
            break;

        case 13:
            cout << "King";
            break;

        case 14:
            cout << "Ace";
            break;

        default:
            cout << card;
            break;
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
        cout << "How many rounds ("
             << lo << " to " << hi << ")? ";

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

//Fill the deck with four copies of card values 2 through 14
void fillDck(int deck[], int size)
{
    for(int i = 0; i < size; i++)
    {
        deck[i] = i % 13 + 2;
    }
}

//Shuffle the deck by swapping each card with a random card
void shufDck(int deck[], int size)
{
    int swap;
    int temp;

    for(int i = 0; i < size; i++)
    {
        //Select a random location in the deck
        swap = rand() % size;

        //Swap the two card positions
        temp = deck[i];
        deck[i] = deck[swap];
        deck[swap] = temp;
    }
}

//Return the next card from the shuffled deck
int drawCrd(int deck[], int &pos, int size)
{
    int card;

    //Get the card at the current deck position
    card = deck[pos];

    //Move to the next deck position
    pos++;

    return card;
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
