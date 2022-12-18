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
#include <regex>
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

map<AOC::Point, set<AOC::Point>> collisions;

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);

  auto lines = reader.ReadLines();

  vector<vector<int>> data;
  for (auto & line : lines)
  {
    data.push_back({});

    auto xxx = AOC::Explode(line, ',');

    transform(begin(xxx), end(xxx), back_inserter(data.back()),
              [](string number)
              {
                return atoi(number.c_str());
              });
  }

  for (int i = 0; i < data.size(); i++)
  {
    for (int j = i + 1; j < data.size(); j++)
    {
      auto & a = data[i];
      auto & b = data[j];

      auto first  = AOC::Point({ a[0], a[1], a[2] });
      auto second = AOC::Point({ b[0], b[1], b[2] });

      collisions[first];
      collisions[second];

      for (const auto & neighbour : first.GetDirect3DNeighbours())
      {
        if (neighbour == second)
          collisions[first].insert(second);
      }

      for (const auto & neighbour : second.GetDirect3DNeighbours())
      {
        if (neighbour == first)
          collisions[second].insert(first);
      }
    }
  }

  map<AOC::Point, bool> freeCube;
  queue<AOC::Point>     q;
  q.push({ 0, 0, 0 });

  while (!q.empty())
  {
    auto currentNode = q.front();
    q.pop();

    for (auto & neighbour : currentNode.GetDirect3DNeighbours())
    {
      if (!neighbour.IsInBoundary({ -1, -1, -1 }, { 100, 100, 100 }))
        continue;

      // not a point
      if (!freeCube.count(neighbour) && !collisions.count(neighbour))
      {
        freeCube[neighbour] = true;
        q.push(neighbour);
      }
    }
  }

  int s = 0;
  for (auto & [point, collisionsSet] : collisions)
  {
    for (auto & neighbour : point.GetDirect3DNeighbours())
    {
      if (!collisionsSet.count(neighbour))
      {
        if (freeCube[neighbour])
          s++;
      }
    }
  }

  cout << s;

  return 0;
}
