#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
// swapping ostringstream objects
#include <string>       // std::string
#include <iostream>
#include <iomanip>
#include <utility>
#include <tuple>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <unordered_set>
#include <float.h>
#include <algorithm>
#include <ctime>

using namespace std;


vector<vector<pair<vector<int>, double>>>vertices;
int numVertices;
unordered_set<string>tabuList;
int timeLimit;

vector<int>bestSolution;
double bestSolutionCost = DBL_MAX;

//Function Declarations
vector<int> keyToListOfInts(string key);
void print(int start, int end);
vector<int>randomize(int size);
string listToString(vector<int> &list);
void printList(vector<int>&list);
void swap(vector<int>&list, int a, int b);

void readInput(){
    cout << "Would you like to set your own time limit? (y/n)?" <<endl;
    char yesOrNo;
    cin >> yesOrNo;
    if (yesOrNo =='y' || yesOrNo =='Y'){
        cout << "Enter time limit in minutes" << endl;
        cin >> timeLimit;
    } else {
        timeLimit = 60;
    }
    string fileName = "data1.txt";

    cout << "Enter file name" << std::endl;
    cin >> fileName;


    ifstream in;

    in.open(fileName);

    if (!in){
        cout << "File could not be opened" << endl;
        exit(1);
    }
    in >> numVertices;
    vertices = vector<vector<pair<vector<int>, double>>>(numVertices + 1);
    for (int i = 1; i <= numVertices; ++i){
        int vertex;
        int lines;
        in >> vertex;
        in >> lines;
        for (int j =0; j < lines; ++j){
            string key;
            double value;
            in >> key;
            in >> value;
            vertices[i].push_back(make_pair(keyToListOfInts(key), value));
        }
    }
    in.close();
}
void print(int start = 1, int end = numVertices){
    for (int i = start; i <= end; ++i){
        cout << "Vertex Number " << i << endl;
        for (auto it = vertices[i].begin(); it != vertices[i].end(); ++it){
            cout << "Key " << listToString(it->first) << " Val " << it->second << endl;
        }
    }
}

void swap(vector<int>&list, int a, int b){
    int temp = list[a];
    list[a] = list[b];
    list[b] = temp;
}
vector<int>randomize(int size = numVertices){
    vector<int>list;
    for (int i = 1; i <= size; ++i){
        list.push_back(i);
    }
    //fisher yates shuffle
    for (int i = 0; i < size - 2; ++i){
        int random = rand() % (size - i);
        swap(list,i,random);
    }
    return list;
}
string listToString(vector<int> &list){
    string answer;
    for (unsigned int i = 0; i < list.size(); ++i){
        answer += to_string(list[i]);
        answer += ",";
    }
    return answer;
}
vector<int> keyToListOfInts(string key){
    vector<int> vect;
    stringstream ss{key};
    char c;
    // get the opening brace
    ss >> c;
    int i;
    while (ss >> i)
    {
        vect.push_back(i);

        if (ss.peek() == ',') {
            ss.ignore();
        } else if (ss.peek() == '}'){
            break;
        }
    }
    return  vect;
}
void printList(vector<int>&list){
    if (list.empty())
        cout << "empty";
    for (auto &num:list){
        cout << num << " ";
    }
    cout << endl;
}
double determineCost(vector<int> &list){
    unordered_set<int> numsSeenSoFar;
    double sum = 0;
    //go through the list of numbers
    for (unsigned int i = 0; i < list.size(); ++i){
        double minCost = DBL_MAX;

        //each vertex has an associated set of sets of list that have some cost
        for (auto &pair:vertices[list[i]]){ //vector of pairs
            int sizeParentSet = (int) pair.first.size();
            int numSeenInParentSet = 0;
            //pair.first is a vector of ints, pair.second is
            for (auto &number:pair.first){
                if (numsSeenSoFar.find(number) != numsSeenSoFar.end()){
                    ++numSeenInParentSet;
                } else {
                    break;
                }
            }
            if (numSeenInParentSet == sizeParentSet){
                if (pair.second < minCost){
                    minCost = pair.second;
                }
            }
        }
        numsSeenSoFar.insert(list[i]);
        if (minCost!= DBL_MAX) {
            sum += minCost;
        }
    }
    return sum;
}

//returns first best neighbour or invalid if it can't find any
tuple<int, int, double> getBestNeighbor(vector<int> &copy, const double cost){
    double bestCostSoFar = DBL_MAX;
    int bestA = -1;
    int bestB = -1;
    string bestStrRep;

    for (int i = 0; i < (numVertices - 1); ++i){
        for (int j = i + 1; j < numVertices; ++j) {
            swap(copy, i, j);
            string strRep = listToString(copy);
            if (tabuList.find(strRep) == tabuList.end()){
                tabuList.insert(strRep);
                double costNew = determineCost(copy);
                if (costNew < bestCostSoFar){
                    bestCostSoFar = costNew;
                    bestA = i;
                    bestB = j;
                    bestStrRep = move(strRep);
                }
            }
            //undo the swap for the next iteration
            swap(copy, i, j);
        }
    }
    tuple<int,int,double> ans = make_tuple(bestA, bestB, bestCostSoFar);
    // if (bestCostSoFar >= cost) {
    //     tabuList.insert(bestStrRep);
    // }
    return ans;
}

double localSearch(vector<int> list, double cost){
    double previousCost = cost;
    double currentCost = cost;
    int numIterations = 0;
    while (true) {
        tuple<int, int, double> bestN = getBestNeighbor(list, currentCost);
        swap(list, get<0>(bestN), get<1>(bestN));
        if(get<2>(bestN) < currentCost){ //only update the previousCost if we've improved
            previousCost = currentCost;
        }
        currentCost = get<2>(bestN);


        if (currentCost < (previousCost * 0.95)) { //reset if we've significantly improved
            numIterations = 0;
            //cout << "significant improvement by going to this node" << endl;
        }

        if (currentCost >= (previousCost * 0.95)) { //we haven't had a 5 percent
            ++numIterations;
        }
        if (numIterations >= numVertices) {
            //cout << "not much improvement, try new start" << endl;
            break;
        }
    }
    return currentCost;
}

void randomStart(){
    time_t start = time(NULL);
    time_t end;

    while (true){
        end = time(NULL);
        double elapsed = difftime(end,start);
        if (elapsed >= ((double)timeLimit * 60)){
            break;
        }
        vector<int> temp = randomize();
        double cost = determineCost(temp);
        string startingList= listToString(temp);
        if (tabuList.find(startingList) != tabuList.end()){
            continue;
        }
        tabuList.insert(startingList);
        startingList="";
        double ans = localSearch(temp, cost);
        if (ans < bestSolutionCost) {
            bestSolutionCost = ans;
            bestSolution = temp;
            cout << endl;
            time_t now = time(0);
            // convert now to string form
            char* dt = ctime(&now);
            cout << "We discover a new solution at time: " << dt << endl;
            printList(temp);
            cout << ans << endl;
        }
    }
}


int main() {
    readInput();
    cout << fixed;
    cout << setprecision(3);
    randomStart();
    cout << "the very best list is" << endl;
    printList(bestSolution);
    cout << bestSolutionCost << endl;


}
