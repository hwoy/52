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
	
	void draw(Player_t &player,D &deck,unsigned int n=1)
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
					unsigned int bid;
					
					player.deck.take(deck,1);
					player.deck.back().visible=true;
					
					bid=player.money>=BID?BID:player.money;
					
					player.money-=bid;
					money+=bid;
					

					if(!player.money) 
					{
						player.canbid=false;break;
					}

				}
		}
	}
	
		
	static std::vector<unsigned int> gameover(const G &group)
	{
		std::vector<unsigned int> vec;
		
		if(std::any_of(group.begin(),group.end(),[](const Player_t &player)->bool{return player.live && player.canbid;}))
			return vec;
		
		for(unsigned int i=0;i<group.size();i++)
			if(group[i].live) vec.push_back(i);
		
		
		std::sort(vec.begin(),vec.end(),[&group](unsigned int a,unsigned int b)->bool{
			return group[a].score > group[b].score;
		});
		
		while(group[vec.front()].score>group[vec.back()].score) vec.pop_back();
		
		return vec;
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