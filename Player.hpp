#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;



enum Position {GK, DEF, MID, FW};

class Player {
    private:
        
        Position _pos;
        string name;
        string club;
        double points;
        double cost;
        double playerValue;
        int posval;
        double epsilon = 1e-6;
    public:
        int id;
        Player(string);
        
        static string enum_to_string(Position);
        static Position string_to_enum(string);
        static vector<string> split(string, string);
        void toString();
        bool operator<(Player&);
        Position getPosition();  
        double getCost();  
        int getPosval();
        double getPoints();
        double getPlayerValue();
        int getId();
        string getClub();
};

#endif