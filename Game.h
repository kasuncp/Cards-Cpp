#ifndef _GAME_HEADER_
#define _GAME_HEADER_

#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "Deck.h"

#define KIT_MAJOR_SCORE 3
#define KIT_MINOR_SCORE 2
#define KIT_WINNING_SCORE 10

class Game 
{
public:
	Game();
	~Game();

	void Init();
	void Run();

protected:
	void DoDropKitRoutine( Move* pNextMove );
	void DoAskACardRoutine( Move* pNextMove );

private:
	Player* ap_Players[NUM_PLAYERS];
	Deck* p_Deck;

	int i_Turn;
	int i_ThisPlayer;
	bool b_Running;
	int ai_Score[2];
};

#endif // _GAME_HEADER_