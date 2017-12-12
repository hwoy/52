#ifndef __HUMAN_H__
#define __HUMAN_H__


#include <iostream>
#include <string>


#include "card.hpp"
#include "player.hpp"
#include "52.hpp"

#include "52config.hpp"
#include "52type.hpp"

//========================= Human ========================= 

struct human final : public player_t
{
	human(unsigned int id,const std::string &name,unsigned int money):player_t(id,name,money){}
	
	char bid(const group_t &group,const deck_t &deck,gen_t &) const override
	{
		
		std::string str;
		
		do{
			std::cout << name << "<"  <<'[' << IDYES << ']' << "Yes," << '[' << IDNO << ']' << "No," << '[' << IDQUIT << ']' << "Quit"<< ">:";
			std::getline(std::cin,str);
			
			if(!str.size()) str=IDDEFAULT;
			
		}while((str[0]!=IDYES && str[0]!=IDNO && str[0]!=IDQUIT) || str.size()>1);
		
		return str[0];
	}
};
#endif

