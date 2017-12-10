#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <vector>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif
#include <memory>
#include <initializer_list>
#include "card.hpp"
#include "52config.hpp"

template <typename P>
struct Group;

template <typename D=Deck<Card> >
struct Player
{
	typedef D Deck_t;
	unsigned int id;
	
#if __cplusplus > 201402L
	std::string_view name;
#else
	std::string name;
#endif

	unsigned int money;
	unsigned int score;
	
	bool canbid;
	bool live;
	
	unsigned int A;
	unsigned int B;
	unsigned int C;
	
	D deck;
	
	Player(unsigned int id,const char *name,unsigned int money=MONEY):
	id(id),name(name),money(money),score(0),canbid(true),live(true),A(0),B(0),C(0){}
	
	virtual char bid(const Group<std::shared_ptr<Player<Deck<Card> > >> &deck) const {return '\0';}
};

template <typename P=std::shared_ptr<Player<Deck<Card> > > >
struct Group : public std::vector<P>
{
	typedef P Player_ptr;
	typedef typename P::element_type Player_t;
	
};

#endif