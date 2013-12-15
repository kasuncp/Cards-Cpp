#include "HumanPlayer.h"
#include "Globals.h"
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer( int iPlayerID ) : AIPlayer(iPlayerID)
{

}

HumanPlayer::~HumanPlayer()
{

}

/*
void HumanPlayer::UpdateKB( Move* pMove )
{

}
*/

// Validations are not handled in this implementation
Move* HumanPlayer::NextMove()
{
	int iChoice = -1;
	do
	{
		cout << "1. Drop a Kit			2. Ask for a Card"		<< endl;
		cout << "Select your move: ";
		cin >> iChoice;
	}
	while( iChoice < 1 || iChoice > 2 );

	cout << endl;

	if (iChoice == 1)
	{
		return DropKit( GetKitFromUser() );
	}
	else if (iChoice == 2)
	{
		return AskForCard();
	}

	return NULL;
}

Move* HumanPlayer::DropKit( int iKitID )
{
	Move* pNextMove = new Move();
	pNextMove->e_Type = Move::MoveType::DROP_A_KIT;
	pNextMove->i_InitiatorID = i_PlayerID;
	pNextMove->i_KitType = iKitID;

	return pNextMove;
}

Move* HumanPlayer::AskForCard()
{
	int iSuit	= -1;
	int iKit	= -1;
	int iCard	= -1;

	iKit = GetKitFromUser();
	
	if ( (iKit & KIT::KIT_MASK) == KIT::MAJOR )
	{
		do 
		{
			cout << "1. Nine			2. Ten"		<< endl;
			cout << "3. Jack			4. Queen"	<< endl;
			cout << "5. King			6. Ace"		<< endl << endl;
			cout << "Select Card: ";
			cin >> iCard;
		} 
		while (iCard < 1 || iCard > 6);

	}
	else
	{
		do 
		{
			cout << "1. Three			2. Four"	<< endl;
			cout << "3. Five				4. Six"	<< endl;
			cout << "5. Seven			6. Eight"	<< endl << endl;
			cout << "Select Card: ";
			cin >> iCard;
		} 
		while (iCard < 1 || iCard > 6);
	}

	cout << endl;

	int iSelectedCard = iKit;

	if ( (iKit & KIT::KIT_MASK) == KIT::MAJOR )
	{
		switch(iCard)
		{
		case 1:
			iSelectedCard = (iSelectedCard | CARD::NINE);
			break;

		case 2:
			iSelectedCard = (iSelectedCard | CARD::TEN);
			break;

		case 3:
			iSelectedCard = (iSelectedCard | CARD::JACK);
			break;

		case 4:
			iSelectedCard = (iSelectedCard | CARD::QUEEN);
			break;

		case 5:
			iSelectedCard = (iSelectedCard | CARD::KING);
			break;

		case 6:
			iSelectedCard = (iSelectedCard | CARD::ACE);
			break;
		}
	}
	else
	{
		switch(iCard)
		{
		case 1:
			iSelectedCard = (iSelectedCard | CARD::THREE);
			break;

		case 2:
			iSelectedCard = (iSelectedCard | CARD::FOUR);
			break;

		case 3:
			iSelectedCard = (iSelectedCard | CARD::FIVE);
			break;

		case 4:
			iSelectedCard = (iSelectedCard | CARD::SIX);
			break;

		case 5:
			iSelectedCard = (iSelectedCard | CARD::SEVEN);
			break;

		case 6:
			iSelectedCard = (iSelectedCard | CARD::EIGHT);
			break;
		}
	}

	// select player
	int iPlayer = -1;
	do 
	{
		cout << " Your Team		Opponent Team"	<< endl;
		cout << "-------------------------------------"	<< endl;
		cout << "1. Player 1		2. Player 2"	<< endl;
		cout << "3. Player 3		4. Player 4"	<< endl;
		cout << "5. Player 5		6. Player 6"	<< endl << endl;
		cout << "Select Player: ";
		cin >> iPlayer;
	} 
	while (iPlayer < 1 || iPlayer > 6);

	cout << endl;

	int iAffectee = iPlayer - 1;

	Move* pNextMove = new Move();
	pNextMove->e_Type = Move::MoveType::ASK_A_CARD;
	pNextMove->i_InitiatorID = i_PlayerID;
	pNextMove->i_AffecteeID = iAffectee;
	pNextMove->i_CardID = iSelectedCard;

	return pNextMove;
}

int HumanPlayer::GetKitFromUser()
{
	int iSuit	= -1;
	int iKit	= -1;

	do
	{
		cout << "1. Spades			2. Hearts"		<< endl;
		cout << "3. Clubs			4. Diamonds"	<< endl << endl;
		cout << "Select Suit: ";
		cin >> iSuit;
	}
	while( iSuit < 1 || iSuit > 4 );

	cout << endl;

	do
	{
		cout << "1. Major			2. Minor"		<< endl << endl;
		cout << "Select Kit: ";
		cin >> iKit;
	}
	while (iKit < 1 || iKit > 2);

	cout << endl;

	int iSelectedKit = 0;

	switch (iSuit)
	{
	case 1:
		iSelectedKit = (iSelectedKit | SUIT::SPADES);
		break;

	case 2:
		iSelectedKit = (iSelectedKit | SUIT::HEARTS);
		break;

	case 3:
		iSelectedKit = (iSelectedKit | SUIT::CLUBS);
		break;

	case 4:
		iSelectedKit = (iSelectedKit | SUIT::DIAMONDS);
		break;
	}

	switch(iKit)
	{
	case 1:
		iSelectedKit = (iSelectedKit | KIT::MAJOR);
		break;

	case 2:
		iSelectedKit = (iSelectedKit | KIT::MINOR);
		break;
	}

	return iSelectedKit;
}
