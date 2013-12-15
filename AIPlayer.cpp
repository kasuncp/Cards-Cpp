#include "AIPlayer.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#include "Deck.h"

using namespace std;

AIPlayer::AIPlayer( int iPlayerID ) : Player(iPlayerID)
{
	srand(time(NULL));
	p_DiffFilter = new DifferentialFilter();
}

AIPlayer::~AIPlayer()
{

}

void AIPlayer::UpdateKB( Move* pMove )
{
	if ( pMove->b_Processed )
	{
		// Step 1. Check if we are interested in this kit
		//================================================
		int iCard = pMove->i_CardID;
		int iKitID = ( iCard & (SUIT::SUIT_MASK | KIT::KIT_MASK) );

		map<int, KnowledgeBase*>::iterator iter = map_KnowledgeBases.find(iKitID);
		if ( iter == map_KnowledgeBases.end() )
		{
			// We didn't have any cards from this kit from the beginning,
			// or we had cards of this kit earlier, but they were all taken from us
			// So ignore.
			return;
		}
		
// 		bool bHasCardsLeft = false;
// 		for (p_Hand->Begin(); p_Hand->HasNext(); )
// 		{
// 			if ( iKitID == (p_Hand->GetNext()->GetID() & (SUIT::SUIT_MASK | KIT::KIT_MASK)) )
// 			{
// 				bHasCardsLeft = true;
// 				break;
// 			}
// 		}

		bool bHasCardsLeft = iter->second->HaveCardsLeft(i_PlayerID);
		if ( !bHasCardsLeft )
		{
			// We have no cards left of this kit.
			// So erase if from the KB and ignore
			map_KnowledgeBases.erase(iter); // version 2.8 - Issue 2 solution: If no cards left, remove the kit from the KB
			return;
		}

		// Step 2: Check the True/False value of Move
		// and pass the control to each scenario
		//================================================

		KnowledgeBase* pKB = map_KnowledgeBases[iKitID];

		// Refer the AI document for scenarios 1 and 2
		if (pMove->b_Successful)
		{
			AIScenario1(iKitID, pKB, pMove);
		}
		else
		{
			AIScenario2(iKitID, pKB, pMove);
		}

		DoInference(pKB);
	}
}

Move* AIPlayer::NextMove()
{
	int iKitDropped = false;
	Move* pMove = NULL;

	map<int, KnowledgeBase*>::iterator iter;
	for (iter = map_KnowledgeBases.begin(); iter != map_KnowledgeBases.end(); ++iter)
	{
		if ( iter->second->CanDropKit() )
		{
			pMove = DropKit( iter->second->GetKitID() );
			iKitDropped = true;
			break;
		}
	}

	if ( !iKitDropped )
	{
//		pMove = AskForCard();
		pMove = AskForCardV2();
	}

	return pMove;
}

void AIPlayer::AIScenario1( int iKitID, KnowledgeBase* pKB, Move* pMove )
{
	// Scenario 1 - Step 1
	//=====================

	// Set HAVE state to the initiator and DONT_HAVE to all others
	for (int iPlayer = 0; iPlayer < NUM_PLAYERS; ++iPlayer)
	{
		pKB->SetState(iPlayer, pMove->i_CardID, STATE::DONT_HAVE);
	}

	pKB->SetState(pMove->i_InitiatorID, pMove->i_CardID, STATE::HAVE);

	// Scenario 1 - Step 2
	//=====================

	Set<Card>* pKit = Deck::GetInstace()->GetKit(iKitID);
	bool bHave = false;

	for ( pKit->Begin(); pKit->HasNext(); )
	{
		Card* pCard = pKit->GetNext();
		int iState;

		// The initiator (if not me) must have at least one other card of this kit,
		// so update other positions to PROBABLY_HAVE
		if ( pMove->i_InitiatorID != i_PlayerID )
		{	
			iState = pKB->GetState(pMove->i_InitiatorID, pCard->GetID());
			if ( iState == STATE::NOT_CHECKED || iState == STATE::USED_TO_HAVE )
			{
				pKB->SetState(pMove->i_InitiatorID, pCard->GetID(), STATE::PROBABLY_HAVE );
			}
		}

		// Scenario 1 - Step 3
		// ===================

		// Check if the affectee has other cards of that kit left
		iState = pKB->GetState(pMove->i_AffecteeID, pCard->GetID());
		if ( iState == STATE::HAVE )
		{
			bHave = true;
		}
	}

	// If the affectee (if not me) doesn't have any cards of that kit left,
	// change all the PROBABLY_HAVEs to USED_TO_HAVEs
	if ( pMove->i_AffecteeID != i_PlayerID )
	{
		if ( !bHave )
		{
			for ( pKit->Begin(); pKit->HasNext(); )
			{
				Card* pCard = pKit->GetNext();
				int iState = pKB->GetState(pMove->i_AffecteeID, pCard->GetID());
				if ( iState == STATE::PROBABLY_HAVE )
				{
					pKB->SetState(pMove->i_AffecteeID, pCard->GetID(), STATE::USED_TO_HAVE );
				}
			}
		}
	}

}

