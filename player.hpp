#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <vector>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <initializer_list>
#include "card.hpp"

template <typename P>
struct Group;

template <typename Deck=Deck<Card> >
struct Player
{
	unsigned int id;
	
#if __cplusplus > 201402L
	std::string_view name;
#else
	std::string name;
#endif
	
	Deck deck;
	
	Player(unsigned int id,const char *name):id(id),name(name){}
	
	virtual char bid(const Group<Player> &deck) const {return '\0';}
};

template <typename P=Player<Deck<Card> > >
struct Group : public std::vector<P>
{
	Group(){}
	
	Group(std::initializer_list<P> l):std::vector<P>(l){}
	
};

#endif