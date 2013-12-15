#ifndef _HUMAN_PLAYER_HEADER_
#define _HUMAN_PLAYER_HEADER_

#include "AIPlayer.h"
#include "Player.h"

class HumanPlayer : public AIPlayer
{
public:
	HumanPlayer(int iPlayerID);
	virtual ~HumanPlayer();

//	void	UpdateKB(Move* pMove);
	Move*	NextMove();

protected:
	Move*	DropKit( int iKitID );
	Move*	AskForCard();
private:

	int		GetKitFromUser();
};

#endif // _HUMAN_PLAYER_HEADER_