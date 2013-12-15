#ifndef _KNOWLEDGE_BASE_HEADER_
#define _KNOWLEDGE_BASE_HEADER_

#include "Globals.h"
#include "Set.h"
#include "Card.h"
#include <map>

using std::map;

#define NUM_CARDS			6
#define NUM_PLAYERS			6

typedef int KitStrengthParam;

class KnowledgeBase
{
public:
	KnowledgeBase(int iKitID, int iPlayerID);
	virtual ~KnowledgeBase();

	void InitKB(Set<Card> oSet);

	void	SetState(int iPlayerID, int iCardID, int iState);
	int		GetState(int iPlayerID, int iCardID);

	int		GetKitID()		{ return i_KitID; }
	int		GetPlayerID()	{ return i_ThisPlayerID; }

	bool	HaveCardsLeft			( int iPlayerID )	{  return (ai_HaveCardCount[iPlayerID] > 0);		}
	bool	ProbablyHaveCardsLeft	( int iPlayerID )	{  return (ai_ProbablyCardCount[iPlayerID] > 0);	}
	bool	UsedToHaveCardsLeft		( int iPlayerID )	{  return (ai_UsedToCardCount[iPlayerID] > 0);		}
	bool	NotCheckedCardsLeft		( int iPlayerID )	{  return (ai_NotCheckedCardCount[iPlayerID] > 0);		}

	bool	CanDropKit();

	int		GetKitStrength();

	void DebugPrint();

protected:
	int GetCardIndex		(int iCardID);
	int GetCardIDFromIndex	(int iIndex	);

	int i_KitID;
	int i_ThisPlayerID;

	int ai_StateMatrix[NUM_CARDS][NUM_PLAYERS];

	int ai_HaveCardCount[NUM_PLAYERS];
	int ai_ProbablyCardCount[NUM_PLAYERS];
	int ai_UsedToCardCount[NUM_PLAYERS];
	int ai_NotCheckedCardCount[NUM_PLAYERS];
	bool ab_HasCardsLeft[NUM_PLAYERS];

	map<KitStrengthParam, int> map_KitStrengthParams;

private:
};

#endif // _KNOWLEDGE_BASE_HEADER_