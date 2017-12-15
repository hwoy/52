#ifndef __COMPUTER_H__
#define __COMPUTER_H__


#include <random>
#include <algorithm>

#if __cplusplus > 201402L
	#include <string_view>
#else
	#include <string>
#endif



#include "card.hpp"
#include "player.hpp"
#include "52.hpp"

#include "52config.hpp"
#include "52type.hpp"



//========================= computer ========================= 
struct computer final : public player_t
{

		template <typename T>
		static unsigned int find (const group_t &group, const T &t)
		{
			unsigned int index;
			for(index=0;index<group.size();index++)
				if(group[index]->live && group[index]->score<=SCORE) break;
			
			for(unsigned int i=index+1;i<group.size();i++)
				if( group[i]->live && group[i]->score<=SCORE && t(group[i]->score , group[index]->score)) index=i;
			
			return index;
		}


	computer(unsigned int id,\
	
#if __cplusplus > 201402L
	const std::string_view &name
#else
	const std::string &name
#endif
	
	,unsigned int money,gen_t &gen)
	:player_t(id,name,money)
	{
		A = random(_A-2,_A+2,gen);
		B = random(_B-5,_B+5,gen);
		C = random(_C-5,_C+20,gen);
	}
	

	char bid(const group_t &group,const deck_t &deck,gen_t &gen) const override
	{
		char ch;
		

		if( group[find(group,[](unsigned int a,unsigned int b)->bool{return a>b;} )]->id==id)
			{
				if(std::count_if(group.begin(),group.end(),[](const player_ptr &player)->bool { return player->live && player->canbid;  } )==1 )
					return IDNO;
				
				else{
					std::uniform_int_distribution<unsigned int> dis(1,100);
		
		
					if((SCORE-score)<=2) ch=(dis(gen)<=A?IDYES:IDNO);
					else if((SCORE-score)<=5) ch=(dis(gen)<=B?IDYES:IDNO);
					else if((SCORE-score)<=9) ch=(dis(gen)<=C?IDYES:IDNO);
					else ch=IDYES;					
				}
			}
			
		else ch=IDYES;
	
		return ch;
	}
	
	private:
	template <typename Gen>
	static unsigned int random(unsigned int a,unsigned int b,Gen &gen)
	{
		std::uniform_int_distribution<> dis;
		
		return a+(dis(gen)%(b-a+1));
	}

};


//========================= computer 2 ========================= 

struct computer2 final : public player_t
{

		template <typename T>
		static unsigned int find (const group_t &group, const T &t)
		{
			unsigned int index;
			for(index=0;index<group.size();index++)
				if(group[index]->live && group[index]->score<=SCORE) break;
			
			for(unsigned int i=index+1;i<group.size();i++)
				if( group[i]->live && group[i]->score<=SCORE && t(group[i]->score , group[index]->score)) index=i;
			
			return index;
		}

	
	
	computer2(unsigned int id,
	
#if __cplusplus > 201402L
	const std::string_view &name
#else
	const std::string &name
#endif

	,unsigned int money)
	:player_t(id,name,money)
	{
		A=2;
		B=2;
		C=2;
	}
	
	char bid(const group_t &group,const deck_t &deck,gen_t &gen) const 
	{
		char ch;
		

		if( group[find(group,[](unsigned int a,unsigned int b)->bool{return a>b;} )]->id==id)
			{
				if(std::count_if(group.begin(),group.end(),[](const player_ptr &player)->bool { return player->live && player->canbid;  } )==1 )
					return IDNO;
				
				else{
					std::uniform_int_distribution<unsigned int> dis(1,100);
		
					const unsigned int diff=(SCORE-score);

					if(diff<10)
					{
						auto count=std::count_if(deck.begin(),deck.end(),[diff](const card_t &card)->bool
						{
							return card.rank.value<diff;
						});
						
						ch=(dis(gen)<=((count*100)/deck.size())?IDYES:IDNO);
	
					}
					
					else{
						ch=IDYES;
					}
				}
			}
			
		else ch=IDYES;
	
		return ch;
	}
	

};

#endif