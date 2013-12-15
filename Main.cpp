#include "Globals.h"
#include <conio.h>
#include <iostream>
#include "Card.h"
#include "Set.h"
#include "Deck.h"
#include "DifferentialFilter.h"
#include "KnowledgeBase.h"
#include "Game.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"

using namespace std;

int main()
{
	Game oGame;
	oGame.Init();

	oGame.Run();

// 	AIPlayer oPlayer(0);
// 	Card* pCard = new Card(SUIT::CLUBS || KIT::MAJOR || CARD::TEN);
// 	oPlayer.AddCard(pCard);

	getch();
	return 0;
}