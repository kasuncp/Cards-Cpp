#include "Deck.h"
#include "Globals.h"

Deck* Deck::p_Instance = NULL;

Deck::Deck()
{
	p_SortedDeck	= new DeckRoot();
	p_Deck			= new Set<Card>();
	b_Dealing		= false;
}

Deck::~Deck()
{
	DeckSuit* pSPADESSuit = p_SortedDeck->map_Suits[SUIT::SPADES];
	DeckKit* pSPADESMajor = pSPADESSuit->map_Kits[KIT::MAJOR];
	DeckKit* pSPADESMinor = pSPADESSuit->map_Kits[KIT::MINOR];
	
	delete pSPADESMajor;
	delete pSPADESMinor;
	delete pSPADESSuit;

	DeckSuit* pHEARTSSuit = p_SortedDeck->map_Suits[SUIT::HEARTS];
	DeckKit* pHEARTSMajor = pHEARTSSuit->map_Kits[KIT::MAJOR];
	DeckKit* pHEARTSMinor = pHEARTSSuit->map_Kits[KIT::MINOR];
	
	delete pHEARTSMajor;
	delete pHEARTSMinor;
	delete pHEARTSSuit;

	DeckSuit* pCLUBSSuit = p_SortedDeck->map_Suits[SUIT::CLUBS];
	DeckKit* pCLUBSMajor = pCLUBSSuit->map_Kits[KIT::MAJOR];
	DeckKit* pCLUBSMinor = pCLUBSSuit->map_Kits[KIT::MINOR];

	delete pCLUBSMajor;
	delete pCLUBSMinor;
	delete pCLUBSSuit;

	DeckSuit* pDIAMONDSSuit = p_SortedDeck->map_Suits[SUIT::DIAMONDS];
	DeckKit* pDIAMONDSMajor = pDIAMONDSSuit->map_Kits[KIT::MAJOR];
	DeckKit* pDIAMONDSMinor = pDIAMONDSSuit->map_Kits[KIT::MINOR];

	delete pDIAMONDSMajor;
	delete pDIAMONDSMinor;
	delete pDIAMONDSSuit;
	
 	delete p_SortedDeck;
// 	delete p_Deck;
}

void Deck::Init()
{
	DeckKit* pSPADESMajor	= new DeckKit;
	DeckKit* pSPADESMinor	= new DeckKit;
	DeckKit* pHEARTSMajor	= new DeckKit;
	DeckKit* pHEARTSMinor	= new DeckKit;
	DeckKit* pCLUBSMajor	= new DeckKit;
	DeckKit* pCLUBSMinor	= new DeckKit;
	DeckKit* pDIAMONDSMajor = new DeckKit;
	DeckKit* pDIAMONDSMinor = new DeckKit;

	DeckSuit* pSPADESSuit	= new DeckSuit;
	pSPADESSuit->map_Kits[KIT::MAJOR]		= pSPADESMajor;
	pSPADESSuit->map_Kits[KIT::MINOR]		= pSPADESMinor;
	p_SortedDeck->map_Suits[SUIT::SPADES]	= pSPADESSuit;

	DeckSuit* pHEARTSSuit	= new DeckSuit;
	pHEARTSSuit->map_Kits[KIT::MAJOR]		= pHEARTSMajor;
	pHEARTSSuit->map_Kits[KIT::MINOR]		= pHEARTSMinor;
	p_SortedDeck->map_Suits[SUIT::HEARTS]	= pHEARTSSuit;

	DeckSuit* pCLUBSSuit	= new DeckSuit;
	pCLUBSSuit->map_Kits[KIT::MAJOR]		= pCLUBSMajor;
	pCLUBSSuit->map_Kits[KIT::MINOR]		= pCLUBSMinor;
	p_SortedDeck->map_Suits[SUIT::CLUBS]	= pCLUBSSuit;

	DeckSuit* pDIAMONDSSuit = new DeckSuit;
	pDIAMONDSSuit->map_Kits[KIT::MAJOR]		= pDIAMONDSMajor;
	pDIAMONDSSuit->map_Kits[KIT::MINOR]		= pDIAMONDSMinor;
	p_SortedDeck->map_Suits[SUIT::DIAMONDS]	= pDIAMONDSSuit;

	//SPADES Minor
	for(int i=1; i < 7; ++i)
	{
		int iID = SUIT::SPADES | KIT::MINOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::SPADES]->map_Kits[KIT::MINOR]->o_Cards.PushBack(pCard);
	}

	//SPADES Major
	for(int i=7; i < 13; ++i)
	{
		int iID = SUIT::SPADES | KIT::MAJOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::SPADES]->map_Kits[KIT::MAJOR]->o_Cards.PushBack(pCard);
	}

	//HEARTS Minor
	for(int i=1; i < 7; ++i)
	{
		int iID = SUIT::HEARTS | KIT::MINOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::HEARTS]->map_Kits[KIT::MINOR]->o_Cards.PushBack(pCard);
	}

	//HEARTS Major
	for(int i=7; i < 13; ++i)
	{
		int iID = SUIT::HEARTS | KIT::MAJOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::HEARTS]->map_Kits[KIT::MAJOR]->o_Cards.PushBack(pCard);
	}

	//CLUBS Minor
	for(int i=1; i < 7; ++i)
	{
		int iID = SUIT::CLUBS | KIT::MINOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::CLUBS]->map_Kits[KIT::MINOR]->o_Cards.PushBack(pCard);
	}

	//CLUBS Major
	for(int i=7; i < 13; ++i)
	{
		int iID = SUIT::CLUBS | KIT::MAJOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::CLUBS]->map_Kits[KIT::MAJOR]->o_Cards.PushBack(pCard);
	}

	//DIAMONDS Minor
	for(int i=1; i < 7; ++i)
	{
		int iID = SUIT::DIAMONDS | KIT::MINOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::DIAMONDS]->map_Kits[KIT::MINOR]->o_Cards.PushBack(pCard);
	}

	//DIAMONDS Major
	for(int i=7; i < 13; ++i)
	{
		int iID = SUIT::DIAMONDS | KIT::MAJOR | i;
		Card* pCard = new Card(iID);
		p_Deck->PushBack(pCard);
		p_SortedDeck->map_Suits[SUIT::DIAMONDS]->map_Kits[KIT::MAJOR]->o_Cards.PushBack(pCard);
	}
}

