#include "CardFactory.h"

CardFactory::CardFactory()
{

}

CardFactory::~CardFactory()
{

}

CardFactory* CardFactory::Instance()
{
	if( !p_Instance )
		p_Instance = new CardFactory();

	return p_Instance;
}

Set<Card>* CardFactory::GetCardsBySuit(int iSuit)
{

}