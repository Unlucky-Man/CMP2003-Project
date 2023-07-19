#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "createAllList.h"
#include "ratePredictor.h"

using namespace std;

int main() {
    vector<vector<pair<float, float> > > rating_pair_vector;
    //vector<vector<vector<pair<float,float> > > > ALL_HAIL_THE_OMEGA_VECTOR; // All users, all "rating pair" vectors.
    string train = "train.csv";
    string test = "test.csv";
    //A list created from train.csv with indices corresponding to User IDs.
    vector<vector<pair<float, float> > > user_based_all_list_train = createAllList(train, 0);
    //A list created from test.csv with indices corresponding to IDs (the order, from top to down).
    vector<vector<pair<float, float> > > user_based_all_list_test = createAllList(test, 0);
    //A list created from train.csv with indices corresponding to Item IDs.
    vector<vector<pair<float, float> > > item_based_all_list_train = createAllList(train, 1);
    //Lists created from user_based_all_list_train and item_based_all_list_train with indices corresponding to Users/Items
    //and elements to number of Items watched/People who have watched Item.
    vector<int> all_users_item_count, all_items_user_count;
    //max_10_users: The vector which holds 10 users who have watched the most films.
    //max_10_items: The vector which holds 10 films which have been most watched by the users.
    pair<int, int> max_10_users[10], max_10_items[10];
    //A more accurate rating pair vector. It only has requiredQuality or higher amount of elements in every sub-vector.
    vector<vector<pair<float, float> > > quality_rating_pair_vector;
    //Vectors for store results in the final
    vector<float> container;
    vector<float> container2;
    vector<float> finalcontainer;

    auto start = chrono::high_resolution_clock::now();

    //creating the list which stores 10 users who have watched the most films.

    for (int j = 0; j < user_based_all_list_train.size(); j++) {
        all_users_item_count.push_back(user_based_all_list_train[j].size());
    }
    for (int k = 0; k < all_users_item_count.size(); k++) {
        if (k > 10 && all_users_item_count[k] > max_10_users[0].second) {
            max_10_users[0] = make_pair(k, all_users_item_count[k]);
            pairInsertionSort(max_10_users, 10,0,0);
        } else if (k < 10) {
            max_10_users[k] = make_pair(k, all_users_item_count[k]);
        }
    }

    //creating the list which stores 10 films which have been most watched by the users.

    for (int j = 0; j < item_based_all_list_train.size(); j++) {
        all_items_user_count.push_back(item_based_all_list_train[j].size());
    }
    for (int k = 0; k < all_items_user_count.size(); k++) {
        if (k > 10 && all_items_user_count[k] > max_10_items[0].second) {
            max_10_items[0] = make_pair(k, all_items_user_count[k]);
            pairInsertionSort(max_10_items, 10,0,1);
        } else if (k < 10) {
            max_10_items[k] = make_pair(k, all_items_user_count[k]);
        }
    }

    for (auto x: max_10_users) {
        cout << "User: " << x.first << " " << "Number of films watched: " << x.second << endl;
    }

    cout<< endl;

    for (auto x: max_10_items) {
        cout << "Item: " << x.first << " " << "Number of users watched: " << x.second << endl;
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time to find top 10 Users and Films: "<< duration.count()/1000.0 << " milliseconds"<< endl;


    //all_users_item_count generator.
    for (int j = 0; j < user_based_all_list_train.size(); j++) {
        all_users_item_count.push_back(user_based_all_list_train[j].size());
    }
    //all_items_user_count generator.
    for (int j = 0; j < item_based_all_list_train.size(); j++) {
        all_items_user_count.push_back(item_based_all_list_train[j].size());
    }
    auto start2 = chrono::high_resolution_clock::now();
    //predictionIndex is equal to "ID" in the test.csv file. It is the "row number" to find which userID we are working with.
    for (int predictionIndex = 0; predictionIndex < user_based_all_list_test.size(); predictionIndex++) {
        //UserID is being assigned to predicted_user.
        int predicted_user = user_based_all_list_test[predictionIndex][0].first;
        //ItemID is being assigned to predicted_item.
        int predicted_item = user_based_all_list_test[predictionIndex][0].second;
        //Amount of films which "UserID" has watched is being assigned to predicted_users_item_count.
        int predicted_users_item_count = user_based_all_list_train[predicted_user].size();
        //Pair list which stores films and their corresponding "total watch amounts" is created.
        pair<float, float> predicted_users_items[predicted_users_item_count + 1];
        //Pair list which stores films and their corresponding "total watch amounts" is filled.
        for (int k = 0; k < predicted_users_item_count; k++) {
            int item_id = user_based_all_list_train[predicted_user][k].first; // NOLINT(cppcoreguidelines-narrowing-conversions)
            predicted_users_items[k] = make_pair(item_id, all_items_user_count[item_id]);
        }
        pairInsertionSort(predicted_users_items, predicted_users_item_count, 1, 1);
        //Pair-vector-vector which stores pairs of ratings. One side of the rating pair is prediction_user's rating and
        //the other side is the rating of every other user who have watched the predicted_item.
        int requiredQuality = 5;
        for (int h = 0; h < item_based_all_list_train[predicted_item].size(); h++) {
            //Two indices to compare two users' items.
            int i = 0, j = 0;
            //Temporary vector being pushed back into rating_pair_vector to create a vector to hold pairs.
            vector<pair<float, float> > temp;
            rating_pair_vector.push_back(temp);
            //quality_rating_pair_vector.push_back(temp);
            //The rating which was given by the "userID" to predicted_item is being stored with a "token" -1.
            rating_pair_vector[h].push_back(make_pair(-1, item_based_all_list_train[predicted_item][h].second));
            //The algorithm to compare two users Items, very quickly.
            while (i < user_based_all_list_train[predicted_user].size() &&
                   j < user_based_all_list_train[item_based_all_list_train[predicted_item][h].first].size()) {
                while (user_based_all_list_train[predicted_user][i].first >
                       user_based_all_list_train[item_based_all_list_train[predicted_item][h].first][j].first &&
                       j < user_based_all_list_train[item_based_all_list_train[predicted_item][h].first].size()) {
                    j++;
                }
                while (user_based_all_list_train[predicted_user][i].first <
                       user_based_all_list_train[item_based_all_list_train[predicted_item][h].first][j].first &&
                       i < user_based_all_list_train[predicted_user].size()) {
                    i++;
                }
                if (user_based_all_list_train[predicted_user][i].first ==
                    user_based_all_list_train[item_based_all_list_train[predicted_item][h].first][j].first) {
                    rating_pair_vector[h].push_back(make_pair(user_based_all_list_train[predicted_user][i].second,
                                                              user_based_all_list_train[item_based_all_list_train[predicted_item][h].first][j].second));
                    j++;
                }
            }

        }

        //These vectors take the result of two different mode of finalFunction
        container.push_back(finalFunction(rating_pair_vector,item_based_all_list_train[predicted_item].size(),0));
        container2.push_back(finalFunction(rating_pair_vector,item_based_all_list_train[predicted_item].size(),1));

        //container.push_back(finalFunction(rating_pair_vector,item_based_all_list_train[predicted_item].size(),2));
        //container2.push_back(finalFunction(rating_pair_vector,item_based_all_list_train[predicted_item].size(),1));

        quality_rating_pair_vector.clear();
        rating_pair_vector.clear();
    }



    //Simply take the average of these two vectors
    for (int i = 0; i < container.size(); ++i) {
        finalcontainer.push_back((container[i]+container2[i])/2);
    }

    //And we write results to the submission.csv
    fstream submissionfile;
    submissionfile.open("submission.csv",ios::out | ios::app);
    submissionfile <<"ID"<<","<<"Predicted"<<endl;
    for (int j = 0; j < finalcontainer.size(); ++j) {
        submissionfile <<j<<","<<finalcontainer[j]<< endl;
    }

// If wanted the bottom code can be un-commented to print out all predictions.
/*
    submissionfile.close();
    for(auto x: finalcontainer){
        cout<<x<<endl;
    }
*/
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(stop2 - start2);
    cout <<  "Time to run all of the algorithm: "<< duration2.count()/1000.0<< " seconds"<< endl;

    return 0;
}




