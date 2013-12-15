#include "Game.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <cstdio>
#include <cassert>
#include "Globals.h"

using namespace std;

Game::Game()
{
	srand(time(NULL));

	i_ThisPlayer = 0;
//	int iDebugPlayer = 1;

	for (int i=0; i < NUM_PLAYERS; ++i)
	{
		if (i != i_ThisPlayer)
		{
			ap_Players[i] = new AIPlayer(i);
		}
	}

	ap_Players[ i_ThisPlayer ] = new HumanPlayer(i_ThisPlayer);
//	ap_Players[ iDebugPlayer ] = new HumanPlayer(iDebugPlayer);

	p_Deck = Deck::GetInstace();

	i_Turn = 0;
	b_Running = false;

	cout << "Starting Kitz..." << endl;
}

Game::~Game()
{

}

void Game::Init()
{
	p_Deck->Shuffle(5);

	//Debug
//	Set<Card>* pSet = p_Deck->GetSuit(SUIT::SPADES);
//	pSet->Shuffle(1);


	for (int i=0; i < NUM_PLAYERS; ++i)
	{
		ap_Players[i]->ReceiveHand( p_Deck->Deal() );
		ap_Players[i]->ComputeInitialKB();
	}


	/*
	int i=0; 
	for (pSet->Begin(); pSet->HasNext() && i < NUM_PLAYERS;)
	{
		Card* pCard1 = pSet->GetNext();
		Card* pCard2 = pSet->GetNext();

		ap_Players[i]->AddCard(pCard1);
		ap_Players[i]->AddCard(pCard2);

		ap_Players[i]->ComputeInitialKB();

		++i;
	}
	*/

	ai_Score[0] = 0;
	ai_Score[1] = 0;

	cout << "Game initialization complete." << endl;
}

void Game::Run()
{
	cout << "Running game..." << endl << endl;
	b_Running = true;
	int iContinue = -1;
	int iSession = 1;

	while(b_Running)
	{
		if (ai_Score[ i_ThisPlayer % 2 ] > KIT_WINNING_SCORE)
		{
			cout << "Congratulations! Your team won the game." << endl;
			b_Running = false;
		}
		else if ( ai_Score[ (i_ThisPlayer + 1) % 2 ] > KIT_WINNING_SCORE )
		{
			cout << "Opponent team won the game. Better luck next time." << endl;
			b_Running = false;
		}
		else if (ai_Score[ i_ThisPlayer % 2 ] == KIT_WINNING_SCORE || ai_Score[ (i_ThisPlayer+1) % 2 ] == KIT_WINNING_SCORE)
		{
			cout << "The game has end in a draw." << endl;
			b_Running = false;
		}
		else
		{
			cout << "================= Session " << iSession << " ===================" << endl << endl;
			ap_Players[i_Turn]->ShowHand();

			Move* pNextMove = ap_Players[i_Turn]->NextMove();
			assert(pNextMove->e_Type != Move::MoveType::UNDEFINED);

			if (pNextMove->e_Type == Move::MoveType::DROP_A_KIT)
			{
				DoDropKitRoutine(pNextMove);
			}
			else if (pNextMove->e_Type == Move::MoveType::ASK_A_CARD)
			{
				DoAskACardRoutine(pNextMove);
			}

			cout << "================= Session " << iSession++ << " End ===============" << endl << endl;

			cout << "Continue ?" << endl;
			cout << "1. Yes		2. No" << endl;
			cin >> iContinue;

			b_Running = iContinue == 2 ? false : true;

			cout << endl;
		}
	}

	cout << "Ending game..." << endl;
}

