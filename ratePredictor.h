//
// Created by Polat Öztürk on 2.01.2023.
//

#ifndef CMP2003PROJECT_FINAL_RATEPREDICTOR_H
#define CMP2003PROJECT_FINAL_RATEPREDICTOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <stdexcept>
#include <map>
#include <cmath>
#include "createAllList.h"

using namespace std;

//This function calculate our equation which including cosine similarity.
float cosineSimilarity(vector<vector<pair<float, float> > > data,float alice){

    float numer = 0;
    float denom = 0;
    float final = 0;
    for (int t = 0; t < data.size(); ++t) {
        float sum = 0;
        float mul1 = 0;
        float mul2 = 0;
        float result=0;
        float sqr1 = 0;
        float sqr2 = 0;
        for (int p = 1; p < data[t].size(); ++p) {
            //sum variable store the sum of product of normalized ratings of same movie
            sum = sum+(data[t][p].first*data[t][p].second);
            //mul1 and mul2 variables separately store the sum of square of normalized ratings
            mul1 = mul1+(data[t][p].first*data[t][p].first);
            mul2 = mul2+(data[t][p].second*data[t][p].second);
        }
        //For avoiding to denominator become 0
        if(mul1!=0&&mul2!=0) {
            //Calculate the square root of mul1 and mul2
            sqr1 = sqrt(mul1);
            sqr2 = sqrt(mul2);
            //And result of the normalized cosine similarity result variable
            result = sum / (sqr1 * sqr2);
            //After these operations we sum all normalized cosine similarities times normalized
            //rating of that movie times number of common movies of these users.
            //And we sum absolute value of normalized cosine similarity times number of common movies of these users.
            numer = numer+(result*data[t][0].second*data[t].size());
            denom = denom+ (abs(result)*data[t].size());

        }

    }
    //Then we predict the rating by adding these values ratio to the average rate value of that user.
    final = alice + (numer/denom);
    return final;

}
//This function find the users which have common movies more than we want
vector<vector<pair<float, float> > > qualityVector(vector<vector<pair<float, float> > > rating_pair_vector, float size, float requiredQuality,int min_user_size){
    vector<vector<pair<float, float> > > data;

    for (int p = 0; p < size; ++p) {
        if(rating_pair_vector[p].size() > requiredQuality){
            data.push_back(rating_pair_vector[p]);
            continue;
        }
    }
    //If there is too less user with this much of common movies then function call itself recursively
    //but common movie number we want decrease by 10
    if (data.size()<min_user_size&&requiredQuality>10){
        data = qualityVector(rating_pair_vector, size,requiredQuality-10,min_user_size);
    }

    return data;

}

//With this function we take our user we look for,user pair list for all users.
//Then we take all of them average separately and subtract from themselves
//Thus we get our normalized rating data
float fundamentalFunction(vector<vector<pair<float, float> > > data){

    float special_average = 0;
    for (int j = 0; j < data.size(); ++j) {
        float sum1 = 0;
        float sum2 = 0;
        float average1 = 0;
        float average2 = 0;

        for (int i = 1; i < data[j].size(); ++i) {

            sum1 = sum1 + data[j][i].first;
            sum2 = sum2 + data[j][i].second;
        }
        //Special average is average of user we look for
        if(j==1){
            special_average = sum1/(data[j].size()-1);

        }
        average1 = sum1/(data[j].size()-1);
        average2 = sum2/(data[j].size()-1);


        for (int k = 0; k < data[j].size(); ++k) {
            //Index one of our data is for rate of movie we are looking for.
            //So we don't have any rate on the first side of the pair in index 0
            if(k==0){
                data[j][k].second = data[j][k].second -average2;
                continue;
            }
            //We simply subtract average of user from rate of that movie we are look for
            data[j][k].first = data[j][k].first - average1;
            data[j][k].second = data[j][k].second - average2;

        }
    }

    //After all of that we call our cosine similarity function
    float result = cosineSimilarity(data,special_average);

    return result;

}

//This function is for take raw data and make it quality data by using qualityVector function
//after that find the normalized version of it and calculate the rating prediction by using fundamentalFunction function.
float predictor(vector<vector<pair<float, float> > > rating_pair_vector, float size, int requiredSize, int min_user_size){

    vector<vector<pair<float, float> > > qualityData = qualityVector(rating_pair_vector,size,requiredSize, min_user_size);
    float finalresult = fundamentalFunction(qualityData);

    return finalresult;

}

