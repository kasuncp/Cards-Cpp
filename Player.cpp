#include "Player.h"
#include <iostream>

using namespace std;

Player::Player( int iID ) : i_PlayerID(iID)
{
	b_KBInit = false;
	p_Hand = new Hand();
}

Player::~Player()
{
	
}

int Player::GetID()
{
	return i_PlayerID;
}

bool Player::HasCard( int iCardID )
{
	return p_Hand->HasCard(iCardID);
}

Card* Player::RemoveCard( int iCardID )
{
	Card* pCard = p_Hand->RemoveCard(iCardID);

	return pCard;
}

void Player::AddCard( Card* pCard )
{
	p_Hand->AddCard(pCard);
}

void Player::ComputeInitialKB()
{
	/*
	if ( p_Hand->Size() != 8 )
	{
		cout << "Hand doesn't have the required number of cards to init." << endl;
		return;
	}
	*/

	p_Hand->Sort();

	p_Hand->Begin();
	while( p_Hand->HasNext() )
	{
		Card* pCard = p_Hand->GetNext();
		
		int iCardID = pCard->GetID();
		int iKBKey = iCardID & (SUIT::SUIT_MASK | KIT::KIT_MASK);
		if ( map_KnowledgeBases.find(iKBKey) == map_KnowledgeBases.end() )
		{
			KnowledgeBase* pKB = new KnowledgeBase(iKBKey, i_PlayerID);
			pKB->InitKB(*p_Hand);
			map_KnowledgeBases[iKBKey] = pKB;
		}
	}

	b_KBInit = true;
}

void Player::ReceiveHand( Set<Card>* pSet )
{
	if (p_Hand)
	{
		pSet->Begin();

		for (int i = 0; i < pSet->Size(); ++i)
		{
			p_Hand->AddCard(pSet->GetNext());
		}
	}

	delete pSet;
}

void Player::ShowHand()
{
	cout << "             - Player " << i_PlayerID + 1 << "'s Hand -" << endl << endl;

	p_Hand->Sort();
	p_Hand->DebugPrint();

	//cout << endl << endl;
	cout << endl << "-----------------------------------------------" << endl << endl;
}

Set<Card>* Player::GetCardsOfType( int iKitID )
{
	int iSuit	= (iKitID & SUIT::SUIT_MASK);
	int iKit	= (iKitID & KIT::KIT_MASK);

	Set<Card>* pSet = new Set<Card>();

	for (p_Hand->Begin(); p_Hand->HasNext();)
	{
		Card* pCard = p_Hand->GetNext();
		if ( pCard->GetSuit() == iSuit && pCard->GetKit() == iKit )
		{
			pSet->PushBack(pCard);
		}
	}

	return pSet;
}

void Player::DebugShowKB( int iKitID )
{
	int iKBKey = iKitID & (SUIT::SUIT_MASK | KIT::KIT_MASK);
	if ( map_KnowledgeBases.find(iKBKey) != map_KnowledgeBases.end() )
	{
		map_KnowledgeBases[iKBKey]->DebugPrint();
	}
}

void Player::RemoveCardsOfType( int iKitID )
{
	map<int, KnowledgeBase*>::iterator iter = map_KnowledgeBases.find(iKitID);
	if (iter != map_KnowledgeBases.end())
	{
		int iSuit	= (iKitID & SUIT::SUIT_MASK);
		int iKit	= (iKitID & KIT::KIT_MASK);

		list<int> lst_CardIDs;

		for (p_Hand->Begin(); p_Hand->HasNext();)
		{
			Card* pCard = p_Hand->GetNext();
			if ( pCard->GetSuit() == iSuit && pCard->GetKit() == iKit )
			{
				lst_CardIDs.push_back(pCard->GetID());
			}
		}

		for (list<int>::iterator it = lst_CardIDs.begin(); it != lst_CardIDs.end(); ++it)
		{
			p_Hand->RemoveCard(*it);
		}

		map_KnowledgeBases.erase(iter);
	}
}
