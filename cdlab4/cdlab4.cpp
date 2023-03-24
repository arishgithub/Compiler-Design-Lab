/* 20BCS012 ARISH ANWAR
   CD Lab Program 4 (Nfa to Dfa) */

#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

void getInitialState(ifstream&, int&);
void getFinalStates(ifstream&, set<int>&);
void getTransitionTable(ifstream&, vector<vector<vector<int>>>&, vector<pair<int, int>>&);
void nfaToDfa(set<int>&, vector<vector<vector<int>>>&, vector<pair<int, int>>&);
void toIncreaseSize(vector<vector<vector<int>>>&, int);

int main(void)
{
    ifstream fin;
    fin.open("nfa.txt"); // by default open mode = ios::in

    if (!fin)
    {
        cout << "Could not open text file.\n\n";
        return 1;
    }

    int initialState;
    set<int> finalStates;
    vector<vector<vector<int>>> transitions;
    vector<pair<int, int>> multipleTransitions;

    // getting the initial state
    if (fin.is_open())
        getInitialState(fin, initialState);
      
    // getting the set of final states
    if (fin.is_open())
        getFinalStates(fin, finalStates);

    // getting the transition table
    if (fin.is_open())
        getTransitionTable(fin, transitions, multipleTransitions);

    fin.close();

    // nfa to dfa conversion
    nfaToDfa(finalStates, transitions, multipleTransitions);
    
    // printing corresponding dfa
    cout << endl << "Corresponding DFA:\n\n";
    cout << setw(2) << initialState << endl;

    for (set<int>::iterator it = finalStates.begin(); it != finalStates.end(); it++)
        cout << setw(2) << *it << " ";
    cout << endl;

    for (const auto& a : transitions)
    {
        for (const auto& b: a)
            cout << setw(2) << b[0] << " ";

        cout << endl;
    }
    cout << endl;
}

void getInitialState(ifstream& file, int& initial)
{
    string str;

    getline(file, str);
    initial = stoi(str);
}

void getFinalStates(ifstream& file, set<int>& final)
{
    string str, tmp;
    stringstream ss;

    getline(file, str);
    ss.str(str);
    while (ss >> tmp)
        final.insert(stoi(tmp));
    ss.clear();
}

void getTransitionTable(ifstream& file, vector<vector<vector<int>>>& transition, vector<pair<int, int>>& multiple)
{
    string str, tmp;
    stringstream ss, ss2;
    int i = 0, j;

    while (getline(file, str))
    {
        transition.push_back({});
        ss.str(str);
        j = 0;
        while (ss >> tmp)
        {
            transition[i].push_back({});
            if (tmp.find(',') != string::npos)
                multiple.push_back({i, j});
            
            ss2.str(tmp);
            while (getline(ss2, tmp, ','))
                transition[i][j].push_back(stoi(tmp));
            
            j++;
            ss2.clear();
        }
        ss.clear();
        i++;
    }

    // sorting the multiple transitions for same input
    for (auto& a: transition)
    {
        for (auto& b: a)            
            sort(b.begin(), b.end());
    }
}

void nfaToDfa(set<int>& final, vector<vector<vector<int>>>& transition, vector<pair<int, int>>& multiple)
{
    int j = transition.size(), originalFinal = *final.begin();
    map<vector<int>, int> modify;
    set<int> tmp;

    for (int i = 0; i < multiple.size(); i++)
    {
        if (modify.count(transition[multiple[i].first][multiple[i].second]))
            continue;

        modify.insert({transition[multiple[i].first][multiple[i].second], j});
        
        // resizing transition table to accomodate new transitions
        toIncreaseSize(transition, j);

        // checking if new state is a final state as well
        if (find(transition[multiple[i].first][multiple[i].second].begin(), transition[multiple[i].first][multiple[i].second].end(), originalFinal) != transition[multiple[i].first][multiple[i].second].end())
            final.insert(j);

        // transition addition
        for (int k = 0, n = transition[0].size(); k < n; k++)
        {
            tmp.clear();
            for (const auto& a : transition[multiple[i].first][multiple[i].second])
            {
                for (const auto& b : transition[a][k])
                    tmp.insert(b);
            }
            tmp.erase(-1);            
            if (tmp.empty())
            {
                transition[j][k] = {-1};
                continue;
            }

            transition[j][k] = vector<int>(tmp.begin(), tmp.end());

            if (tmp.size() > 1)
                multiple.push_back({j, k});
        }
        j++;
    }

    // putting states numbers in transition table
    for (int i = 0; i < multiple.size(); i++)
        transition[multiple[i].first][multiple[i].second] = {modify.at(transition[multiple[i].first][multiple[i].second])};
}

void toIncreaseSize(vector<vector<vector<int>>>& transition, int n)
{
    transition.resize(n + 1);
    for (int i = 0; i < n + 1; i++)
    {
        int k = transition[0].size();
        transition[i].resize(k, vector<int>(n + 1));
    }
}