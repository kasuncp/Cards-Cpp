#ifndef _AI_PLAYER_HEADER_
#define _AI_PLAYER_HEADER_

#include "Player.h"
#include "DifferentialFilter.h"

class AIPlayer : public Player
{
public:
	AIPlayer(int iPlayerID);
	virtual ~AIPlayer();

	void	UpdateKB(Move* pMove);
	Move*	NextMove();

	void	PrintKB();

protected:
	Move*	DropKit( int iKitID );
	Move*	AskForCard();
	Move*	AskForCardV2();
	void	DoInference(KnowledgeBase* pKB);

private:
	void AIScenario1(int iKitID, KnowledgeBase* pKB, Move* pMove);
	void AIScenario2(int iKitID, KnowledgeBase* pKB, Move* pMove);

	DifferentialFilter* p_DiffFilter;

};

#endif // _AI_PLAYER_HEADER_