void Game::DoDropKitRoutine( Move* pNextMove )
{
	cout << "<*> Player " << pNextMove->i_InitiatorID + 1 << " is going to drop " <<  SYMBOL::GetKitSymbol(pNextMove->i_KitType) << " kit." << endl << endl;

	bool bKitDropSuccessful = true;

	Set<Card>* pKit = Deck::GetInstace()->GetKit(pNextMove->i_KitType);
	for ( pKit->Begin(); pKit->HasNext(); )
	{
		Card* pCard = pKit->GetNext();
		// if the current player doesn't have the card
		if ( !ap_Players[i_Turn]->HasCard(pCard->GetID()) )
		{
			cout << "Which player has the card " << SYMBOL::GetCardSymbol(pCard->GetID(), false) << "?" << endl;
			int iPlayer = -1;
			do 
			{
				cout << "1. Player 1		2. Player 2"	<< endl;
				cout << "3. Player 3		4. Player 4"	<< endl;
				cout << "5. Player 5		6. Player 6"	<< endl << endl;
				cout << "Select Player: ";
				cin >> iPlayer;
			} 
			while (iPlayer < 1 || iPlayer > 6);
			cout << endl;

			// if the selected player doesn't have the card
			if ( !ap_Players[iPlayer - 1]->HasCard(pCard->GetID()) )
			{
				cout << "Player " << iPlayer << " doesn't have that card." << endl;
				cout << SYMBOL::GetKitSymbol(pNextMove->i_KitType) << " kit drop failed." << endl << endl;

				i_Turn = (i_Turn + 1) % NUM_PLAYERS;
				bKitDropSuccessful = false;
				
				break;
			}
			else
			{
				cout << "Correct!" << endl;
			}
		}
	}

	if (bKitDropSuccessful)
	{
		cout << "Player " << pNextMove->i_InitiatorID + 1 << " successfully dropped " << SYMBOL::GetKitSymbol(pNextMove->i_KitType) << " kit." << endl << endl;
	}

	// Remove all the cards of this kit from all the players
	// regardless of its success of failure
	for (int i = 0; i < NUM_PLAYERS; ++i)
	{
		ap_Players[i]->RemoveCardsOfType(pNextMove->i_KitType);
	}

	// Once DropKitRoutine() is executed, one of the teams definitely scores.
	// Based on the success or failure of the drop, the current turn is set
	// Whoever has the new turn will get the score
	if ( (pNextMove->i_KitType & KIT::KIT_MASK) == KIT::MAJOR )
	{
		ai_Score[i_Turn % 2] += KIT_MAJOR_SCORE;
	}
	else
	{
		ai_Score[i_Turn % 2] += KIT_MINOR_SCORE;
	}

	cout << "    - Current Score -" << endl;
	cout << " Your Team	Opponent Team"	<< endl;
	cout << "-----------------------------"	<< endl;
	cout << "     " << ai_Score[i_ThisPlayer % 2] << "       |        " << ai_Score[(i_ThisPlayer+1) % 2] << "     |" << endl;
	cout << "-----------------------------"	<< endl << endl;
}

void Game::DoAskACardRoutine( Move* pNextMove )
{
	cout << "<*> Player " << pNextMove->i_InitiatorID + 1 << " asked for " << SYMBOL::GetCardSymbol(pNextMove->i_CardID, false) 
		<< " from Player " << pNextMove->i_AffecteeID + 1 << endl;

	if ( ap_Players[pNextMove->i_AffecteeID]->HasCard(pNextMove->i_CardID) )
	{
		pNextMove->b_Successful = true;

		Card* pCard = ap_Players[pNextMove->i_AffecteeID]->RemoveCard(pNextMove->i_CardID);
		ap_Players[pNextMove->i_InitiatorID]->AddCard(pCard);

		cout << "<*> Player " << pNextMove->i_InitiatorID + 1 << " successfully received it." << endl << endl;
	}
	else
	{
		i_Turn = pNextMove->i_AffecteeID;

		cout << "<*> Player " << pNextMove->i_InitiatorID + 1 << " failed to receive it." << endl << endl;
	}

	ap_Players[pNextMove->i_AffecteeID]->ShowHand();

	pNextMove->b_Processed = true;

	for (int i=0; i < NUM_PLAYERS; ++i)
	{
		ap_Players[i]->UpdateKB(pNextMove);
	}

	ap_Players[pNextMove->i_InitiatorID]->DebugShowKB(pNextMove->i_CardID);
	ap_Players[pNextMove->i_AffecteeID]->DebugShowKB(pNextMove->i_CardID);
}
