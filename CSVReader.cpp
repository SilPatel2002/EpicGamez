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
    string appid = "";
    string name = "";
    string genre = "";
    string features = "";
    string strPosRating = "";
    string strNegRating ="";
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

        location = line.find(',');
        appid = line.substr(0 , location);                           // will isolate the appid
        line = line.substr(location + 1, line.length());

        location = line.find(',');
        name = line.substr(0 , location);                           // will isolate the name
        line = line.substr(location + 1, line.length());


        location = line.find(',');                        // skip over release date
        line = line.substr(location + 1, line.length());



        location = line.find(',');                        // skip over english
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over developer
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over publisher
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over platform
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over required age
        line = line.substr(location + 1, line.length());


                                               
        location = line.find(',');                       // will isolate the list of features
        features = line.substr(0, location);
        line = line.substr(location + 1, line.length());
      
        
        location = line.find(',');                        // skip over reduced genres list
        line = line.substr(location + 1, line.length());



        location = line.find(',');                      // will isolate the genres
        genre = line.substr(0, location);
        line = line.substr(location + 1, line.length());



        location = line.find(',');                        // skip over achievements
        line = line.substr(location + 1, line.length());


        location = line.find(','); 
        strPosRating = line.substr(0, location);          // will isolate the number of positive ratings
        line = line.substr(location + 1, line.length());

        location = line.find(','); 
        strNegRating = line.substr(0, location);          // will isolate the number of negative ratings
        line = line.substr(location + 1, line.length());



        location = line.find(',');                        // skip over avg playtime
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over median playtime
        line = line.substr(location + 1, line.length());

        location = line.find(',');                        // skip over number of owners




        strPrice = line.substr(location + 1, line.length()); // is the price


       


     


        

        //=========================================FORMAT VALUES===============================

        vector<string> genreVector;


        while(genre != ""){

            if (genre.find(";") == string::npos){ //if genre does not have a semi colon in it
                
                genreVector.push_back(genre);
                
                genre = "";
            }
        
            else{               // split each genre into its own string 
                location = genre.find(';');
            
                genreVector.push_back(genre.substr(0, location));
            
                genre = genre.substr(location + 1, genre.length());
            }
            
        }


        vector<string> featureVector;

        while(features != ""){

            if (features.find(";") == string::npos){ //if genre does not have a semi colon in it
             
                featureVector.push_back(features);
                
            
                features= "";
            }
        
            else{               // split each genre into its own string 
                location = features.find(';');
               
                featureVector.push_back(features.substr(0, location));
                
                
                features = features.substr(location + 1, features.length());
            }
            
        }


        try {
            rating = stof(strPosRating) - stof(strNegRating);
        }
        catch(const invalid_argument& e){
            rating = -1.0f;
        }
        
            // try to convert the string to a float. if it is "N/A" set it to -1.0

        try {
            price = stof(strPrice);
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


    
        */

       
    
    iFile.close();

    return 0;

   
    }
    

}




int main(){


    readCSV("steam_games.csv");

    return 0;
}