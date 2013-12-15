#include "KnowledgeBase.h"
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

KnowledgeBase::KnowledgeBase( int iKitID, int iPlayerID )
{
	i_KitID			= iKitID;
	i_ThisPlayerID	= iPlayerID;

	for (int iCard = 0; iCard < NUM_CARDS; ++iCard)
	{
		for (int iPlayer = 0; iPlayer < NUM_PLAYERS; ++iPlayer)
		{
			ai_StateMatrix[iCard][iPlayer] = STATE::NOT_CHECKED;
			ai_NotCheckedCardCount[iPlayer] = ai_NotCheckedCardCount[iPlayer] + 1;
		}
	}

	for (int i=0; i<NUM_PLAYERS; ++i)
	{
		ai_HaveCardCount[i]			= 0;
		ai_ProbablyCardCount[i]		= 0;
		ai_UsedToCardCount[i]		= 0;
		ai_NotCheckedCardCount[i]	= 0;
	}

	//Kit Strength parameters
	map_KitStrengthParams[KIT_PARAM::MAJOR]			= 3;
	map_KitStrengthParams[KIT_PARAM::MINOR]			= 2;
	map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_1] = 1;
	map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_2] = 2;
	map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_3] = 4;
	map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_4] = 7;
	map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_5] = 10;
}

KnowledgeBase::~KnowledgeBase()
{

}

void KnowledgeBase::InitKB( Set<Card> oSet )
{
	oSet.Begin();
	while( oSet.HasNext() )
	{
		Card* pCard = oSet.GetNext();
		if ( i_KitID == ( pCard->GetID() & (SUIT::SUIT_MASK | KIT::KIT_MASK) ) )
		{
			SetState(i_ThisPlayerID, pCard->GetID(), STATE::HAVE);
			for(int i=0; i<NUM_PLAYERS; ++i)
			{
				if ( i != i_ThisPlayerID )
				{
					SetState(i, pCard->GetID(), STATE::DONT_HAVE);
				}
			}
		}
	}

	// There cannot be NOT_CHECKED states in this player's positions, turn them to DONT_HAVE
	for (int i = 0; i < NUM_CARDS; ++i)
	{
		if ( GetState(i_ThisPlayerID, GetCardIDFromIndex(i)) == STATE::NOT_CHECKED )
		{
			SetState(i_ThisPlayerID, GetCardIDFromIndex(i), STATE::DONT_HAVE);
		}
	}
}

void KnowledgeBase::SetState( int iPlayerID, int iCardID, int iState )
{
	assert( iPlayerID >= 0 && iPlayerID <= 5 );
	assert( iState >= 0 && iState <= 4 );

	int iCardIndex = GetCardIndex(iCardID);
	int iPrevState = ai_StateMatrix[iCardIndex][iPlayerID];

	if ( iPrevState == STATE::NOT_CHECKED && iState == STATE::HAVE )
	{
		// state change from NOT_CHECKED to HAVE
		ai_NotCheckedCardCount[iPlayerID] = ai_NotCheckedCardCount[iPlayerID] - 1;
		ai_HaveCardCount[iPlayerID] = ai_HaveCardCount[iPlayerID] + 1;
	}
	else if ( iPrevState == STATE::NOT_CHECKED && iState == STATE::PROBABLY_HAVE )
	{
		// state change from NOT_CHECKED to PROBABLY_HAVE
		ai_NotCheckedCardCount[iPlayerID] = ai_NotCheckedCardCount[iPlayerID] - 1;
		ai_ProbablyCardCount[iPlayerID] = ai_ProbablyCardCount[iPlayerID] + 1;
	}
	else if ( iPrevState == STATE::NOT_CHECKED && iState == STATE::DONT_HAVE )
	{
		// state change from NOT_CHECKED to DONT_HAVE
		ai_NotCheckedCardCount[iPlayerID] = ai_NotCheckedCardCount[iPlayerID] - 1;
	}
	else if ( iPrevState == STATE::USED_TO_HAVE && iState == STATE::PROBABLY_HAVE )
	{
		// state change from USED_TO_HAVE to PROBABLY_HAVE
		ai_ProbablyCardCount[iPlayerID] = ai_ProbablyCardCount[iPlayerID] + 1;
		ai_UsedToCardCount[iPlayerID] = ai_UsedToCardCount[iPlayerID] - 1;
	}
	else if ( iPrevState == STATE::PROBABLY_HAVE && iState == STATE::USED_TO_HAVE )
	{
		// state change from PROBABLY_HAVE to USED_TO_HAVE
		ai_ProbablyCardCount[iPlayerID] = ai_ProbablyCardCount[iPlayerID] - 1;
		ai_UsedToCardCount[iPlayerID] = ai_UsedToCardCount[iPlayerID] + 1;
	}
	else if ( iPrevState == STATE::PROBABLY_HAVE && iState == STATE::HAVE )
	{
		// inference condition
		ai_HaveCardCount[iPlayerID] = ai_HaveCardCount[iPlayerID] + 1;
		ai_ProbablyCardCount[iPlayerID] = ai_ProbablyCardCount[iPlayerID] - 1;
	}
	else if ( iPrevState == STATE::HAVE && iState == STATE::DONT_HAVE )
	{
		// if a card is taken from the player, reduce the count by one
		ai_HaveCardCount[iPlayerID] = ai_HaveCardCount[iPlayerID] - 1;
	}
	else if ( iPrevState == STATE::DONT_HAVE && iState == STATE::HAVE )
	{
		// if a card is added, increase the count by one
		ai_HaveCardCount[iPlayerID] = ai_HaveCardCount[iPlayerID] + 1;
	}

	ai_StateMatrix[iCardIndex][iPlayerID] = iState;

}