void AIPlayer::AIScenario2( int iKitID, KnowledgeBase* pKB, Move* pMove )
{
	// Scenario 2 - Step 1
	//=====================

	// None of them has the card
	pKB->SetState(pMove->i_InitiatorID, pMove->i_CardID, STATE::DONT_HAVE);
	pKB->SetState(pMove->i_AffecteeID, pMove->i_CardID, STATE::DONT_HAVE);

	// Scenario 2 - Step 1
	//=====================

	if ( pMove->i_InitiatorID != i_PlayerID )
	{
		Set<Card>* pKit = Deck::GetInstace()->GetKit(iKitID);
		bool bHave = false;

		for ( pKit->Begin(); pKit->HasNext(); )
		{
			Card* pCard = pKit->GetNext();
			int iState = pKB->GetState(pMove->i_InitiatorID, pCard->GetID());
			if ( iState == STATE::HAVE )
			{
				bHave = true;
				break;
			}
		}

		if ( !bHave )
		{
			// In order for the initiator to ask a card of this kit, 
			// he must have at least one other card of that type
			for ( pKit->Begin(); pKit->HasNext(); )
			{
				Card* pCard = pKit->GetNext();
				int iState = pKB->GetState(pMove->i_InitiatorID, pCard->GetID());
				if ( iState == STATE::NOT_CHECKED || iState == STATE::USED_TO_HAVE )
				{
					pKB->SetState(pMove->i_InitiatorID, pCard->GetID(), STATE::PROBABLY_HAVE );
				}
			}
		}
	}
}

void AIPlayer::DoInference( KnowledgeBase* pKB )
{
	Set<Card>* pKit = Deck::GetInstace()->GetKit(pKB->GetKitID());

	// TODO
	// Improve the following part using ai_CardCount[] of KnowledgeBase
	for (int iPlayerID = 0; iPlayerID < NUM_PLAYERS; ++iPlayerID)
	{
		if (iPlayerID != i_PlayerID)
		{
			int iPCount = 0;
			int iPValCardID = -1;
			bool bContinue = true;

			for(pKit->Begin(); pKit->HasNext();)
			{
				Card* pCard = pKit->GetNext();
				int iState = pKB->GetState(iPlayerID, pCard->GetID());
				if ( iState == STATE::NOT_CHECKED || iState == STATE::USED_TO_HAVE )
				{
					bContinue = false;
					break;
				}

				if (iState == STATE::PROBABLY_HAVE)
				{
					++iPCount;
					iPValCardID = pCard->GetID();
				}
			}
			
			if (bContinue && (iPCount == 1))
			{
				// If there are no NC or U values and
				// there is only one P value, use inference

				// Set all the other players to DONT_HAVE and iPlayerID to HAVE
				for (int i=0; i < NUM_PLAYERS; ++i)
				{
					pKB->SetState(i, iPValCardID, STATE::DONT_HAVE);
				}

				pKB->SetState(iPlayerID, iPValCardID, STATE::HAVE);
			}
		}
	}
}

void AIPlayer::PrintKB()
{
	map<int, KnowledgeBase*>::iterator iter;
	for (iter = map_KnowledgeBases.begin(); iter != map_KnowledgeBases.end(); ++iter)
	{
		iter->second->DebugPrint();
	}
}

