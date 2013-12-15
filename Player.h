#ifndef _PLAYER_HEADER_
#define _PLAYER_HEADER_

#include <map>

#include "Hand.h"
#include "KnowledgeBase.h"

using std::map;

class Move
{
public:
	enum MoveType 
	{
		UNDEFINED,
		ASK_A_CARD,
		DROP_A_KIT
	};

	Move()
	{
		i_InitiatorID	= -1;
		i_AffecteeID	= -1;
		i_CardID		= -1;
		i_KitType		= -1;

		e_Type			= UNDEFINED;
		b_Processed		= false;
		b_Successful	= false;
	}

	//ASK_A_CARD
	int i_InitiatorID;
	int i_AffecteeID;
	int i_CardID;

	//DROP_A_KIT
	int i_KitType;

	//General
	MoveType e_Type;
	bool b_Processed;
	bool b_Successful;
};

class Player
{
public:
	Player(int iID);
	virtual ~Player();

	int		GetID();

	bool	HasCard			( int iCardID );
	Card*	RemoveCard		( int iCardID );
	void	RemoveCardsOfType( int iKitID  );
	void	AddCard			( Card* pCard );
	void	ReceiveHand		( Set<Card>* pSet );
	//Must be called after ReceiveHand() is called
	void	ComputeInitialKB();

	virtual void	UpdateKB(Move* pMove)	= 0;
	virtual Move*	NextMove()				= 0;

	void	ShowHand();

	void	DebugShowKB(int iKitID);

protected:
	virtual Move*	DropKit( int iKitID )	= 0;
	virtual Move*	AskForCard()			= 0;

	Set<Card>* GetCardsOfType(int iKitID);

	int		i_PlayerID;
	Hand*	p_Hand;

	map<int, KnowledgeBase*> map_KnowledgeBases;
	bool b_KBInit;
};

#endif // _PLAYER_HEADER_