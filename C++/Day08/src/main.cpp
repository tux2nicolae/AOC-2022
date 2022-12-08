/**
 * Advent of code 2022
 * @author : Nicolae Telechi
 */
#include <algorithm>
#include <assert.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Parse.h"
#include "../../AOCLib/src/Point.h"
#include "../../AOCLib/src/Ship.h"
#include "../../AOCLib/src/Time.h"
#include "../../AOCLib/src/Util.h"

int GetScenicScore(const vector<vector<int>> & v, int houseLine, int houseColumn)
{
  int homeRight = 1;
  int homeLeft  = 1;
  int homeDown  = 1;
  int homeUp    = 1;

  // home -> right
  for (int i = houseColumn + 1; i < v[houseLine].size() - 1; i++)
  {
    if (v[houseLine][houseColumn] > v[houseLine][i])
      homeRight++;
    else
      break;
  }

  // home -> right
  for (int i = houseColumn - 1; i > 0; i--)
  {
    if (v[houseLine][i] < v[houseLine][houseColumn])
      homeLeft++;
    else
      break;
  }

  // home -> down
  for (int i = houseLine + 1; i < v.size() - 1; i++)
  {
    if (v[houseLine][houseColumn] > v[i][houseColumn])
      homeDown++;
    else
      break;
  }

  // home -> up
  for (int i = houseLine - 1; i > 0; i--)
  {
    if (v[i][houseColumn] < v[houseLine][houseColumn])
      homeUp++;
    else
      break;
  }

  return homeRight * homeLeft * homeDown * homeUp;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          v      = reader.ReadDataAsMatrixOfDigits();
  auto          backup = v;

  set<pair<int, int>> idexes;

  // lines
  for (int line = 0; line < v.size(); line++)
  {
    idexes.insert(make_pair(line, 0));
    idexes.insert(make_pair(line, v[line].size() - 1));

    // left -> right
    auto ln = v[line];
    for (int i = 1; i < v[line].size(); i++)
    {
      if (ln[i - 1] < ln[i])
        idexes.insert(make_pair(line, i));
      else
        ln[i] = ln[i - 1];
    }

    // right -> left
    ln = v[line];
    for (int i = ln.size() - 2; i >= 0; i--)
    {
      if (v[line][i] > v[line][i + 1])
        idexes.insert(make_pair(line, i));
      else
        v[line][i] = v[line][i + 1];
    }
  }

  // columns
  for (int column = 0; column < v.size(); column++)
  {
    idexes.insert({
      0,
      column,
    });

    idexes.insert(make_pair(int(v.size() - 1), column));

    v = backup;

    // top -> bottom
    for (int line = 1; line < v.size(); line++)
    {
      if (v[line][column] > v[line - 1][column])
        idexes.insert(make_pair(line, column));
      else
        v[line][column] = v[line - 1][column];
    }

    v = backup;

    // bottom -> top
    for (int line = v.size() - 2; line >= 0; line--)
    {
      if (v[line][column] > v[line + 1][column])
        idexes.insert({ line, column });
      else
        v[line][column] = v[line + 1][column];
    }
  }

  cout << idexes.size() << endl;

  int maxScore = 0;
  for (int i = 0; i < v.size(); i++)
  {
    for (int j = 0; j < v[0].size(); j++)
    {
      maxScore = max(maxScore, GetScenicScore(v, i, j));
    }
  }

  cout << maxScore;

  return 0;
}