Move* AIPlayer::DropKit( int iKitID )
{
	Move* pNextMove = new Move();
	pNextMove->e_Type = Move::MoveType::DROP_A_KIT;
	pNextMove->i_InitiatorID = i_PlayerID;
	pNextMove->i_KitType = iKitID;

	return pNextMove;
}

Move* AIPlayer::AskForCard()
{
	// The simplest logic is implemented here
	// Which is randomly selecting cards and players
	int iTeamID = i_PlayerID % 2;

	int iPlayer;

	// Select only opponents
	do 
	{
		iPlayer = rand() % NUM_PLAYERS;
	}
	while ( (iPlayer % 2) == iTeamID );

	bool bSelectOtherCard;
	int iSelectedCardID = -1;

	do 
	{
		bSelectOtherCard = false;

		int iRandKit	= map_KnowledgeBases.size();
		int iKit		= ( rand() % iRandKit );

		// Select kit
		map<int, KnowledgeBase*>::iterator iter;
		int iCount = 0;
		for (iter = map_KnowledgeBases.begin(); iter != map_KnowledgeBases.end(); ++iter)
		{
			if (iCount == iKit)
			{
				break;
			}

			++iCount;
		}

		int iKitID = iter->second->GetKitID();
		Set<Card>* pSet = GetCardsOfType(iKitID);

		Set<Card>* pResultSet = p_DiffFilter->Enter(pSet);

		// ======== AI Improvement - version 2.8 - improvement 1 ===========

		Card* pResultCard = NULL;
		int ai_PHStates[NUM_CARDS];
		int ai_UStates[NUM_CARDS];
		int ai_NCStates[NUM_CARDS];
		int iPHCount = 0;
		int iUCount = 0;
		int iNCCount = 0;

		for (pResultSet->Begin(); pResultSet->HasNext();)
		{
			pResultCard = pResultSet->GetNext();

			int iState = map_KnowledgeBases[iKitID]->GetState(iPlayer, pResultCard->GetID());
			if ( iState == STATE::PROBABLY_HAVE )
			{
				ai_PHStates[iPHCount++] =  pResultCard->GetID();
			}
			else if ( iState == STATE::USED_TO_HAVE )
			{
				ai_UStates[iUCount++] =  pResultCard->GetID();
			}
			else if ( iState == STATE::NOT_CHECKED )
			{
				ai_NCStates[iNCCount++] = pResultCard->GetID();
			}
		}

		if ( iPHCount > 0 )
		{
			// Randomly select a card from the PROBABLY_HAVE set
			int iRandCardIndex = rand() % iPHCount;
			iSelectedCardID = ai_PHStates[iRandCardIndex];
		}
		else if ( iUCount > 0 )
		{
			// Randomly select a card from the USED_TO_HAVE set
			int iRandCardIndex = rand() % iUCount;
			iSelectedCardID = ai_UStates[iRandCardIndex];
		}
		else if ( iNCCount > 0 )
		{
			// Randomly select a card from the NOT_CHECKED set
			int iRandCardIndex = rand() % iNCCount;
			iSelectedCardID = ai_NCStates[iRandCardIndex];
		}
		else
		{
			// This scenario means there is no card of this kit worth asking
			bSelectOtherCard = true;
		}
	} 
	while ( bSelectOtherCard );
	// ==================================================================

	Move* pNextMove = new Move();
	pNextMove->e_Type = Move::MoveType::ASK_A_CARD;
	pNextMove->i_InitiatorID = i_PlayerID;
	pNextMove->i_AffecteeID = iPlayer;
	pNextMove->i_CardID = iSelectedCardID;

	return pNextMove;
}

