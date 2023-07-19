//
// Created by Sercan Eser on 27.12.2022.
//

#ifndef CMP2003_PROJECT_CREATEALLLIST_H
#define CMP2003_PROJECT_CREATEALLLIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <stdexcept>

using namespace std;

bool compareVectorPairFirst(pair<float,float> pair1,pair<float,float> pair2){
    return (pair1.first<pair2.first);

}

pair<float,float> vectorPairBinarySearch(vector<pair<float,float> > vector1, int idBeingSearched, int low, int high){
    while(high>low){
        int mid = (high+low)/2;
        if(idBeingSearched == vector1[mid].first)
            return vector1[mid];
        else if(idBeingSearched > vector1[mid].first){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    return make_pair(-1,-1);
}

    template<typename T>
void pairInsertionSort(pair<T, T> *pair_list, int length, int mode, int order) {

    /*
     * mode = 0; Sorts the pair array with regard to the first element of pair.
     * mode = 1; Sorts the pair array with regard to the second element of pair.
     * order = 0; Sorts the pair in ascending order.
     * order = 1; Sorts the pair in descending order.
     */

    int i, j;
    pair<T, T> key;

    if (order == 0) {
        switch (mode) {
            case 0:
                for (i = 1; i < length; i++) {
                    key = pair_list[i];
                    j = i - 1;

                    while (j >= 0 && pair_list[j].first > key.first) {
                        pair_list[j + 1] = pair_list[j];
                        j = j - 1;
                    }
                    pair_list[j + 1] = key;
                }
                break;
            case 1:
                for (i = 1; i < length; i++) {
                    key = pair_list[i];
                    j = i - 1;

                    while (j >= 0 && pair_list[j].second > key.second) {
                        pair_list[j + 1] = pair_list[j];
                        j = j - 1;
                    }
                    pair_list[j + 1] = key;
                }
                break;
            default:
                throw invalid_argument("Mode can only be 0 (for sorting with regard to first element) "
                                       "or 1 (for sorting with regard to second element).");
        }
    } else if (order == 1) {
        switch (mode) {
            case 0:
                for (i = 1; i < length; i++) {
                    key = pair_list[i];
                    j = i - 1;

                    while (j >= 0 && pair_list[j].first < key.first) {
                        pair_list[j + 1] = pair_list[j];
                        j = j - 1;
                    }
                    pair_list[j + 1] = key;
                }
                break;
            case 1:
                for (i = 1; i < length; i++) {
                    key = pair_list[i];
                    j = i - 1;

                    while (j >= 0 && pair_list[j].second < key.second) {
                        pair_list[j + 1] = pair_list[j];
                        j = j - 1;
                    }
                    pair_list[j + 1] = key;
                    /*
                     // For printing the lists current order
                    for(int q = 0; q < length;q++){
                        cout << pair_list[q].second <<" ";
                    }
                    cout<<endl;
                     */
                }
                break;
            default:
                throw invalid_argument("Mode can only be 0 (for sorting with regard to first element) "
                                       "or 1 (for sorting with regard to second element).");
        }
    } else {
        throw invalid_argument("Order can only be 0 (for sorting in ascending order) "
                               "or 1 (for sorting in descending order).");
    }

    for (i = 1; i < length; i++) {
        key = pair_list[i];
        j = i - 1;

        while (j >= 0 && pair_list[j].second > key.second) {
            pair_list[j + 1] = pair_list[j];
            j = j - 1;
        }
        pair_list[j + 1] = key;
    }
}

/**
 * The function reads data from a CSV file and creates a 2D vector (named "all") of pairs.
 * The pairs in the vector represent movie ratings by users.
 * If mode = 0, pairs are in the form (ItemID, Rating) indexed by UserID in "all".
 * If mode = 1, pairs are in the form (UserID, Rating) indexed by ItemID in "all".
 * The function also sorts sub-vectors of "all" based on ItemID or UserID.
 *
 * @param fileName: Name of the CSV file to read data from.
 * @param mode: Operation mode of the function (0 or 1).
 * @return A 2D vector of pairs representing movie ratings by users.
 */
vector<vector<pair<float, float> > > createAllList(string const file_name, int mode) {

    //mode = 0; is to create a user based vector (indices of outer vector will correspond to ID's of users.).
    //mode = 1; is to create an item based vector (indices of outer vector will correspond to ID's of items.).
    //Any other mode is undefined and will give an error if used.

    //Vector that is being used to quickly access all the data in csv file.
    vector<vector<pair<float, float> > > all;
    //Variables used for reading the csv file.
    vector<vector<string> > content;
    vector<string> row;
    string line, word;
    // Largest User ID and Item ID in the given list.
    int max_user_id = 0, max_item_id = 0;


    fstream file(file_name, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    } else
        cout << "Could not open the file\n";

    for (int i = 1; i < content.size(); i++) {
        if (stoi(content[i][0]) > max_user_id) {
            max_user_id = stoi(content[i][0]);
        }
        if (stoi(content[i][1]) > max_item_id) {
            max_item_id = stoi(content[i][1]);
        }
    }
    switch (mode) {
        case 0:
            for (int i = 0; i < max_user_id + 1; i++) {
                vector<pair<float, float> > temp;
                all.push_back(temp);
            }
            for (int i = 1; i < content.size(); i++) {
                all[stoi(content[i][0])].push_back(make_pair(stof(content[i][1]), stof(content[i][2])));
            }
            break;
        case 1:
            for (int i = 0; i < max_item_id + 1; i++) {
                vector<pair<float, float> > temp;
                all.push_back(temp);
            }
            for (int i = 1; i < content.size(); i++) {
                all[stoi(content[i][1])].push_back(make_pair(stof(content[i][0]), stof(content[i][2])));
            }
            break;
        default:
            throw invalid_argument("Mode can only be 0 (for user based list) or 1 (for item based list).");
    }
    for(int i = 0;i< all.size();i++) {
        sort(all[i].begin(), all[i].end(), compareVectorPairFirst);
    }
    return all;
}

#endif //CMP2003_PROJECT_CREATEALLLIST_H