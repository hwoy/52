#ifndef __52_H__
#define __52_H__


#include <utility>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif

#include <algorithm>

#include "card.hpp"
#include "player.hpp"


template <typename G=Group<>,typename D=Deck<> >
struct Game52
{
	typedef typename G::Player_t Player_t;
	
	unsigned int money;
	
	Game52():money(0){}
	
	
	void shufflephase(D &deck,unsigned int loop=10240)
	{
		deck.shuffle(loop);
	}
	
	void drawphase(G &group,D &deck,unsigned int n=1)
	{
		Game52<G,D>::take(group,deck,n);
		Game52<G,D>::score(group);

	}
	
	void draw(G &group,D &deck,unsigned int index,unsigned int n=1)
	{
		Game52<G,D>::draw(group[index],deck,n);
	}
	
	template <typename P>
	void draw(P &player,D &deck,unsigned int n=1)
	{
		Game52<G,D>::takep(player,deck,n);
		Game52<G,D>::score(player);

	}
	
	static void score(G &group)
	{
		for(auto &i:group)
			Game52<G,D>::score(i);
	}
	
	static unsigned score(Player_t &player)
	{
		unsigned int sum=0;
		for(const auto &i:player.deck)
			sum+=i.rank.value;
		
		return player.score=sum;
	}
	
	void endphase(G &group,D &deck)
	{
		Game52<G,D>::giveall(group,deck);
		
		Game52<G,D>::money=0;
		
		for(auto &i:group)
		{
			i.score=0;
			i.canbid=true;
		}
		
	}

	static void giveall(G &group,D &deck)
	{
		for(auto &i:group)
		{
			for(auto &j:i.deck)
			{
				j.visible=false;
			}
			
			i.deck.giveall(deck);
		}
	}
	

	void take(G &group,D &deck,unsigned int n)
	{
		for(unsigned int i=0;i<n;i++)
		{
			for(auto &i:group)
			{
				takep(i,deck,1);
			}
		}
	}
	

	void takep(Player_t &player,D &deck,unsigned int n)
	{
			for(unsigned int i=0;i<n;i++)
		{
				if(player.canbid && player.live)
				{
					player.deck.take(deck,1);
					player.deck.back().visible=true;
					
					player.money-=BID;
					money+=BID;
				}
		}
	}
	
	static std::pair<bool,unsigned int> gameover(const G &group)
	{
		
		if(std::count_if(group.begin(),group.end(),[](const Player_t &player)->bool{return player.canbid;}))
			return std::make_pair(false,-1U);
		
		
		return std::make_pair(false,0);
	}
	
	
	static std::pair<bool,unsigned int> matchover(const G &group)
	{
		unsigned int j=0,id=0;
		
		for(const auto &i:group)
		{
			if(i.live)
			{
				j++;
				id=i.id;
			}
		}
		
		return std::make_pair(j==1,id);
	}
};

#endif