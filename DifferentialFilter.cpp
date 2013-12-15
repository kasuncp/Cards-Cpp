#include "DifferentialFilter.h"
#include "Deck.h"


DifferentialFilter::DifferentialFilter()
{

}

DifferentialFilter::~DifferentialFilter()
{

}

// Assumes the input set only contains cards of a single kit
Set<Card>* DifferentialFilter::Enter( Set<Card>* pInput )
{
	pInput->Begin();
	if ( pInput->HasNext() )
	{
		Card* pCard = pInput->GetNext();
		int iSuit	= pCard->GetSuit();
		int iKit	= pCard->GetKit();

		Set<Card>* pCompleteKit = Deck::GetInstace()->GetKit( iSuit | iKit );
		Set<Card>* pResultSet = new Set<Card>();
		
		for (pCompleteKit->Begin(); pCompleteKit->HasNext();)
		{
			Card* pResultCard = pCompleteKit->GetNext();
			bool bRequired = true;

			for (pInput->Begin(); pInput->HasNext();)
			{
				Card* pInputCard = pInput->GetNext();
				if ( pResultCard->GetID() == pInputCard->GetID() )
				{
					bRequired = false;
					break;
				}
			}

			if (bRequired)
			{
				pResultSet->PushBack(pResultCard);
			}
		}

		return pResultSet;
	}

	return NULL;
}
