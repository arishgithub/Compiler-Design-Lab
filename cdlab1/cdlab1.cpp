/* 20BCS012 ARISH ANWAR
   CD Lab Program 1 (DFA) */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void getInitialState(ifstream&, int&);
void getFinalStates(ifstream&, unordered_set<int>&);
void getTransitionTable(ifstream&, vector<vector<int>>&);
int checkStringAcceptance(const string&, const int&, const unordered_set<int>&, const vector<vector<int>>&);

int main(void)
{
    ifstream fin;
    fin.open("dfa.txt"); // by default open mode = ios::in

    if (!fin)
    {
        cout << "Could not open text file.\n\n";
        return 1;
    }

    int initialState;
    unordered_set<int> finalStates;
    vector<vector<int>> transitions;

    // getting the initial state
    if (fin.is_open())
        getInitialState(fin, initialState);
      
    // getting the set of final states
    if (fin.is_open())
        getFinalStates(fin, finalStates);

    // getting the transition table
    if (fin.is_open())
        getTransitionTable(fin, transitions);
    
    fin.close();

    // taking input from the user
    string input;
    char check;
    cout << "\n";
    while (1)
    {
        cout << "Enter input: ";
        getline(cin, input);
        cout << endl;

        // checking string acceptance
        if (checkStringAcceptance(input, initialState, finalStates, transitions))
           cout << "Output: String accepted\n\n";
        else
            cout << "Output: String not accepted\n\n";
        
        cout << "Do you wish to continue?(y/n) ";
        cin >> check;
        cin.ignore();
        cout << endl << endl;
        if (check != 'y')
            break;     
    }   
}

void getInitialState(ifstream& file, int& initial)
{
    string str;

    getline(file, str);
    initial = stoi(str);
}

void getFinalStates(ifstream& file, unordered_set<int>& final)
{
    string str, tmp;
    stringstream ss;

    getline(file, str);
    ss.str(str);
    while (ss >> tmp)
        final.insert(stoi(tmp));
    ss.clear();
}

void getTransitionTable(ifstream& file, vector<vector<int>>& transition)
{
    string str, tmp;
    stringstream ss;
    int i = 0;

    while (getline(file, str))
    {
        transition.push_back(vector<int>());
        ss.str(str);
        while (ss >> tmp)
            transition[i].push_back(stoi(tmp));

        ss.clear();
        i++;
    }
}

int checkStringAcceptance(const string& inp, const int& initial, const unordered_set<int>& final, const vector<vector<int>>& transition)
{
    if (inp == "")
        return (final.count(initial)) ? 1 : 0;

    int current = initial;
    for (int i = 0, n = inp.length(); i < n; i++)
    {
        current = transition[current][int(inp[i] - 48)];
        if (current == -1)
            return 0;        
    }
    
    return (final.count(current)) ? 1 : 0;
}