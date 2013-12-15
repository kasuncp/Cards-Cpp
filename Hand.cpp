#include "Hand.h"
#include "Globals.h"
#include "Deck.h"


Hand::Hand() 
{

}

Hand::~Hand()
{

}

bool Hand::HasKit( int iKitID )
{
	int iSuit	= iKitID & SUIT::SUIT_MASK;
	int iKit	= iKitID & KIT::KIT_MASK;

	list<Card*>::iterator iter = lst_Items.begin();
	for (; iter != lst_Items.end(); ++iter)
	{
		if ( (*iter)->GetSuit() == iSuit && (*iter)->GetKit() == iKit )
		{
			return true;
		}
	}

	return false;
}

void Hand::DebugPrint()
{
	Set<Card>::DebugPrint();
}

bool Hand::HasCard( int iCardID )
{
	list<Card*>::iterator iter = lst_Items.begin();
	for (; iter != lst_Items.end(); ++iter)
	{
		if ( (*iter)->GetID() == iCardID  )
		{
			return true;
		}
	}

	return false;
}

void Hand::AddCard( Card* pCard )
{
	PushBack(pCard);
}

Card* Hand::RemoveCard( int iCardID )
{
	Card* pCard = NULL;

	list<Card*>::iterator iter = lst_Items.begin();
	for (; iter != lst_Items.end(); ++iter)
	{
		if ( (*iter)->GetID() == iCardID )
		{
			lst_Items.erase(iter);
			pCard = Deck::GetInstace()->GetCard(iCardID);
			break;
		}
	}

	return pCard;
}
