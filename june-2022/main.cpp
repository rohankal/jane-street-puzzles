#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include "prettyprint.hpp"

using namespace std;

#define N 5

struct PuzzleCell
{
    int assignedValue;
    set<int> possibleValues;
    char region;
    int row;
    int col;
    bool finalized;
};

vector<vector<PuzzleCell>> puzzle;
map<char, int> regionSizes{
    {'a', 5}, {'b', 5}, {'c', 5}, {'d', 1}, {'e', 1}, {'f', 4}, {'g', 3}, {'h', 1}};
map<char, set<int>> regionUsed{
    {'a', {}}, {'b', {}}, {'c', {}}, {'d', {}}, {'e', {}}, {'f', {}}, {'g', {}}, {'h', {}}};

void populatePuzzle()
{
    ifstream fin("example.txt");
    string line;
    int currentRow = 0;
    while (getline(fin, line))
    {
        vector<PuzzleCell> toAdd;
        int currentCol = 0;
        for (auto ch : line)
        {
            int assignedValue = 0;
            char region = ch;
            set<int> possibleValues;
            bool finalized = false;
            int regionSize = regionSizes[region];
            if (regionSize == 1)
            {
                assignedValue = 1;
                finalized = true;
            }
            for (int i = 1; i <= regionSize; i++)
            {
                possibleValues.insert(i);
            }
            PuzzleCell cell = {assignedValue, possibleValues, region, currentRow, currentCol, finalized};
            toAdd.push_back(cell);
            currentCol++;
        }
        puzzle.push_back(toAdd);
        currentRow++;
    }

    ifstream instream("examplePS.txt");
    while (getline(instream, line))
    {
        cout << line << endl;
        int value = (int)(line.at(0)) - 48;
        int row = (int)line.at(1) - 48;
        int col = (int)line.at(2) - 48;
        cout << row << " " << col << " " << value << endl;
        puzzle[row][col].assignedValue = value;
    }
}

void displayPuzzle()
{
    for (int i = 0; i < puzzle.size(); i++)
    {
        for (int j = 0; j < puzzle[0].size(); j++)
        {
            cout << puzzle[i][j].assignedValue << " ";
        }
        cout << endl;
    }
}

set<int> getPossibleValuesOfCell(int row, int col)
{
    set<int> toRet = puzzle[row][col].possibleValues;
    cout << toRet << endl;
    return toRet;
}

int calculateAnswer()
{
    int toRet = 0;
    int rowProduct;
    for (int i = 0; i < puzzle.size(); i++)
    {
        rowProduct = 1;
        for (int j = 0; j < puzzle[0].size(); j++)
        {
            rowProduct *= puzzle[i][j].assignedValue;
        }
        toRet += rowProduct;
    }
    return toRet;
}

bool beenUsed(int row, int col, int num)
{
    set<int> used = regionUsed[puzzle[row][col].region];
    cout << "Used Set: " << used << endl;
    if (used.find(num) == used.end())
    {
        return false;
    }
    return true;
}

bool isSafe(int row, int col, int num)
{
    int minTaxicabDistance = INT_MAX;
    int currentTaxicabDistance;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (puzzle[i][j].assignedValue == num)
            {
                currentTaxicabDistance = abs(i - row) + abs(j - col);
                if (currentTaxicabDistance < minTaxicabDistance)
                {
                    minTaxicabDistance = currentTaxicabDistance;
                }
            }
        }
    }
    if (minTaxicabDistance == num || minTaxicabDistance == INT_MAX)
    {
        return true;
    }
    return false;
}
bool solvePuzzle(int row, int col)
{
    if (row == N - 1 && col == N)
        return true;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (puzzle[row][col].assignedValue != 0)
        return solvePuzzle(row, col + 1);

    set<int> possibleNumbers = getPossibleValuesOfCell(row, col);

    for (auto num : possibleNumbers)
    {
        if (beenUsed(row, col, num))
        {
            continue;
        }
        else
        {
            if (isSafe(row, col, num))
            {
                puzzle[row][col].assignedValue = num;
                regionUsed[puzzle[row][col].region].insert(num);
                displayPuzzle();
                cout << endl;
                if (solvePuzzle(row, col + 1))
                    return true;
                regionUsed[puzzle[row][col].region].erase(num);
                puzzle[row][col].assignedValue = 0;
            }
        }
    }
    return false;
}

int main()
{
    populatePuzzle();
    displayPuzzle();
    // if (solvePuzzle(0, 0))
    // {
    //     cout << calculateAnswer() << endl;
    // }
    // else
    // {
    //     cout << "There is no solution to this particular puzzle." << endl;
    // }
    // return 0;
}
