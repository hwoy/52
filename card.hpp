#ifndef __CARD_H__
#define __CARD_H__

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <list>
#include <random>
#include <chrono>

using namespace std::chrono;


struct Rank
{
	unsigned int id;
#if __cplusplus > 201402L
	std::string_view name;
#else
	std::string name;
#endif
	unsigned int value;
};


struct Suit
{
	unsigned int id;
	
#if __cplusplus > 201402L
	std::string_view name;
#else
	std::string name;
#endif

	unsigned int priority;
};

template <typename R,typename S>
struct Card
{
	typedef R Rank_t;
	typedef S Suit_t;
	
	unsigned int id;
	
	R rank;
	S suit;
	
	bool visible;
	
	Card(unsigned int id,const R &rank,const S &suit,bool visible=false):
	id(id),rank(rank),suit(suit),visible(visible){}
};

template <typename C>
struct Deck: public std::list<C>
{
	
	typedef C Card_t;
	
	Deck()=default;
	
	Deck(const Deck<C> &deck):
	std::list<C>(deck){}
	
	template <typename Gen>
	void shuffle(unsigned int loop,Gen &gen)
	{
		std::uniform_int_distribution<>dis(0,Deck<C>::size()?Deck<C>::size()-1:0);
		
		for(unsigned int i=0;i<loop;i++)
		{
			auto pos=Deck<C>::begin();
			std::advance(pos,dis(gen));
			
			Deck<C>::push_back(*pos);
			Deck<C>::erase(pos);
			
			Deck<C>::cut(Deck<C>::size()>1);
		}
	}
	
	void cut(unsigned int pos)
	{
		for(unsigned int i=0;i<pos;i++)
		{
			auto pos=Deck<C>::begin();
			Deck<C>::push_back(*pos);
			Deck<C>::erase(pos);
			
		}
	}
	
	unsigned int giveall(Deck<C> &deck)
	{
		deck.splice(deck.end(),*this);
		return deck.size();
	}
	
	unsigned int give(Deck<C> &deck,unsigned int n)
	{
		auto end=Deck<C>::end();
		auto begin=end;
		
		std::advance(begin,-reinterpret_cast<int&>(n));
		
		deck.splice(deck.end(),*this,begin,end);
		
		return deck.size();
	}
	
	unsigned int takeall(Deck<C> &deck)
	{
		return deck.giveall(*this);
	}
	
	unsigned int take(Deck<C> &deck,unsigned int n)
	{
		return deck.give(*this,n);
	}
};





#endif