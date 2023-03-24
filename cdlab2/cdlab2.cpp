/* 20BCS012 ARISH ANWAR
   CD Lab Program 2 (Mealy) */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void getInitialState(ifstream&, int&);
void getTransitionTable(ifstream&, vector<vector<pair<int, string>>>&);
string getOutputMealy(const string&, const int&, const vector<vector<pair<int, string>>>&);

int main(void)
{
    ifstream fin;
    fin.open("mealy.txt"); // by default open mode = ios::in

    if (!fin)
    {
        cout << "Could not open text file.\n\n";
        return 1;
    }

    int initialState;
    vector<vector<pair<int, string>>> transitions;

    // getting the initial state
    if (fin.is_open())
        getInitialState(fin, initialState);

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

        // getting mealy output
        cout << "Output: " << getOutputMealy(input, initialState, transitions) << "\n\n";
    
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

void getTransitionTable(ifstream& file, vector<vector<pair<int, string>>>& transition)
{
    string str, tmp1, tmp2;
    stringstream ss;
    int i = 0;

    while (getline(file, str))
    {
        transition.push_back({});
        ss.str(str);
        while (ss >> tmp1 && ss >> tmp2)
            transition[i].push_back(pair<int, string>(stoi(tmp1), tmp2));

        ss.clear();
        i++;
    }
}

string getOutputMealy(const string& inp, const int& initial, const vector<vector<pair<int, string>>>& transition)
{
    string out = "";
    if (inp == "")
        return out;

    int current = initial;
    for (int i = 0, n = inp.length(); i < n; i++)
    {
        out += transition[current][int(inp[i] - 48)].second;
        current = transition[current][int(inp[i] - 48)].first;
        if (current == -1)
            return out.substr(0, out.length() - 2);
    }   

    return out;
}