### 52 is a kind of card game.
- C++11 implementation
- auto, rang-for, lambda, final, override, std::any_of, std::all_of
- Some ISO C++17 feature (std::string_view) [gcc v.7/clang v.4]
- Unlucky, considered low performance becuz virtual function in loop

### How to play

```cmd.exe
$ 52-r -b:20 -c:voy -c2:view
```

```exam


Main Deck: <46>
Money: <120>
BID: <20>
Base Index: <voy>
Current Index: <voy>

voy(5,27,80)[40] ===> Diamon[5] Spade[2] Clover[6]  <==== 3[13]

view(2,2,2)[40] ===> Clover[K] Heart[4] Heart[10]  <==== 3[24]

voy: y
voy ===> Spade[9] 

Main Deck: <45>
Money: <140>
BID: <20>
Base Index: <voy>
Current Index: <view>

voy(5,27,80)[20] ===> Diamon[5] Spade[2] Clover[6] Spade[9]  <==== 4[22]

view(2,2,2)[40] ===> Clover[K] Heart[4] Heart[10]  <==== 3[24]

view: y
view ===> Heart[7] 

Main Deck: <44>
Money: <160>
BID: <20>
Base Index: <voy>
Current Index: <voy>

voy(5,27,80)[20] ===> Diamon[5] Spade[2] Clover[6] Spade[9]  <==== 4[22]

view(2,2,2)[20] ===> Clover[K] Heart[4] Heart[10] Heart[7]  <==== 4[31]

voy: y
voy ===> Diamon[6] 

Main Deck: <43>
Money: <180>
BID: <20>
Base Index: <voy>
Current Index: <view>

voy(5,27,80)[0] ===> Diamon[5] Spade[2] Clover[6] Spade[9] Diamon[6]  <==== 5[28]*

view(2,2,2)[20] ===> Clover[K] Heart[4] Heart[10] Heart[7]  <==== 4[31]

view: n

Main Deck: <43>
Money: <180>
BID: <20>
Base Index: <voy>
Current Index: <view>

voy(5,27,80)[0] ===> Diamon[5] Spade[2] Clover[6] Spade[9] Diamon[6]  <==== 5[28]*

view(2,2,2)[20] ===> Clover[K] Heart[4] Heart[10] Heart[7]  <==== 4[31]*


========= Winner is view[+180] =========


$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Winner of the match is view[200] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


```

- Email: booskillerz@gmail.com
- FB: facebook.com/watt.duean
