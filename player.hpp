#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <vector>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <memory>
#include "card.hpp"
#include "52config.hpp"

template <typename P>
struct Group;

template <typename D,typename Gen>
struct Player
{
	typedef D Deck_t;
	typedef Gen Gen_t;
	
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
	
	Player(unsigned int id,
	
#if __cplusplus > 201402L
	const std::string_view &name
#else
	const std::string &name
#endif
	
	
	,unsigned int money=MONEY):
	id(id),name(name),money(money),score(0),canbid(true),live(true),A(0),B(0),C(0){}
	
	virtual char bid(const Group<Player<D,Gen>> &group, const D &deck, Gen &gen) const {return '\0';}
};

template <typename P>
struct Group : public std::vector<std::unique_ptr<P>>
{
	typedef std::unique_ptr<P> Player_ptr;
	typedef P Player_t;

	
};

#endif