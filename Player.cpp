#include "Player.hpp"

vector<string> Player::split(string str, string token){
    vector<string> result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

Player::Player(string input){
    //split the input string
    vector<string> vs = split(input,",");

    //unique player ID
    id = stoi(vs[0]);

    //player position (as string and enum)
    if(vs[1] == "GK") {posval=0;}
    else if(vs[1] == "DEF") {posval=1;}
    else if(vs[1] == "MID") {posval=2;}
    else if(vs[1] == "FW") {posval=3;}

    _pos = string_to_enum(vs[1]);

    //player Name
    name = vs[2];

    //player Club
    club = vs[3];

    //player points from last season
    points = stod(vs[4]);

    //player cost (in millions)
    cost = stod(vs[5]);

    //how the greedy algorithm values a player
    playerValue = points == 0 ? 0 : points*log(points)*log(points)/cost;
}

string Player::enum_to_string(Position position){
    switch(position){
        case GK:
            return "GK";
        case DEF:
            return "DEF";
        case MID:
            return "MID";
        case FW:
            return "FW";
        default:
            return "Error";
    }
}

Position Player::string_to_enum(string pos){
        if(pos == "GK") return Position::GK;
        if(pos == "DEF") return Position::DEF;
        if(pos == "MID") return Position::MID;
        if(pos == "FW") return Position::FW;
}

void Player::toString(){
    cout<<id<<" | "<<Player::enum_to_string(_pos)<<" | "<<name<<" | "<<club<<" | "<<points<<" | "<<cost<<"\n";
}

bool Player::operator<(Player& p){
    return this->playerValue > p.playerValue;
}

Position Player::getPosition(){return _pos;}
double Player::getCost(){return cost;}
double Player::getPoints(){return points;}
int Player::getPosval(){return posval;}
double Player::getPlayerValue(){return playerValue;}
int Player::getId(){return id;}
string Player::getClub(){return club;}
