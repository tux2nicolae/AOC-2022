/**
 * Advent of code 2022
 * @author : Nicolae Telechi
 */
#include <algorithm>
#include <assert.h>
#include <bitset>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
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

void print(ofstream & out, const map<AOC::Point, int> & elves)
{
  auto minX = numeric_limits<long long>::max();
  auto minY = numeric_limits<long long>::max();

  auto maxX = numeric_limits<long long>::min();
  auto maxY = numeric_limits<long long>::min();

  for (auto & [elve, direction] : elves)
  {
    minX = min(minX, elve.x);
    minY = min(minY, elve.y);

    maxX = max(maxX, elve.x);
    maxY = max(maxY, elve.y);
  }

  for (auto y = minY; y <= maxY; y++)
  {
    for (auto x = minX; x <= maxX; x++)
    {
      if (elves.count({ x, y }))
        out << "#";
      else
        out << ".";
    }

    out << endl;
  }

  out << endl;
}

struct Blizzard
{
  AOC::Point position;
  char       direction{};
};

pair<AOC::Point, AOC::Point> readData(vector<string> &      lines,
                                      vector<vector<int>> & valley,
                                      vector<Blizzard> &    blizzards)
{
  valley = vector<vector<int>>(lines.size(), vector<int>(lines[0].size()));

  AOC::Point start{ 1, 0 };
  AOC::Point end;

  for (int i = 1; i < lines.size(); i++)
  {
    auto & line = lines[i];
    for (int j = 0; j < line.size(); j++)
    {
      if (line[j] != '#')
      {
        if (line[j] != '.')
        {
          valley[i][j]++;
          blizzards.push_back(Blizzard{ AOC::Point{ j, i }, line[j] });
        }
        else if (i == lines.size() - 1)
        {
          end = { j, i };
        }
      }
    }
  }

  return { start, end };
}

void moveBlizzards(vector<vector<int>> & valley,
                   vector<Blizzard> &    blizzards,
                   AOC::Point            boundary,
                   int                   count)
{
  const auto moduloX = boundary.x;
  const auto moduloY = boundary.y;

  for (auto & [position, direction] : blizzards)
  {
    auto newPosition = position;

    if (direction == '^')
    {
      newPosition.y--;
      newPosition.y = (1000 * moduloX + (newPosition.y - count)) % moduloY;
      newPosition.y++;
    }
    else if (direction == 'v')
    {
      newPosition.y--;
      newPosition.y = (1000 * moduloX + (newPosition.y + count)) % moduloY;
      newPosition.y++;
    }
    else if (direction == '>')
    {
      newPosition.x--;
      newPosition.x = (1000 * moduloX + (newPosition.x + count)) % moduloX;
      newPosition.x++;
    }
    else if (direction == '<')
    {
      newPosition.x--;
      newPosition.x = (1000 * moduloX + (newPosition.x - count)) % moduloX;
      newPosition.x++;
    }
    else
    {
      assert(false);
    }

    assert(1 <= newPosition.x && newPosition.x <= boundary.x);
    assert(1 <= newPosition.y && newPosition.y <= boundary.y);

    valley[position.y][position.x]--;
    valley[newPosition.y][newPosition.x]++;

    position = newPosition;
  }
}

vector<vector<int>> minValley;
vector<Blizzard>    minBlizzards;

map<pair<AOC::Point, int>, int> cost;

int traverse(vector<vector<int>> & valley,
             vector<Blizzard> &    blizzards,
             AOC::Point            boundary,
             AOC::Point            from,
             AOC::Point            to)
{
  stack<AOC::Point> st;
  st.push(from);

  int fewestMinutes = numeric_limits<int>::max();

  while (!st.empty())
  {
    auto current = st.top();
    auto time    = (st.size() - 1) % AOC::Cmmmc(boundary.x, boundary.y);

    if (current == to)
    {
      if ((int)st.size() - 1 < fewestMinutes)
      {
        fewestMinutes = (int)st.size() - 1;
        minValley     = valley;
        minBlizzards  = blizzards;

        cout << fewestMinutes << endl;
      }

      st.pop();
      moveBlizzards(valley, blizzards, boundary, -1);
      continue;
    }

    if (fewestMinutes < (int)st.size() || (int)st.size() > 1000)
    {
      st.pop();
      moveBlizzards(valley, blizzards, boundary, -1);
      continue;
    }

    bool pushed = false;

    moveBlizzards(valley, blizzards, boundary, 1);

    // move to neighbours
    for (const auto & neighbour : current.GetDirect2DNeighbours())
    {
      if (!neighbour.IsInBoundary({ 1, 1 }, boundary) && neighbour != to)
        continue;

      // can't move here
      if (valley[neighbour.y][neighbour.x])
        continue;

      if (!cost.count({ neighbour, time }) || st.size() < cost[{ neighbour, time }])
      {
        cost[{ neighbour, time }] = st.size();
        st.push(neighbour);
        pushed = true;
        break;
      }
    }

    // not move at all
    if (!pushed && !valley[current.y][current.x] &&
        (!cost.count({ current, time }) || st.size() < cost[{ current, time }]))
    {
      cost[{ current, time }] = st.size();
      st.push(current);
      pushed = true;
      continue;
    }

    if (!pushed)
    {
      st.pop();
      moveBlizzards(valley, blizzards, boundary, -2);
    }
  }

  return fewestMinutes;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  AOC::Point boundary = { (int)lines[0].size() - 2, (int)lines.size() - 2 };

  vector<vector<int>> testValley;
  vector<Blizzard>    testBlizzards;

  vector<vector<int>> initialValley;
  vector<Blizzard>    initialBlizzards;

  auto [start, end] = readData(lines, initialValley, initialBlizzards);

  vector<vector<int>> valley    = initialValley;
  vector<Blizzard>    blizzards = initialBlizzards;

  // run test
  testValley = valley;
  moveBlizzards(testValley, testBlizzards, boundary, AOC::Cmmdc(boundary.x, boundary.y));

  for (int i = 1; i < valley.size(); i++)
  {
    for (int j = 0; j < valley[0].size(); j++)
    {
      assert(valley[i][j] == testValley[i][j]);
    }
  }

  // run actual traversal
  cost       = {};
  auto steps = traverse(valley, blizzards, boundary, start, end);

  cout << endl << endl;

  cost               = {};
  auto valleyBack    = minValley;
  auto blizzardsBack = minBlizzards;

  auto stepsBack = traverse(valleyBack, blizzardsBack, boundary, end, start);

  cout << endl << endl;

  cost                       = {};
  auto valleyForwardAgain    = minValley;
  auto blizzardsForwardAgain = minBlizzards;

  auto stepsForwardAgain =
    traverse(valleyForwardAgain, blizzardsForwardAgain, boundary, start, end);

  cout << endl;
  cout << steps << endl;
  cout << stepsBack << endl;
  cout << stepsForwardAgain;

  cout << endl << "PART:" << endl;
  cout << steps + stepsBack + stepsForwardAgain << endl;

  return 0;
}
