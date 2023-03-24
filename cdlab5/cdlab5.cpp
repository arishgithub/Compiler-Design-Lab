/* 20BCS012 ARISH ANWAR
   CD Lab Program 5 (First and Follow) */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

void getProductionRules(ifstream&, char&, multimap<char, string>&);
void getTermsAndNonTerms(const multimap<char, string>&, set<char>&, set<char>&, vector<char>&);
void getFirstAndFollow(const char&, multimap<char, string>, const set<char>&, const set<char>&, map<char, set<char>>&, map<char, set<char>>&, vector<char>&);
void recursiveNTFirst(const multimap<char, string>&, map<char, set<char>>&, char, char, string);

int main(void)
{
    ifstream fin;
    fin.open("cfg.txt"); // by default open mode = ios::in

    if (!fin)
    {
        cout << "Could not open text file.\n\n";
        return 1;
    }

    char startSymbol;
    multimap<char, string> productionRules;
    set<char> terminals, nonTerminals;
    map<char, set<char>> firsts, follows;
    vector<char> epsilonPNTs;

    // getting production rules
    if (fin.is_open())
        getProductionRules(fin, startSymbol, productionRules);
    
    fin.close();

    // getting sets of terminals and non terminals
    getTermsAndNonTerms(productionRules, terminals, nonTerminals, epsilonPNTs);
    
    // getting firsts and follows
    getFirstAndFollow(startSymbol, productionRules, terminals, nonTerminals, firsts, follows, epsilonPNTs);

    // printing firsts
    cout << "\nPrinting First Sets:\n\n";
    for (const auto& a : firsts)
    {
        cout << a.first << " -> { ";
        for (const auto& b : a.second)
            cout << b << " ";

        cout << "}\n"; 
    }
    cout << endl << endl;

    // printing follows
    cout << "Printing Follow Sets:\n\n";
    for (const auto& a : follows)
    {
        cout << a.first << " -> { ";
        for (const auto& b : a.second)
            cout << b << " ";

        cout << "}\n"; 
    }
}

void getProductionRules(ifstream& file, char& start, multimap<char, string>& production)
{
    char tmpNonTerm;
    string str, tmp;
    stringstream ss;

    getline(file, str);
    
    ss.str(str);
    ss >> tmp;
    start = tmp[0];
    ss >> tmp;
    production.insert({start, tmp});
    ss.clear();

    while (getline(file, str))
    {
        ss.str(str);

        ss >> tmp;
        tmpNonTerm = tmp[0];
        ss >> tmp;
        production.insert({tmpNonTerm, tmp});

        ss.clear();
    } 
}

void getTermsAndNonTerms(const multimap<char, string>& production, set<char>& terminal, set<char>& nonTerminal, vector<char>& epsilonPNT)
{
    for (const auto& a : production)
    {
        nonTerminal.insert(a.first);

        for (const char& b : a.second)
        {
            if (b == '#')
                epsilonPNT.push_back(a.first);
            else if (b < 65 || b > 90)
                terminal.insert(b);
        }
    }
}

void getFirstAndFollow(const char& start, multimap<char, string> production, const set<char>& terminal, const set<char>& nonTerminal, map<char, set<char>>& first, map<char, set<char>>& follow, vector<char>& epsilonPNT)
{
    // putting terminals's firsts
    for (const auto& a : terminal)
        first.insert({a, {a}});

    // dealing with epsilon productions
    vector<pair<char, string>> extraPs;
    for (auto a : production)
    {
        size_t x;
        for (auto b : epsilonPNT)
        {
            x = a.second.find(b);
            if (x != string::npos)
                extraPs.push_back({a.first, a.second.substr(0, x) + a.second.substr(x + 1)});
        }
    }
    for (int i = 0; i < extraPs.size(); i++)
    {
        size_t x;
        for (auto b : epsilonPNT)
        {
            x = extraPs[i].second.find(b);
            if (x != string::npos)
                extraPs.push_back({extraPs[i].first, extraPs[i].second.substr(0, x) + extraPs[i].second.substr(x + 1)});
        }
    }

    for (auto a : extraPs)
        production.insert(a);

    // calculating non terminals's firsts
    for (const auto& a : nonTerminal)
    {
        for (auto it1 = production.lower_bound(a), it2 = production.upper_bound(a); it1 != it2; it1++)            
            recursiveNTFirst(production, first, it1->first, it1->first, it1->second);
    }

    for (auto& a : first)
    {
        if (find(epsilonPNT.begin(), epsilonPNT.end(), a.first) != epsilonPNT.end())
            continue;
        else
            a.second.erase('#');
    }

    for (const auto& a : production)
    {
        if (a.second.length() == 0)
            first.at(a.first).insert('#');
    }

    // calculating follows
    follow[start] = {'$'};

    for (const auto& a : production)
    {
        if (a.second.length() == 0)
            continue;
        
        for (int i = 0; i < a.second.length() - 1; i++)
        {
            if (a.second.length() == 0)
                continue;
            
            if (a.second[i] > 64 && a.second[i] < 91)
            {
                try
                {
                    follow.at(a.second[i]).insert(first.at(a.second[i + 1]).begin(), first.at(a.second[i + 1]).end());
                }
                catch(const std::out_of_range& e)
                {
                    follow[a.second[i]] = {};
                    follow.at(a.second[i]).insert(first.at(a.second[i + 1]).begin(), first.at(a.second[i + 1]).end());
                } 
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (const auto& a : production)
        {
            if (a.second.length() == 0)
                continue;

            try
            {
                if (a.second[a.second.length() - 1] > 64 && a.second[a.second.length() - 1] < 91)
                    follow.at(a.second[a.second.length() - 1]).insert(follow.at(a.first).begin(), follow.at(a.first).end());         
            }
            catch(const std::out_of_range& e)
            {
                follow[a.second[a.second.length() - 1]] = {};
            }      
        }
        i++;
    }

    for (auto& a : follow)
        a.second.erase('#');
}

void recursiveNTFirst(const multimap<char, string>& production, map<char, set<char>>& first, char origNT, char currNT, string currDeriv)
{
    if (currDeriv.length() == 0)
        return;
    else if (currDeriv[0] < 65 || currDeriv[0] > 90)
    {
        try
        {
            first.at(origNT).insert(currDeriv[0]);
        }
        catch(const std::out_of_range& e)
        {
            first[origNT] = {};
            first.at(origNT).insert(currDeriv[0]);
        }
        return;
    }
    else
    {
        if (currNT == currDeriv[0])
            return;
            
        for (auto it1 = production.lower_bound(currDeriv[0]), it2 = production.upper_bound(currDeriv[0]); it1 != it2; it1++)            
            recursiveNTFirst(production, first, origNT, it1->first, it1->second);
    }      
}