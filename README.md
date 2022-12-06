# FantasyDraft
Fantasy football is a game where participants serve as managers of virtual football teams. In the Fantasy Premier League (https://fantasy.premierleague.com/) (FPL), you are given a task to pick a squad of real- life football players playing in the English Premier League, who score points for your team based on their performances in their own real matches. As a manager, you are given a budget of 100 million in virtual currency, which can be spent on selecting a squad.

A squad has to have 15 players across four different positions: goalkeepers (GK), defenders (DEF),midfielders (MID), and forwards (FW). However, only 11 players will make it to the first team lineup, and 4 players have to serve as substitutes. Points can only be scored by players selected in the first team lineup. Given the list of players with their respective value (price), club, and points scored, your task is to draft a squad of 15 players, and pick the first team lineup, so as to maximize the number of points scored by the selected eleven players on the pitch. The following text describes the format of problem instances (input files), and formulates the objectives and constraints.

Problem instances define the list of players which are available for the draft. These instances were collected from various gameweeks during the course of previous and current football seasons. The data provided in each instance file is defined as follows:
    • ID – unique integer representing each player
    • Position – player's position on the pitch (e.g., GK, DEF, MID, or FW)
    • Name – player's surname
    • Club – player's club
    • Points – points scored during that season (until the point of creating the instance)
    • Price – player's cost in virtual currency (in millions)

The problem defines the following constraints:
1. Budget
    • A budget of 100 million virtual currency is available for the draft.
    • Total cost of the drafted 15 players must not exceed the budget, but can be less than
        the budget.
2. Squad draft
    • A 15-player squad must be drafted, consisting of:
        o 2 goalkeepers
        o 5 defenders
        o 5 midfielders
        o 3 forwards
    • A maximum of 3 players can be selected from a single club
3. First team lineup
    • From the 15-player squad, 11 players must be picked for the first team.
    • The players can play in any formation, providing:
        o 1 goalkeeper
        o At least 3 defenders
        o At least 1 forward
    • 4 players must be selected as substitutes
        o 1 goalkeeper must be a substitute
    • Players can only play in positions defined by the instance (i.e., goalkeepers can only
        play in a goalkeeper position, or sit on the bench)

The main objective is to maximize the sum of points from the players selected for the first team, and find the ''best eleven'' players in each instance. Points from players placed on the bench are not calculated in the overall score, however, their cost is deducted from the available budget. The solution consists of a list of (15) players drafted, a list of (11) players picked for the first team, the selected formation, and the overall score of the first team squad.

# Running the program
Datasets are stored in .txt files (could convert to .csv).
3 possible sets - players1.txt, players2.txt and players3.txt

Everything is already compiled in main.exe - to run type the command in terminal in the following format :
    if using "greedy" : ./main players[i].txt 1
    if using "grasp" : ./main players[i].txt 2 [alpha]

alpha is a double value between 0 and 1.
When alpha is 0 the search is equal to greedy.
When alpha is 1 the search is completely randomised.

Solution printout example : 
    Starting lineup :   

    545 | FW | Vardy | Leicester | 156 | 9.7    

    10 | GK | Patr∩┐╜cio | Wolves | 110 | 5.1   

    142 | DEF | Dunk | Brighton | 85 | 4.8  

    384 | MID | Snodgrass | West Ham | 86 | 5.3 

    285 | MID | De Bruyne | Man City | 178 | 10.7   

    118 | DEF | Stevens | Sheffield Utd | 104 | 5.1 

    119 | DEF | Baldock | Sheffield Utd | 109 | 5.1 

    282 | MID | Salah | Liverpool | 177 | 12.8  

    356 | MID | Doucour∩┐╜ | Watford | 92 | 5.6 

    293 | MID | Alli | Spurs | 107 | 8.3    

    166 | DEF | Fern∩┐╜ndez | Newcastle | 78 | 4.5  

    Bench payers :  

    572 | FW | Maupay | Brighton | 99 | 5.8 

    561 | FW | Calvert-Lewin | Everton | 109 | 6.4  

    124 | DEF | Mee | Burnley | 85 | 5  

    12 | GK | Dubravka | Newcastle | 110 | 5    

    Team score : 1282   
    