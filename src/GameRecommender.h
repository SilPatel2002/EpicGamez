#pragma once
#include <string>
#include <vector>
#include <unordered_map>


using namespace std;

struct Game{
    string name;
    vector<string> genres;   //horror, adventure, etc
    vector<string> features;  // singleplayer, mulitplayer, etc
    float rating;
    float price;

    Game();

    Game(string name, vector<string> genres, vector<string> features, float rating, float price);
};



class GameRecommender{


    private:

        unordered_map<string, Game> gameDatabase;

        unordered_map<string, vector<pair<string, float>>> adjList; // stores the game name mapped to (Game, similarity score)


        float calcGameSimilarity(const Game& game1, const Game& game2);

        //void addEdge(const Game& game1, const Game& game2, float &similarity);
        void addEdge(const Game& game1, const Game& game2, float similarity);




    public:

        void addGame(const string& name, vector<string>& genre, vector<string>& features, float rating, float price);

        void buildAdjList(); //building out data structure

        bool gameExists(const std::string& name) const; //simple bool

        vector<pair<string, float>> getSimilarGames(Game game, int count = 5);

        vector<string> depthFirstSearch(string src, string dest);

        vector<string> breadthFirstSearch(string src, string dest);


};
