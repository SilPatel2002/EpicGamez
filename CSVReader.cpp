#include "GameRecommender.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma once 

using namespace std;

int readCSV(const string& filename){


    //===============================INITIALIZE VARIABLES===========================================
    ifstream iFile;
    string line = "";
    string name = "";
    string genre = "";
    string features = "";
    string strRating = "";
    string strPrice = "";
    float rating;
    float price;

    int location;

    GameRecommender gamer;


    //==========================Read CSV =========================================================


    iFile.open(filename);
    getline(iFile, line);
    


    for (int i = 0; i < 10; i++){

    
        getline(iFile, line);

        location = line.find('~');
        name = line.substr(0 , location);                           // will isolate the name
        line = line.substr(location + 1, line.length());


        if (line[0] == '"'){                                        // will isolate the genres

            line = line.substr(1, line.length());

            location = line.find('"');
            genre = line.substr(0, location);
            line = line.substr(location + 2, line.length());
        }

        else {
            location = line.find('~');
            genre = line.substr(0, location);
            line = line.substr(location + 1, line.length());
        }

        

        if (line[0] == '"'){                                       // will isolate the features

            line = line.substr(1, line.length());

            location = line.find('"');
            features = line.substr(0, location);
            line = line.substr(location + 2, line.length());
        }

        else {
            location = line.find('~');
            features = line.substr(0, location);
            line = line.substr(location + 1, line.length());
        }

        location = line.find('~');                              // will isolate the rating
        strRating = line.substr(0, location);
        strPrice = line.substr(location + 1, line.length());    // price is left over


        

        //=========================================FORMAT VALUES===============================

        vector<string> genreVector;


        while(genre != ""){

            if (genre.find(",") == string::npos){ //if genre does not have a comma in it
                if (features[0] == ' '){
                    genreVector.push_back(genre.substr(1, genre.length() - 1));
                }
                else{
                    genreVector.push_back(genre);
                }
                genre = "";
            }
        
            else{               // split each genre into its own string 
                location = genre.find(',');
                if (features[0] == ' '){
                    genreVector.push_back(genre.substr(1, location - 1));
                }
                else{
                    genreVector.push_back(genre.substr(0, location));
                }
                genre = genre.substr(location + 1, genre.length());
            }
            
        }


        vector<string> featureVector;

        while(features != ""){

            if (features.find(",") == string::npos){ //if genre does not have a comma in it
                if (features[0] == ' '){
                    featureVector.push_back(features.substr(1, features.length() - 1));
                }
                else{
                    featureVector.push_back(features);
                }
            
                features= "";
            }
        
            else{               // split each genre into its own string 
                location = features.find(',');
                if (features[0] == ' '){
                    featureVector.push_back(features.substr(1, location - 1));
                }
                else{
                    featureVector.push_back(features.substr(0, location));
                }
                
                features = features.substr(location + 1, features.length());
            }
            
        }


        try {
            rating = stof(strRating);
        }
        catch(const invalid_argument& e){
            rating = -1.0f;
        }
        
            // try to convert the string to a float. if it is "N/A" set it to -1.0

        try {
            price = stof(strPrice.substr(1, strPrice.length()));
        }
        catch (const invalid_argument& e){
            price = -1.0f;
        }



        //============================ add game ===============================



        gamer.addGame(name, genreVector, featureVector, rating, price);


         //===============================Print Testing ======================================


         /*
        cout << name << endl;
        
        for (auto g : genreVector){
            cout << g << "~";
        }
        
        cout << endl;

        for (auto f : featureVector){
            cout << f << "~";
        }
        
        cout << endl;

        cout << rating << endl;
        cout << price << endl;


    }

       
    */
        

   
    }
    iFile.close();

    return 0;

}




int main(){


    readCSV("games.csv");

    return 0;
}