//This function is for to avoid from nan of negative values and at the same time get a accurate rates.
//To do this function start with higher minimum common movie size and decrease it until our final rates does not contain
//nan or negative values. It has three modes these are different combination of starting point of minimum common movie sizes
//and required minimum user size in this data
float finalFunction(vector<vector<pair<float, float> > > rating_pair_vector, float size, int mode){
    float finalresult = 0;
    float finalresult_one = 0;
    float finalresult_two = 0;

    switch (mode) {
        case 0: {


            float final_result50 = predictor(rating_pair_vector, size, 50, 50);


            if(!isnan(final_result50)&&final_result50>0){
                finalresult = final_result50;
                break;
            }
            float final_result40 = predictor(rating_pair_vector, size, 40, 50);
            if(isnan(final_result50)&&!isnan(final_result40)&&final_result40>0){
                finalresult = final_result40;
                break;
            }
            float final_result30 = predictor(rating_pair_vector, size, 30 , 50);
            if(isnan(final_result50)&& isnan(final_result40)&&!isnan(final_result30)&&final_result30>0){
                finalresult = final_result30;
                break;
            }
            float final_result20 = predictor(rating_pair_vector, size, 20, 50);
            if (isnan(final_result50)&& isnan(final_result40)&&isnan(final_result30)&&!isnan(final_result20)&&final_result20>0){
                finalresult = final_result20;
                break;
            }
            float final_result10 = predictor(rating_pair_vector, size, 10, 50);
            if (isnan(final_result50)&& isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&!isnan(final_result10)&&final_result10>0){
                finalresult = final_result10;
                break;
            }
            float final_result5 = predictor(rating_pair_vector, size, 5, 50);
            if (isnan(final_result50)&& isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&isnan(final_result10)&&!isnan(final_result5)&&final_result5>0){
                finalresult = final_result5;
                break;
            }else{
                finalresult = 3.5;
            }

            break;
        }

        case 1: {

            float final_result55 = predictor(rating_pair_vector, size, 55, 100);

            if(!isnan(final_result55)&&final_result55>0){
                finalresult = final_result55;
                break;
            }
            float final_result50 = predictor(rating_pair_vector, size, 50, 100);
            if(isnan(final_result55)&&!isnan(final_result50)&&final_result50>0){
                finalresult = final_result50;
                break;
            }
            float final_result40 = predictor(rating_pair_vector, size, 40, 100);
            if(isnan(final_result55)&& isnan(final_result50)&&!isnan(final_result40)&&final_result40>0){
                finalresult = final_result40;
                break;
            }
            float final_result30 = predictor(rating_pair_vector, size, 30 , 100);
            if (isnan(final_result55)&& isnan(final_result50)&&isnan(final_result40)&&!isnan(final_result30)&&final_result30>0){
                finalresult = final_result30;
                break;
            }
            float final_result20 = predictor(rating_pair_vector, size, 20, 100);
            if (isnan(final_result55)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&!isnan(final_result20)&&final_result20>0){
                finalresult = final_result20;
                break;
            }
            float final_result10 = predictor(rating_pair_vector, size, 10, 100);
            if (isnan(final_result55)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&!isnan(final_result10)&&final_result10>0){
                finalresult = final_result10;
                break;
            }
            float final_result5 = predictor(rating_pair_vector, size, 5, 100);
            if (isnan(final_result55)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&isnan(final_result10)&&!isnan(final_result5)&&final_result5>0){
                finalresult = final_result5;
            }else{
                finalresult = 3.5;
            }
            break;
        }

        case 2: {

            float final_result60 = predictor(rating_pair_vector, size, 60, 40);


            if(!isnan(final_result60)&&final_result60>0){
                finalresult = final_result60;
                break;
            }
            float final_result50 = predictor(rating_pair_vector, size, 50, 40);
            if(isnan(final_result60)&&!isnan(final_result50)&&final_result50>0){
                finalresult = final_result50;
                break;
            }
            float final_result40 = predictor(rating_pair_vector, size, 40 , 40);
            if(isnan(final_result60)&& isnan(final_result50)&&!isnan(final_result40)&&final_result40>0){
                finalresult = final_result40;
                break;
            }
            float final_result30 = predictor(rating_pair_vector, size, 30, 40);
            if (isnan(final_result60)&& isnan(final_result50)&&isnan(final_result40)&&!isnan(final_result30)&&final_result30>0){
                finalresult = final_result30;
                break;
            }
            float final_result20 = predictor(rating_pair_vector, size, 20, 40);
            if (isnan(final_result60)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&!isnan(final_result20)&&final_result20>0){
                finalresult = final_result20;
                break;
            }
            float final_result10 = predictor(rating_pair_vector, size, 10, 40);
            if (isnan(final_result60)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&!isnan(final_result10)&&final_result10>0){
                finalresult = final_result10;
                break;
            }
            float final_result5 = predictor(rating_pair_vector, size, 5, 40);
            if (isnan(final_result60)&& isnan(final_result50)&&isnan(final_result40)&&isnan(final_result30)&&isnan(final_result20)&&isnan(final_result10)&&!isnan(final_result5)&&final_result5>0){
                finalresult = final_result5;
                break;
            }
            else{
                finalresult = 3.5;
            }
            
            break;
        }

    }

    return finalresult;

}

#endif //CMP2003PROJECT_FINAL_RATEPREDICTOR_H
