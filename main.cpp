#include <iostream>
#include <string>
#include <iterator>


#include "card.hpp"
#include "player.hpp"
#include "52io.hpp"

typedef Card card_t;
typedef Deck<> deck_t;

typedef Player<> player_t;
typedef Group<> group_t;

struct computer final : public player_t
{
	computer(unsigned int id,const char *name):player_t(id,name){}
	virtual char bid(const group_t &deck) const
	{
		
		return 'y';
	}
};

struct human final : public player_t
{
	human(unsigned int id,const char *name):player_t(id,name){}
	virtual char bid(const group_t &deck) const
	{
		
		return 'y';
	}
};


static const Card::Rank rank[]={ {0,"A",1} , {1,"2",2} , {2,"3",3} ,\
					  {3,"4",4} , {4,"5",5} , {5,"6",6} ,\
					  {6,"7",7} , {7,"8",8} , {8,"9",9} ,\
					  {9,"10",10} , {10,"K",10} , {11,"Q",10} , {12,"K",10} };
					  
static const Card::Suit suit[]={{0,"Spade",0} , {1,"Heart",1} , {2,"Diamon",2}, {3,"Clover",3}};


int main()
{
	deck_t deck(rank,suit);
	
	group_t group={computer(0,"Hwoy"),computer(1,"View"),computer(3,"Kung")};
	
	group.take(deck,5);
	
	std::cout << group ;
	
	
	return 0;
}