// This version doesn't consider about helping teammates.
Move* AIPlayer::AskForCardV2()
{
	map<int, KnowledgeBase*>::iterator iter;
	int iBestKitID			= -1;
	int iHighestStrength	= 0;
	for (iter = map_KnowledgeBases.begin(); iter != map_KnowledgeBases.end(); ++iter)
	{
		int iCurrStrength = iter->second->GetKitStrength();
		if ( iCurrStrength > iHighestStrength )
		{
			iHighestStrength = iCurrStrength;
			iBestKitID = iter->first;
		}
	}

	// By this time the kit with the highest strength is selected
	Set<Card>* pSet = GetCardsOfType(iBestKitID);
	Set<Card>* pResultSet = p_DiffFilter->Enter(pSet);

	int iTeamID = i_PlayerID % 2;
	int iBestPlayerID = -1;

	for (int iPlayerID = 0; iPlayerID < NUM_PLAYERS; ++iPlayerID)
	{
		// if the player is in the opposing team
		if ( (iPlayerID % 2) != iTeamID )
		{
			// if the player has or suspects he has cards of the interested kit
			KnowledgeBase* pKB = map_KnowledgeBases[iBestKitID];
			if ( 
				 pKB->HaveCardsLeft(iPlayerID)			||
				 pKB->ProbablyHaveCardsLeft(iPlayerID)	||
				 pKB->UsedToHaveCardsLeft(iPlayerID)	||
				 pKB->NotCheckedCardsLeft(iPlayerID)
				)
			{
				iBestPlayerID = iPlayerID;
			}
		}
	}

	// if there are no players selected from the above method, go for a random player
	if ( iBestPlayerID == -1 )
	{
		do 
		{
			iBestPlayerID = rand() % NUM_PLAYERS;
		}
		while ( (iBestPlayerID % 2) == iTeamID );
	}

	// ======== AI Improvement - version 2.8 - improvement 1 ===========

	Card* pResultCard = NULL;
	int iSelectedCardID = -1;

	int ai_HStates[NUM_CARDS];
	int ai_PHStates[NUM_CARDS];
	int ai_UStates[NUM_CARDS];
	int ai_NCStates[NUM_CARDS];
	int iHCount		= 0;
	int iPHCount	= 0;
	int iUCount		= 0;
	int iNCCount	= 0;

	for (pResultSet->Begin(); pResultSet->HasNext();)
	{
		pResultCard = pResultSet->GetNext();

		int iState = map_KnowledgeBases[iBestKitID]->GetState(iBestPlayerID, pResultCard->GetID());
		if ( iState == STATE::HAVE )
		{
			ai_HStates[iHCount++] =  pResultCard->GetID();
		}
		else if ( iState == STATE::PROBABLY_HAVE )
		{
			ai_PHStates[iPHCount++] =  pResultCard->GetID();
		}
		else if ( iState == STATE::USED_TO_HAVE )
		{
			ai_UStates[iUCount++] =  pResultCard->GetID();
		}
		else if ( iState == STATE::NOT_CHECKED )
		{
			ai_NCStates[iNCCount++] = pResultCard->GetID();
		}
	}

	if ( iHCount > 0 )
	{
		// Randomly select a card from the HAVE set
		int iRandCardIndex = rand() % iHCount;
		iSelectedCardID = ai_HStates[iRandCardIndex];
	}
	else if ( iPHCount > 0 )
	{
		// Randomly select a card from the PROBABLY_HAVE set
		int iRandCardIndex = rand() % iPHCount;
		iSelectedCardID = ai_PHStates[iRandCardIndex];
	}
	else if ( iUCount > 0 )
	{
		// Randomly select a card from the USED_TO_HAVE set
		int iRandCardIndex = rand() % iUCount;
		iSelectedCardID = ai_UStates[iRandCardIndex];
	}
	else if ( iNCCount > 0 )
	{
		// Randomly select a card from the NOT_CHECKED set
		int iRandCardIndex = rand() % iNCCount;
		iSelectedCardID = ai_NCStates[iRandCardIndex];
	}
	else
	{
		// This scenario means there is no card of this kit worth asking
		//bSelectOtherCard = true;
	}
// ==================================================================

	Move* pNextMove = new Move();
	pNextMove->e_Type = Move::MoveType::ASK_A_CARD;
	pNextMove->i_InitiatorID = i_PlayerID;
	pNextMove->i_AffecteeID = iBestPlayerID;
	pNextMove->i_CardID = iSelectedCardID;

	return pNextMove;
}
