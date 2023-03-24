/* 20BCS012 ARISH ANWAR
   CD Lab Program 3 (Moore) */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void getInitialState(ifstream&, int&);
void getTransitionTable(ifstream&, vector<pair<vector<int>, string>>&);
string getOutputMoore(const string&, const int&, const vector<pair<vector<int>, string>>&);

int main(void)
{
    ifstream fin;
    fin.open("moore.txt"); // by default open mode = ios::in

    if (!fin)
    {
        cout << "Could not open text file.\n\n";
        return 1;
    }

    int initialState;
    vector<pair<vector<int>, string>> transitions;

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

        // getting moore output
        cout << "Output: " << getOutputMoore(input, initialState, transitions) << "\n\n";
    
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

void getTransitionTable(ifstream& file, vector<pair<vector<int>, string>>& transition)
{
    string str, tmp;
    stringstream ss;
    int i = 0;

    while (getline(file, str))
    {
        transition.push_back(pair<vector<int>, string>({}, ""));
        ss.str(str);
        while (ss >> tmp)
        {
            try
            {
                transition[i].first.push_back(stoi(tmp));
            }
            catch(const std::invalid_argument& e)
            {}
        }

        transition[i].second = tmp;
        ss.clear();
        i++;
    }
}

string getOutputMoore(const string& inp, const int& initial, const vector<pair<vector<int>, string>>& transition)
{
    string out = transition[initial].second;
    if (inp == "")
        return out;

    int current = initial;
    for (int i = 0, n = inp.length(); i < n; i++)
    {
        current = transition[current].first[int(inp[i] - 48)];
        if (current == -1)
            return out;

        out += transition[current].second;
    }   

    return out;
}