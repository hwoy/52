#include <iostream>
#include <string>
#include <iterator>


#include "card.hpp"
#include "player.hpp"
#include "52io.hpp"
#include "52.hpp"

typedef Card card_t;
typedef Deck<> deck_t;

typedef Player<> player_t;
typedef Group<> group_t;

typedef Game52<> game52_t;

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


static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck);

template <std::size_t M,std::size_t N>
static deck_t constructdeck(const Card::Rank (&rank)[M],const Card::Suit (&suit)[N]);


static const Card::Rank rank[]={ {0,"A",1} , {1,"2",2} , {2,"3",3} ,\
					  {3,"4",4} , {4,"5",5} , {5,"6",6} ,\
					  {6,"7",7} , {7,"8",8} , {8,"9",9} ,\
					  {9,"10",10} , {10,"K",10} , {11,"Q",10} , {12,"K",10} };
					  
static const Card::Suit suit[]={{0,"Spade",0} , {1,"Heart",1} , {2,"Diamon",2}, {3,"Clover",3}};


int main()
{
	deck_t deck=constructdeck(rank,suit);
	
	group_t group={computer(0,"Hwoy"),computer(1,"View"),computer(3,"Kung")};
	
	game52_t game52;
	

	game52.shufflephase(deck);
	
	game52.drawphase(group,deck,2);
	
	game52.draw(group,deck,0);
	
	group[2].live=false;group[1].canbid=false;
	
	showinfo(game52,group,deck);
	
	game52.endphase(group,deck);
	
	
	
	return 0;
}

static void showinfo(const game52_t &game52,const group_t &group,const deck_t &deck)
{
	std::cout << "\nMain Deck: <" << deck.size() << ">\n";
	
	std::cout << "Money: <" << game52.money << ">\n";
	
	std::cout << "BID: <" << BID << ">\n\n";
	
	std::cout << group
	
			  << "============================================================================\n";
	
}


template <std::size_t M,std::size_t N>
static deck_t constructdeck(const Card::Rank (&rank)[M],const Card::Suit (&suit)[N])
{
	deck_t deck;
	
	for(unsigned int i=0,c=0;i<N;i++)
	{
		for(unsigned int j=0;j<M;j++)
		{
			deck.push_back({c++,rank[j],suit[i]});
		}
	}
		
		return deck;
}



