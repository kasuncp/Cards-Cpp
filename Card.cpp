#include "Card.h"
#include "Globals.h"
#include <iostream>

using namespace std;

Card::Card( int iID ) : i_ID(iID)
{
	i_Card = ( i_ID & CARD::CARD_MASK );
	i_Suit = ( i_ID & SUIT::SUIT_MASK );
	i_Kit  = ( i_ID & KIT::KIT_MASK   );
}

Card::~Card()
{

}

void Card::DebugPrint()
{
	cout << SYMBOL::GetCardSymbol(i_ID, false);
}
