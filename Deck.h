#ifndef _DECK_HEADER_
#define _DECK_HEADER_

#include "Card.h"
#include "Set.h"
#include <map>
using std::map;

class DeckKit
{
public:
	Set<Card> o_Cards;
};

class DeckSuit
{
public:
	map<int, DeckKit*> map_Kits;
};

class DeckRoot
{
public:
	map<int, DeckSuit*> map_Suits;
};

class Deck
{
public:

	static Deck* GetInstace();
	~Deck();

	void Shuffle( int iNumTurns = 1 );

	Set<Card>*	GetKit	( int iKitID  );
	Set<Card>*	GetSuit	( int iSuitID );
	Card*		GetCard	( int iCardID );

	Set<Card>*	Deal();

	void DebugPrint();

protected:
	Deck();
	void Init();

private:
	DeckRoot*	p_SortedDeck;
	Set<Card>*	p_Deck;
	bool b_Dealing;

	static Deck* p_Instance;
};

#endif // _DECK_HEADER_