#ifndef _GLOBALS_HEADER_
#define _GLOBALS_HEADER_

#include <string>
#include <cmath>

using namespace std;

namespace KIT
{
	static const int KIT_MASK	= 128;		// 0000 0000 1000 0000 - Bit mask to filter KIT

	static const int MAJOR	= 128;	// 0000 0000 1000 0000
	static const int MINOR	= 0;		// 0000 0000 0000 0000
};

namespace SUIT
{
	static const int SUIT_MASK	= 48;	// 0000 0000 0011 0000 - Bit mask to filter SUIT

	static const int SPADES		= 0;	// 0000 0000 0000 0000
	static const int HEARTS		= 16;	// 0000 0000 0001 0000
	static const int CLUBS		= 32;	// 0000 0000 0010 0000
	static const int DIAMONDS	= 48;	// 0000 0000 0011 0000
};

namespace CARD
{
	static const int CARD_MASK	= 15;	// 0000 0000 0000 1111 - Bit mask to filter Card

	static const int ACE	= 12;	// 0000 0000 0000 1100
	static const int KING	= 11;	// 0000 0000 0000 1011
	static const int QUEEN	= 10;	// 0000 0000 0000 1010
	static const int JACK	= 9;	// 0000 0000 0000 1001
	static const int TEN	= 8;	// 0000 0000 0000 1000
	static const int NINE	= 7;	// 0000 0000 0000 0111
	static const int EIGHT	= 6;	// 0000 0000 0000 0110
	static const int SEVEN	= 5;	// 0000 0000 0000 0101
	static const int SIX	= 4;	// 0000 0000 0000 0100
	static const int FIVE	= 3;	// 0000 0000 0000 0011
	static const int FOUR	= 2;	// 0000 0000 0000 0010
	static const int THREE	= 1;	// 0000 0000 0000 0001
	static const int TWO	= 0;	// 0000 0000 0000 0000
};

namespace SYMBOL
{
	static const char SYM_SPADES	= 6; // ♠
	static const char SYM_HEARTS	= 3; // ♥
	static const char SYM_CLUBS		= 5; // ♣
	static const char SYM_DIAMONDS	= 4; // ♦

	static const char SYM_MAJOR		= 'M';
	static const char SYM_MINOR		= 'm';

	static const char SYM_ACE		= 'A';
	static const char SYM_KING		= 'K';
	static const char SYM_QUEEN		= 'Q';
	static const char SYM_JACK		= 'J';

	static char NumToASCII (int i) 
	{
		return '0' + i;
	}

	static string GetCardSymbol( int iCardID, int bShowKit = true )
	{
		int iCard = ( iCardID & CARD::CARD_MASK );
		int iSuit = ( iCardID & SUIT::SUIT_MASK );
		int iKit  = ( iCardID & KIT::KIT_MASK   );
		
		string sSymbol = "[";

		switch (iSuit)
		{
		case SUIT::SPADES:
			sSymbol += SYM_SPADES;
			break;

		case SUIT::HEARTS:
			sSymbol += SYM_HEARTS;
			break;

		case SUIT::CLUBS:
			sSymbol += SYM_CLUBS;
			break;

		case SUIT::DIAMONDS:
			sSymbol += SYM_DIAMONDS;
			break;
		}

		if ( bShowKit )
		{
			sSymbol += "-";

			switch(iKit)
			{
			case KIT::MAJOR:
				sSymbol += SYM_MAJOR;
				break;

			case KIT::MINOR:
				sSymbol += SYM_MINOR;
				break;
			}
		}

		sSymbol += "-";
		
		switch(iCard)
		{
		case CARD::ACE:
			sSymbol += SYM_ACE;
			break;

		case CARD::KING:
			sSymbol += SYM_KING;
			break;

		case CARD::QUEEN:
			sSymbol += SYM_QUEEN;
			break;

		case CARD::JACK:
			sSymbol += SYM_JACK;
			break;

		case CARD::TEN:
			sSymbol += NumToASCII( (iCard + 2)/10 );
			sSymbol += NumToASCII( (iCard + 2)%10 );
			break;

			default:
			sSymbol += NumToASCII(iCard + 2);
			break;
		}

		sSymbol += "]";

		return sSymbol;
	}

	static string GetKitSymbol( int iKitID )
	{
		int iSuit = ( iKitID & SUIT::SUIT_MASK );
		int iKit  = ( iKitID & KIT::KIT_MASK   );

		string sSymbol = "[";

		switch (iSuit)
		{
		case SUIT::SPADES:
			sSymbol += SYM_SPADES;
			break;

		case SUIT::HEARTS:
			sSymbol += SYM_HEARTS;
			break;

		case SUIT::CLUBS:
			sSymbol += SYM_CLUBS;
			break;

		case SUIT::DIAMONDS:
			sSymbol += SYM_DIAMONDS;
			break;
		}

		sSymbol += "-";

		switch(iKit)
		{
		case KIT::MAJOR:
			sSymbol += SYM_MAJOR;
			break;

		case KIT::MINOR:
			sSymbol += SYM_MINOR;
			break;
		}

		sSymbol += "]";

		return sSymbol;
	}
};

namespace STATE
{
	static const int NOT_CHECKED	= 0;
	static const int DONT_HAVE		= 1;
	static const int HAVE			= 2;
	static const int PROBABLY_HAVE	= 3;
	static const int USED_TO_HAVE	= 4;
};

namespace KIT_PARAM
{
	static const int MAJOR			= 0;
	static const int MINOR			= 1;
	static const int CARDS_KNOWN_1	= 2;
	static const int CARDS_KNOWN_2	= 3;
	static const int CARDS_KNOWN_3	= 4;
	static const int CARDS_KNOWN_4	= 5;
	static const int CARDS_KNOWN_5	= 6;
	static const int CARDS_KNOWN_6	= 7;

	static const int BEING_COLLECTED_BY_ME			= 8;
	static const int BEING_COLLECTED_BY_TEAMMATE	= 9;
};

#endif // _GLOBALS_HEADER_