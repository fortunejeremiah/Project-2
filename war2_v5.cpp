/*
File: war2_v5.cpp
Author: Jeremiah Fortune
Created: August 1, 2026, 2:30PM
Purpose: Add player profile file input and saved player data
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
int    gameCnt();                             //Return the next game number
int    getBet(int, int = 1);                  //Get bet using default minimum
void   payBet(int, int, int, int &, bool &);  //Update coins after a game
void   showBal(int);                          //Display coin balance
void   showBal(string, int);                  //Display name and coin balance
void   shop(int &, bool &);                   //Open the game shop
bool   loadPro(string &, int &, bool &);      //Load saved player profile
void   savePro(string, int, bool);            //Save player profile to file

//Program Execution Begins Here
int main(int argc, char** argv)
{
    //Set the random number seed
    srand(static_cast<unsigned>(time(0)));

    //Display the War game title
    header();

    //Open a file to record game results
    ofstream outfile;
    outfile.open("results.txt");

    //Named Constants
    const int MINRND = 1;      //Fewest rounds allowed
    const int MAXRND = 20;     //Most rounds allowed
    const int DIVLEN = 30;     //Length of divider line
    const int DEKSIZ = 52;     //Number of cards in a full deck
    const int START = 100;     //Starting number of coins

    //Declare Variables
    int youCard, cpuCard;      //Card values 2 to 14
    int youWins, cpuWins, ties;//Round outcome counts
    int deck[DEKSIZ];          //One dimensional array holding the deck
    int rounds;                //Number of rounds
    int game;                  //Current game number
    int pos;                   //Position of next card in deck
    int coins;                 //Player's current coin balance
    int bet;                   //Coins wagered on current game
    char again;                //Play again choice
    char visit;                //Shop visit choice
    char useSave;              //Load saved profile choice
    string name;               //Player's name
    string line;               //One line read from file
    float winPct;              //Percent of rounds won
    float margin;              //Gap between scores
    bool boost;                //True when booster is active
    bool loaded;               //True when profile loads

    //Initialize player information
    coins = START;
    boost = false;
    loaded = false;

    //Ask whether to load a saved profile
    cout << "Load saved profile? (y/n): ";
    cin >> useSave;

    //Try to load the existing profile
    if(useSave == 'y' || useSave == 'Y')
    {
        loaded = loadPro(name, coins, boost);
    }

    //Create a new player when no profile was loaded
    if(loaded == false)
    {
        name = getName();
        coins = START;
        boost = false;
    }

    //Display loaded or new player information
    showBal(name, coins);

    //Validate the number of rounds
    rounds = getRnds(MINRND, MAXRND);

    //Play one or more full games
    do
    {
        //Get the next game number from a static variable
        game = gameCnt();

        //Display the player's current balance
        showBal(name, coins);

        //Get the player's bet using the default minimum
        bet = getBet(coins);

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

        //Update the coin balance using the game result
        payBet(youWins, cpuWins, bet, coins, boost);

        //Display the updated balance
        showBal(coins);

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

        //Write this game's results to the output file
        outfile << "Game " << game << ": You " << youWins;
        outfile << ", CPU " << cpuWins;
        outfile << ", Ties " << ties;
        outfile << ", Coins " << coins << endl;

        //Save the player's current profile after each game
        savePro(name, coins, boost);

        //Ask if the player wants to visit the shop
        cout << "Visit the shop? (y/n): ";
        cin >> visit;

        //Open the shop when requested
        if(visit == 'y' || visit == 'Y')
        {
            shop(coins, boost);

            //Save any shop changes to the profile
            savePro(name, coins, boost);
        }

        //Ask if the player wants another game
        cout << "Play again? (y/n): ";
        cin >> again;
    }
    while(again == 'y' || again == 'Y');

    //Save the final player profile
    savePro(name, coins, boost);

    //Close the output file
    outfile.close();

    //Open the results file for input
    ifstream infile;
    infile.open("results.txt");

    //Display the saved game results
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

//Fill the deck with four copies of values 2 through 14
void fillDck(int deck[], int size)
{
    for(int i = 0; i < size; i++)
    {
        deck[i] = i % 13 + 2;
    }
}

//Shuffle the deck by swapping random card positions
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

//Return the next game number using a static variable
int gameCnt()
{
    static int total = 0;

    total++;

    return total;
}

//Validate and return the player's bet
int getBet(int coins, int minBet)
{
    int bet;
    bool valid;

    //End the game when the player has no coins
    if(coins < minBet)
    {
        cout << "You are out of coins." << endl;
        exit(0);
    }

    valid = false;

    while(valid == false)
    {
        cout << "Enter your bet ("
             << minBet << " to " << coins << "): ";

        cin >> bet;

        if(bet >= minBet && bet <= coins)
        {
            valid = true;
        }
        else
        {
            cout << "Invalid bet, try again." << endl;
        }
    }

    return bet;
}

//Update the player's coins after the game
void payBet(int yw, int cw, int bet, int &coins, bool &boost)
{
    //Award coins when the player wins
    if(yw > cw)
    {
        //Double winnings when a booster is active
        if(boost == true)
        {
            coins = coins + bet * 2;
            boost = false;
            cout << "Booster used, winnings doubled." << endl;
        }
        else
        {
            coins = coins + bet;
        }

        cout << "You won the bet." << endl;
    }
    else if(cw > yw)
    {
        //Subtract the wager when the CPU wins
        coins = coins - bet;
        cout << "You lost the bet." << endl;
    }
    else
    {
        //Return the bet when the game is tied
        cout << "Tie game, your bet was returned." << endl;
    }
}

//Display only the player's current balance
void showBal(int coins)
{
    cout << "Coin balance: " << coins << endl;
}

//Display the player's name and balance
void showBal(string name, int coins)
{
    cout << name << ", you have "
         << coins << " coins." << endl;
}

//Allow the player to purchase a winning bet booster
void shop(int &coins, bool &boost)
{
    const int COST = 25;
    int choice;

    //Display the shop menu
    cout << endl;
    cout << "WAR SHOP" << endl;
    cout << "1. Winning bet booster, 25 coins" << endl;
    cout << "2. Leave shop" << endl;
    cout << "3. Quit game" << endl;
    cout << "Choice: ";
    cin >> choice;

    //Process the player's shop selection
    switch(choice)
    {
        case 1:
            //Prevent purchasing another active booster
            if(boost == true)
            {
                cout << "You already have a booster." << endl;
            }
            else if(coins >= COST)
            {
                coins = coins - COST;
                boost = true;
                cout << "Booster purchased." << endl;
            }
            else
            {
                cout << "Not enough coins." << endl;
            }
            break;

        case 2:
            cout << "Leaving the shop." << endl;
            break;

        case 3:
            cout << "Thanks for playing." << endl;
            exit(0);
            break;

        default:
            cout << "Invalid shop choice." << endl;
            break;
    }
}

//Load the player's name, coins, and booster from profile.txt
bool loadPro(string &name, int &coins, bool &boost)
{
    ifstream profile;

    //Open the saved profile file for input
    profile.open("profile.txt");

    //Return false when no profile file exists
    if(!profile)
    {
        cout << "No saved profile found." << endl;
        return false;
    }

    //Read the saved profile information
    profile >> name;
    profile >> coins;
    profile >> boost;

    //Close the profile input file
    profile.close();

    cout << "Profile loaded." << endl;

    return true;
}

//Save the player's name, coins, and booster to profile.txt
void savePro(string name, int coins, bool boost)
{
    ofstream profile;

    //Open the profile file for output
    profile.open("profile.txt");

    //Write the current profile information
    profile << name << endl;
    profile << coins << endl;
    profile << boost << endl;

    //Close the profile output file
    profile.close();
}