int KnowledgeBase::GetState( int iPlayerID, int iCardID )
{
	assert( iPlayerID >= 0 && iPlayerID <= 5 );

	int iCardIndex = GetCardIndex(iCardID);

	return ai_StateMatrix[iCardIndex][iPlayerID];
}

int KnowledgeBase::GetCardIndex( int iCardID )
{
	int iCardVal = (iCardID & CARD::CARD_MASK);

	if ( iCardVal < CARD::NINE ) // MINOR 
	{
		return (iCardVal - 1);
	}
	
	return (iCardVal - 7);
}

void KnowledgeBase::DebugPrint()
{
	bool bArrowHead = false;
	for(int i = 0; i < NUM_PLAYERS; ++i)
	{	
		if ( i == i_ThisPlayerID )
		{
			if (i == 0) 
				cout << setw(10);
			else
				cout << setw(12);

			cout << "*";
			bArrowHead = true;
		}
		else
		{
			if (bArrowHead)
			{
				cout << setw(12);
				bArrowHead = false;
			}
			else
			{
				if (i == 0) 
					cout << setw(10);
				else
					cout << setw(12);

				cout << " ";
			}
		}
	}

	cout << endl;

	for(int i = 0; i < NUM_PLAYERS; ++i)
	{		
		cout << setw(10);
		cout << "[P " << i+1 << "]";
	}

	cout << endl << endl;

	for (int iCard = 0; iCard < NUM_CARDS; ++iCard)
	{
		int iCardID = GetCardIDFromIndex(iCard);
		cout << SYMBOL::GetCardSymbol(iCardID, false);
		
		if ( (iCardID & CARD::CARD_MASK) == CARD::TEN )
			cout << setw(4);
		else
			cout << setw(5);

		for (int iPlayer = 0; iPlayer < NUM_PLAYERS; ++iPlayer)
		{
			int iState = ai_StateMatrix[iCard][iPlayer];
			switch(iState)
			{
			case STATE::NOT_CHECKED:
				cout << ".";
				break;
			case STATE::DONT_HAVE:
				cout << "X";
				break;
			case STATE::HAVE:
				cout << "H";
				break;
			case STATE::PROBABLY_HAVE:
				cout << "P";
				break;
			case STATE::USED_TO_HAVE:
				cout << "U";
				break;
			}

			if (iPlayer < NUM_PLAYERS - 1) cout << setw(12);
		}

		cout << endl << endl;
	}
}

int KnowledgeBase::GetCardIDFromIndex( int iIndex )
{
	int iKit = (i_KitID & KIT::KIT_MASK);

	if (iKit == KIT::MAJOR)
	{
		return ( i_KitID | (iIndex + 7) );
	}

	//MINOR
	return ( i_KitID | (iIndex + 1) );
}

bool KnowledgeBase::CanDropKit()
{
	int iCardCout = 0;
	for (int i=0; i < NUM_PLAYERS; ++i)
	{
		iCardCout += ai_HaveCardCount[i];
	}

	return ( iCardCout == NUM_CARDS ? true : false );
}

int KnowledgeBase::GetKitStrength()
{
	int iStrength = 1;

	if ( (i_KitID & KIT::KIT_MASK) == KIT::MAJOR )
	{
		iStrength *= map_KitStrengthParams[KIT_PARAM::MAJOR];
	}
	else
	{
		iStrength *= map_KitStrengthParams[KIT_PARAM::MINOR];
	}

	int iCardCout = 0;
	for (int i=0; i < NUM_PLAYERS; ++i)
	{
		iCardCout += ai_HaveCardCount[i];
	}

	switch(iCardCout)
	{
	case 0:
		iStrength = 0;
		break;
	case 1:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_1];
		break;
	case 2:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_2];
		break;
	case 3:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_3];
		break;
	case 4:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_4];
		break;
	case 5:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_5];
		break;
	case 6:
		iStrength *= map_KitStrengthParams[KIT_PARAM::CARDS_KNOWN_6];
		break;
	default:
		iStrength *= -1;
	}

	return iStrength;
}