void Deck::DebugPrint()
{
	p_Deck->DebugPrint();
}

void Deck::Shuffle(int iNumTurns)
{
	p_Deck->Shuffle(iNumTurns);
}

Set<Card>* Deck::GetKit( int iKitID )
{
	int iSuit	= (iKitID & SUIT::SUIT_MASK);
	int iKit	= (iKitID & KIT::KIT_MASK);

	Set<Card>* pKit = new Set<Card>();

	p_SortedDeck->map_Suits[iSuit]->map_Kits[iKit]->o_Cards.CopyTo(pKit);
	
	return pKit;
}

Set<Card>* Deck::GetSuit( int iSuitID )
{
	int iSuit	= (iSuitID & SUIT::SUIT_MASK);

	Set<Card>* pSuit = new Set<Card>();

	p_SortedDeck->map_Suits[iSuit]->map_Kits[KIT::MINOR]->o_Cards.CopyTo(pSuit);
	p_SortedDeck->map_Suits[iSuit]->map_Kits[KIT::MAJOR]->o_Cards.CopyTo(pSuit);

	return pSuit;
}

Card* Deck::GetCard( int iCardID )
{
	int iSuit	= (iCardID & SUIT::SUIT_MASK);
	int iKit	= (iCardID & KIT::KIT_MASK	);
	int iVal	= (iCardID & CARD::CARD_MASK);

	Card* pCard = NULL;

	p_SortedDeck->map_Suits[iSuit]->map_Kits[iKit]->o_Cards.Begin();
	while( p_SortedDeck->map_Suits[iSuit]->map_Kits[iKit]->o_Cards.HasNext() )
	{
		pCard = p_SortedDeck->map_Suits[iSuit]->map_Kits[iKit]->o_Cards.GetNext();
		if ( pCard->GetID() == iCardID )
		{
			return pCard;
		}
	}

	return NULL;
}

Deck* Deck::GetInstace()
{
	if ( !p_Instance )
	{
		p_Instance = new Deck();
		p_Instance->Init();
	}

	return p_Instance;
}

Set<Card>* Deck::Deal()
{
	if ( b_Dealing == false )
	{
		p_Deck->Begin();
		b_Dealing = true;
	}

	Set<Card>* pSet = new Set<Card>();

	for (int i=0; i < 8; ++i)
	{
		pSet->PushBack(p_Deck->GetNext());
	}

	if (p_Deck->HasNext() == false)
	{
		b_Dealing = false;
	}

	return pSet;
}


