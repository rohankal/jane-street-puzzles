#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
using namespace std;

struct PuzzleCell
{
    int assignedValue;
    set<int> possibleValues;
    char region;
    int row;
    int col;
    bool finalized;
};

PuzzleCell puzzle[5][5];
map<char, int> regionSizes{
    {'a', 5}, {'b', 5}, {'c', 5}, {'d', 1}, {'e', 1}, {'f', 4}, {'g', 3}};

void populatePuzzle()
{
    ifstream fin("example.txt");
    string line;
    int currentRow = 0;
    while (getline(fin, line))
    {
        int currentCol = 0;
        for (auto ch : line)
        {
            int value = 0;
            char region = ch;
            set<int> possibleValues;
            bool finalized = false;
            int regionSize = regionSizes[region];
            if (regionSize == 1)
            {
                value = 1;
                finalized = true;
            }
            for (int i = 1; i <= regionSize; i++)
            {
                possibleValues.insert(i);
            }
            PuzzleCell cell = {value, possibleValues, region, currentRow, currentCol, false};

            puzzle[currentRow][currentCol] = cell;
            currentCol++;
        }
        currentRow++;
    }
}

void displayPuzzle()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << puzzle[i][j].assignedValue << " ";
        }
        cout << endl;
    }
}

set<int> getPossibleValuesOfCell(int row, int col)
{
    return puzzle[row][col].possibleValues;
}

int main()
{
    populatePuzzle();
    displayPuzzle();
    return 0;
}
