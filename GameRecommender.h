#pragma once
#include <string>
#include <vector>


using namespace std;

struct Game{
    string name;
    vector<string> genres;   //horror, adventure, etc
    vector<string> features;  // singleplayer, mulitplayer, etc
    int rating;
    float price;

    Game(string name, vector<string> genres, vector<string> features, int rating, float price);
};
