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

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  AOC::Point start, exit;

  vector<vector<int>> cost(lines.size(), vector<int>(lines[0].size(), numeric_limits<int>::max()));

  queue<AOC::Point> q;
  set<AOC::Point>   visited;

  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[0].size(); j++)
    {
      // part 2 ->   || lines[i][j] == 'a'
      if (lines[i][j] == 'S' || lines[i][j] == 'a')
      {
        q.push({ i, j });
        cost[i][j] = 0;
        visited.insert({ i, j });
      }
      else if (lines[i][j] == 'E')
      {
        exit = { i, j };
      }
    }
  }

  while (!q.empty())
  {
    auto current = q.front();
    q.pop();

    for (auto & neighbour : current.GetDirectNeighbours())
    {
      if (!neighbour.IsInBoundary({ 0, 0 }, { (int)cost.size() - 1, (int)cost[0].size() - 1 }))
        continue;

      if (visited.count(neighbour))
        continue;

      int currentElevation = lines[current.x][current.y] == 'S' ? 'a' : lines[current.x][current.y];
      int destinationElevation =
        lines[neighbour.x][neighbour.y] == 'E' ? 'z' : lines[neighbour.x][neighbour.y];

      if (destinationElevation - currentElevation <= 1)
      {
        q.push(neighbour);
        visited.insert(neighbour);

        cost[neighbour.x][neighbour.y] =
          min(cost[neighbour.x][neighbour.y], cost[current.x][current.y] + 1);
      }
    }
  }

  cout << cost[exit.x][exit.y];

  FStreamWriter writer(out);
  writer.WriteMatrix(cost);

  return 0;
}
