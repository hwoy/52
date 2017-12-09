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


struct Card
{
	
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
	unsigned int id;
	Rank rank;
	Suit suit;
};

template <typename C=Card,typename Gen=std::mt19937>
struct Deck: public std::list<C>
{
	Gen gen;
	
	Deck(std::time_t t=system_clock::to_time_t(system_clock::now())):
	gen(t){}
	
	Deck(const Deck<C,Gen> &deck,std::time_t t=system_clock::to_time_t(system_clock::now())):
	std::list<C>(deck),gen(t){}
	
	
	void shuffle(unsigned int loop=10240)
	{
		std::uniform_int_distribution<>dis(0,Deck<C,Gen>::size()?Deck<C,Gen>::size()-1:0);
		
		for(unsigned int i=0;i<loop;i++)
		{
			auto pos=Deck<C,Gen>::begin();
			std::advance(pos,dis(Deck<C,Gen>::gen));
			
			Deck<C,Gen>::push_back(*pos);
			Deck<C,Gen>::erase(pos);
			
			Deck<C,Gen>::cut(Deck<C,Gen>::size()>1);
		}
	}
	
	void cut(unsigned int pos)
	{
		for(unsigned int i=0;i<pos;i++)
		{
			auto pos=Deck<C,Gen>::begin();
			Deck<C,Gen>::push_back(*pos);
			Deck<C,Gen>::erase(pos);
			
		}
	}
	
	unsigned int giveall(Deck<C,Gen> &deck)
	{
		deck.splice(deck.end(),*this);
		return deck.size();
	}
	
	unsigned int give(Deck<C,Gen> &deck,unsigned int n)
	{
		auto end=Deck<C,Gen>::end();
		auto begin=end;
		
		std::advance(begin,static_cast<int>(-n));
		
		deck.splice(deck.end(),*this,begin,end);
		
		return deck.size();
	}
	
	unsigned int takeall(Deck<C,Gen> &deck)
	{
		return deck.giveall(*this);
	}
	
	unsigned int take(Deck<C,Gen> &deck,unsigned int n)
	{
		return deck.give(*this,n);
	}
};





#endif