#ifndef _CARD_HEADER_
#define _CARD_HEADER_

class Card
{
public:
	Card(){};
	Card( int iID );
	virtual ~Card();

	int GetID()		{ return i_ID;	 }
	int GetSuit()	{ return i_Suit; }
	int GetKit()	{ return i_Kit;  }
	int GetCardVal(){ return i_Card; }

	void DebugPrint();

protected:
private:
	int i_ID;
	int i_Card;
	int i_Kit;
	int i_Suit;
};

#endif // _CARD_HEADER_