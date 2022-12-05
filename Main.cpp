#include <bits/stdc++.h>
#include "Player.hpp"
#include <fstream>
#include <filesystem>
#include <deque>
#include<cstdlib>
using namespace std;

//#define iterator vector<int>::iterator
#define ll long long

deque<Player> players;

void load_players(string filename){
    filesystem::path cwd = filesystem::current_path() / filename;
    ifstream file(cwd.string());
    if(file){
        string input;
        while(file.good()){
            getline(file,input);
            Player p(input);
            players.push_back(p);
        }
    }
}

bool isFeasible(vector<Player>& model,vector<Player>& start, vector<Player>& bench){
    return model.size()==15 && start.size() == 11 && bench.size() == 4;
}

double evaluateModel(vector<Player>& model){
    double ret = 0;
    double cost = 0;
    for(auto p : model){
        cost += p.getCost();
        ret += p.getPoints();
    }
    if(cost > 100) return 0;
    return ret;
}

bool operator==(const Player& p,const Player& q){
    return q.id == p.id;
}

int main(int argc, char** argv){

    switch(stoi(argv[2])){
        case 1:
        {
            //REGULAR GREEDY
            load_players(argv[1]);
            sort(players.begin(),players.end());

            vector<Player> mySquad;
            vector<Player> startingPlayers;
            vector<Player> bench;
            map<string,int> clubMap;
            vector<bool> inSquad(players.size(),false);

            float budget = 100;

            //filling minimum standards for starting lineup (constraints are taken care of in if-clauses)
            int player_counter = 7;
            int keeper_counter = 2;
            int defender_counter = 5;
            int midfielder_counter = 5;
            int forward_counter = 3;


            for(auto it = players.cbegin(); it!=players.cend() && player_counter; ++it){
                Player p = *it;
                if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3) continue;
                    
                if(!(p.getPosition() == GK && keeper_counter >= 2
                    || p.getPosition() == DEF && defender_counter >= 3
                        || p.getPosition() == MID && midfielder_counter >= 4
                            || p.getPosition() == FW && forward_counter >= 3)){
                    continue;            
                }
                mySquad.push_back(p);
                clubMap[p.getClub()]++;
                startingPlayers.push_back(p);
                inSquad[it-players.cbegin()] = true;
                budget-=p.getCost();
                player_counter--;
                switch(p.getPosition()){
                    case GK:{keeper_counter--;break;}
                    case DEF:{defender_counter--;break;}
                    case MID:{ midfielder_counter--;break;}
                    case FW:{  forward_counter--;break;}
                    default: break;
                }
            }

            //filling rest of the starting lineup (determines the formation)
            player_counter = 4;
            for(auto it = players.cbegin(); it!=players.cend() && player_counter; ++it){
                Player p = *it;
                if(p.getCost() > budget || inSquad[it-players.begin()] || p.getPosition() == GK || clubMap[p.getClub()] >= 3) continue;
                    
                if(!(p.getPosition() == DEF && defender_counter
                        || p.getPosition() == MID && midfielder_counter
                            || p.getPosition() == FW && forward_counter)){
                    continue;            
                }
                mySquad.push_back(p);
                clubMap[p.getClub()]++;
                startingPlayers.push_back(p);
                inSquad[it-players.cbegin()] = true;
                budget-=p.getCost();
                player_counter--;
                switch(p.getPosition()){
                    case GK:{keeper_counter--;break;}
                    case DEF:{defender_counter--;break;}
                    case MID:{ midfielder_counter--;break;}
                    case FW:{  forward_counter--;break;}
                    default: break;
                }
            }

            //try to fill the bench
            player_counter = 4;
            for(auto it = players.cbegin(); it!=players.cend() && player_counter; ++it){
                Player p = *it;
                if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3) continue;
                    
                if(!(p.getPosition() == GK && keeper_counter
                    || p.getPosition() == DEF && defender_counter
                        || p.getPosition() == MID && midfielder_counter
                            || p.getPosition() == FW && forward_counter)){
                    continue;            
                }
                mySquad.push_back(p);
                clubMap[p.getClub()]++;
                bench.push_back(p);
                inSquad[it-players.cbegin()] = true;
                budget-=p.getCost();
                player_counter--;
                switch(p.getPosition()){
                    case GK:{keeper_counter--;break;}
                    case DEF:{defender_counter--;break;}
                    case MID:{ midfielder_counter--;break;}
                    case FW:{  forward_counter--;break;}
                    default: break;
                }
            }

            cout<<"Starting lineup : \n";
            for(auto player : startingPlayers) player.toString();
            cout<<"Bench payers : \n";
            for(auto player : bench) player.toString();
           
            cout<<"Team score : "<<evaluateModel(startingPlayers);
            break;
        }
        case 2:
        {
           //GRASP

           // CONSTRUCTION PHASE
            load_players(argv[1]);
            double alpha = stod(argv[3]);
            sort(players.begin(),players.end());

            vector<Player> mySquad;
            vector<Player> startingPlayers;
            vector<Player> bench;
            vector<Player> RCL;
            map<string,int> clubMap;
            vector<bool> inSquad(players.size(),false);

            float budget = 100;
            //filling minimum standards for starting lineup (constraints are taken care of in if-clauses)
            int player_counter = 7;
            int keeper_counter = 2;
            int defender_counter = 5;
            int midfielder_counter = 5;
            int forward_counter = 3;

            while(player_counter){
                //FIND HEAD ELEMENT (best available that can fit the squad)
                auto it = players.cbegin();
                while(it != players.cend()){
                    Player p = *it;

                    //IS player in the budget, not on the team yet and needed for mandatory 7?
                    if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3){ it++; continue;}
                    if(p.getPosition() == GK && keeper_counter >= 2
                        || p.getPosition() == DEF && defender_counter >= 3
                            || p.getPosition() == MID && midfielder_counter >= 4
                                || p.getPosition() == FW && forward_counter >= 3){
                                    break;
                    }
                    it++;
                }
                //CONSTRUCT RCL
                Player head = *it;
                double c = head.getPlayerValue();
                RCL.clear();
                RCL.push_back(head);
                it++;
                while(it != players.cend()){
                    Player p = *it;
                    if(p.getPlayerValue() < c*(1-alpha)){
                        break;
                    }
                    //IS player available and needed on the team?
                    if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3){ it++; continue;}
                    if(p.getPosition() == GK && keeper_counter >= 2
                        || p.getPosition() == DEF && defender_counter >= 3
                            || p.getPosition() == MID && midfielder_counter >= 4
                                || p.getPosition() == FW && forward_counter >= 3){
                                    RCL.push_back(p);
                    }
                    it++;
                }

                //PICK ONE ELEMENT FROM RCL AT RANDOM
                srand((unsigned) time(NULL));
                int random;
                if(RCL.size() == 1){
                    random = 0;
                }else {
                    random = rand() % RCL.size();       
                }
                Player p = RCL[random];
                mySquad.push_back(p);
                startingPlayers.push_back(p);
                int j = 0;
                for(auto o: players){
                    if(o == p) break;
                    j++;
                }
                inSquad[j] = true;
                clubMap[p.getClub()]++;
                budget-=p.getCost();
                player_counter--;

                switch(p.getPosition()){
                        case GK:{keeper_counter--; break;}
                        case DEF:{   defender_counter--;break;}
                        case MID:{   midfielder_counter--;break;}
                        case FW:{  forward_counter--;break;}
                        default: break;
                }
            }
            //fill the rest of the starting lineup
            player_counter = 4;
            while(player_counter){
                //FIND HEAD ELEMENT (best available that can fit the squad)
                auto it = players.cbegin();
                while(it != players.cend()){
                    Player p = *it;
                    //IS player in the budget, not on the team yet and needed for mandatory 7?
                    
                    if(p.getCost() > budget || inSquad[it-players.begin()] || p.getPosition() == GK || clubMap[p.getClub()]>=3){ it++;continue;}
                    if(p.getPosition() == DEF && defender_counter
                        || p.getPosition() == MID && midfielder_counter
                            || p.getPosition() == FW && forward_counter){
                                break;
                    }
                    it++;
                }
                //CONSTRUCT RCL
                Player head = *it;
                double c = head.getPlayerValue();
                RCL.clear();
                RCL.push_back(head);
                it++;
                while(it != players.cend()){
                    Player p = *it;
                    if(p.getPlayerValue() < c*(1-alpha)){
                        break;
                    }
                    //IS player available and needed on the team?
                    if(p.getCost() > budget || inSquad[it-players.begin()] || p.getPosition() == GK || clubMap[p.getClub()] >= 3){
                        it++;
                        continue;
                    }
                    if(p.getPosition() == DEF && defender_counter
                            || p.getPosition() == MID && midfielder_counter
                                || p.getPosition() == FW && forward_counter){
                                    RCL.push_back(p);
                    }
                    it++;
                }
                //PICK ONE ELEMENT FROM RCL AT RANDOM
                srand((unsigned) time(NULL));
                int random;
                if(RCL.size() == 1){
                    random = 0;
                }else {
                    random = rand() % RCL.size();       
                }
            
                Player p = RCL[random];
                mySquad.push_back(p);
                startingPlayers.push_back(p);
                int j = 0;
                for(auto o: players){
                    if(o == p) break;
                    j++;
                }
                inSquad[j] = true;
                clubMap[p.getClub()]++;
                budget-=p.getCost();
                player_counter--;

                switch(p.getPosition()){
                        case GK:{keeper_counter--; break;}
                        case DEF:{   defender_counter--;break;}
                        case MID:{   midfielder_counter--;break;}
                        case FW:{  forward_counter--;break;}
                        default: break;
                }
            }

            //try to fill the bench
            player_counter = 4;
            while(player_counter){
                //FIND HEAD ELEMENT (best available that can fit the squad)
                auto it = players.cbegin();
                while(it != players.cend()){
                    Player p = *it;

                    //IS player in the budget, not on the team yet and needed for mandatory 7?
                    if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3){
                        it++;
                        continue;
                    }
                    if(p.getPosition() == GK && keeper_counter
                        || p.getPosition() == DEF && defender_counter
                            || p.getPosition() == MID && midfielder_counter
                                || p.getPosition() == FW && forward_counter){
                                    break;
                    }
                    it++;
                }
                //CONSTRUCT RCL
                Player head = *it;
                double c = head.getPlayerValue();
                RCL.clear();
                RCL.push_back(head);
                it++;
                while(it != players.cend()){
                    Player p = *it;
                    if(p.getPlayerValue() < c*(1-alpha)){
                    //    cout<<"end of rcl\n";
                        break;
                    }
                    //IS player available and needed on the team?
                    if(p.getCost() > budget || inSquad[it-players.begin()] || clubMap[p.getClub()] >= 3){
                        it++;
                        continue;
                    }
                    if(p.getPosition() == GK && keeper_counter
                        || p.getPosition() == DEF && defender_counter
                            || p.getPosition() == MID && midfielder_counter
                                || p.getPosition() == FW && forward_counter){
                                    RCL.push_back(p);
                    }
                    it++;
                }

                //PICK ONE ELEMENT FROM RCL AT RANDOM
                srand((unsigned) time(NULL));
                int random;
                if(RCL.size() == 1){
                    random = 0;
                }else {
                    random = rand() % RCL.size();       
                }

                Player p = RCL[random];
                mySquad.push_back(p);
                bench.push_back(p);
                clubMap[p.getClub()]++;
                int j = 0;
                for(auto o: players){
                    if(o == p) break;
                    j++;
                }
                inSquad[j] = true;
                budget-=p.getCost();
                player_counter--;

                switch(p.getPosition()){
                        case GK:{keeper_counter--; break;}
                        case DEF:{   defender_counter--;break;}
                        case MID:{   midfielder_counter--;break;}
                        case FW:{  forward_counter--;break;}
                        default: break;
                }
            }

            //LOCAL SEARCH PHASE
            //for local search i will consider part of the vector inSquad up until the last one of the starting 11
            // because a better result is very unlikely to happen after the last element if the heuristic is to be trusted
            // neighborhood will be taking the player to the left instead(so flip the 0 left of the 1 for each 1)
            // if left of the 1 is already 1 i will skip that case
            //so in total the neighborhood will have at most 11 cases 
            //local search will consist of 10 iterations

            int counter = 0;
            int j = 0;
            for(int i = 0; i < inSquad.size(); i++){
                if(inSquad[i]) counter++;
                if(counter == 11){
                    j = i+1;
                    break;
                } 
            }
    
            inSquad.resize(j);
            int localSearchCounter = 0;
            int maxScore = evaluateModel(startingPlayers);
            while(localSearchCounter < 10){
                pair<int,int> switcheroo(0,0);
                int i = inSquad.size()-1;
                while(i){
                    int j = i-1;
                    while(inSquad[j] == 0 && j){
                        Player p = players[j];
                        if(players[i].getPosition() != players[j].getPosition() || clubMap[p.getClub()] >= 3
                            || (budget+players[i].getCost()-p.getCost()) < 0 || players[i].getPoints() > p.getPoints()){
                            j--;
                            continue;
                        }
                        replace(startingPlayers.begin(),startingPlayers.end(),players[i],players[j]);
                        double newModel = evaluateModel(startingPlayers);
                        if(newModel > maxScore){
                            switcheroo.first = i;
                            switcheroo.second = j;
                            maxScore = newModel;
                        }
                        replace(startingPlayers.begin(),startingPlayers.end(),players[j],players[i]);
                        j--;
                    }
                    i = j;
                }
                    
                
                if(switcheroo.first == 0) break;
                replace(startingPlayers.begin(),startingPlayers.end(),players[switcheroo.first],players[switcheroo.second]);
                inSquad[switcheroo.first] = 0;
                inSquad[switcheroo.second] = 1;
                clubMap[players[switcheroo.first].getClub()]--;
                clubMap[players[switcheroo.second].getClub()]++;
                budget+=players[switcheroo.first].getCost();
                budget-=players[switcheroo.second].getCost();
                localSearchCounter++;
            }
            
            cout<<"Starting lineup : \n";
            for(auto player : startingPlayers) player.toString();
            cout<<"Bench payers : \n";
            for(auto player : bench) player.toString();
            
            cout<<"Team score : "<<evaluateModel(startingPlayers);
            break;
        }
    }
    
    

    return 0;
}
