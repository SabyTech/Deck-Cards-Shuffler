#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<stdint.h>
#include<string>
#include<vector>
#include<map>

using namespace std;

const int CardsPerDeck = 52;
const int NumFaces = 13;
const int NumSuits = 4;

// Card class 
class card {
    public:
        card() {};
        card(string cardFace, string cardSuit);
        string getCard();
    private:
        string mFace;
        string mSuit;
};

//card() : creating a card
card::card(string cardFace, string cardSuit)
{
    mFace = cardFace;
    mSuit = cardSuit;
}

//printCard():  function to get card
string card::getCard()
{
    return (mFace + " of " + mSuit);
}

// Deck class
class cardsDeck 
{
    public:
        cardsDeck();
        cardsDeck(string faces[], string suits[], int totalcards); 
        void shuffle();
        void displayDeck();
        void hashCardSequences();
        bool prevCardSequences();
    private:
        card *mDeck;
        int mNumShuffle;
        map<string, string>cardSequences;
};

//cardsDeck() : creating default Deck of 52 cards
cardsDeck::cardsDeck()
{
    string faces[NumFaces] = {"Ace", "Deuce", "Three", "Four", "Five", "Six",
        "Seven", "Eight", "Nine", "Ten","Jack", "Queen", "King"};
    string suits[NumSuits] = {"Hearts", "Diamonds", "Clubs", "Spades"};

    mDeck = new card[CardsPerDeck];
    mNumShuffle = 0;

    for (int i = 0; i < CardsPerDeck; i++)
        mDeck[i] = card(faces[i % NumFaces], suits[i / NumFaces]);
}

//displayDeck() : print the cards in the deck 
void cardsDeck::displayDeck()
{
    cout<<endl<<"shuffled "<<mNumShuffle<<" times"<<endl;
    for (int i = 0; i < CardsPerDeck; i++) {
        cout<<"\t"<<left<<"card["<<left<<setw(2)<<i<<"] "<<setw(20) \
            <<mDeck[i].getCard();
        if ((i + 1) % 4 == 0)
            cout<<endl;
    }
    cout<<endl;
}

//hashCardSequences(): hash cards previous sequences
void cardsDeck::hashCardSequences()
{
    for (int i = 0; i < CardsPerDeck - 1; i++) {
        string current = mDeck[i].getCard();
        string next = mDeck[i+1].getCard();

        cardSequences[current] = next;
    }
    cardSequences[mDeck[CardsPerDeck - 1].getCard()] = "";
}

//prevCardSequences(): look for card sequences in previous shuffles 
bool cardsDeck::prevCardSequences() {
    for (int i = 0; i < CardsPerDeck - 1; i++) {
        string current = mDeck[i].getCard();
        string next = mDeck[i+1].getCard();

         if (cardSequences[current] == next)
            return true;
    }
    return false;
}

//shuffle(): shuffle the cards by swapping card starting from first
// with random position card in the deck
void cardsDeck::shuffle()
{
    hashCardSequences();
    for (int i = 0; i < CardsPerDeck ; i++) {
        int pos = (rand() % CardsPerDeck);
        card temp = mDeck[pos];
        mDeck[pos] = mDeck[i];
        mDeck[i] = temp;
    }
    mNumShuffle++;
}

//displayDeck() : print the cards in the deck 
// program test function
int main() {
    // 1) create a deck of cards, and print/display the cards
    cardsDeck deckOfCards;
    deckOfCards.displayDeck();

    // 2) shuffle the deck of cards, and display the cards (in their shuffled order)
    deckOfCards.shuffle();
    deckOfCards.displayDeck();

    // 3) reshuffle the deck, and display the cards (in their shuffled order)
    deckOfCards.shuffle();
    deckOfCards.displayDeck();

    // 4) look for any 2-card sequence that was also present in the previous shuffle. If any are found, go back to step 3
    if (deckOfCards.prevCardSequences())
        deckOfCards.shuffle();

    //5) when two back to back shuffles do not contain a duplicate sequence, you're done.
    deckOfCards.displayDeck(); 

    return 0;
}
