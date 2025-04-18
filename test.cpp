#include "GameRecommender.h"



int main(){

    GameRecommender gamer;

    Game game1("Portal",
        {"Puzzle", "First-Person", "Sci-fi"},
        {"Single-player", "Valve"},
        4.5f,  // Now using float
        9.99f
    );
   
    Game game2("Portal 2",
        {"Puzzle", "First-Person", "Sci-fi", "Co-op"},
        {"Single-player", "Online Co-op", "Valve"},
        4.8f,  // Now using float
        9.99f
    );


    gamer.addGame(game1);
    gamer.addGame(game2);

    gamer.buildAdjList();





    return 0;
}