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

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  map<AOC::Point, int> elves;

  for (int i = 0; i < lines.size(); i++)
  {
    auto & line = lines[i];

    for (int j = 0; j < line.size(); j++)
    {
      if (line[j] == '#')
        elves[AOC::Point{ j, i }] = 0;
    }
  }

  out << "Initial state:" << endl;
  print(out, elves);

  int roundsCount = 10;
  for (int round = 1; /*round <= roundsCount*/; round++)
  {
    map<AOC::Point, AOC::Point> fromTo;
    map<AOC::Point, int>        toFrequency;

    auto moveTop = [&](auto & elve) -> bool
    {
      if (elves.count(elve.GetTop()) || elves.count(elve.GetTopLeft()) ||
          elves.count(elve.GetTopRight()))
      {
        return false;
      }

      fromTo[elve] = elve.GetTop();
      toFrequency[elve.GetTop()]++;

      assert(!elves.count(fromTo[elve]));
      return true;
    };

    auto moveBottom = [&](auto & elve) -> bool
    {
      if (elves.count(elve.GetBottom()) || elves.count(elve.GetBottomLeft()) ||
          elves.count(elve.GetBottomRight()))
      {
        return false;
      }

      fromTo[elve] = elve.GetBottom();
      toFrequency[elve.GetBottom()]++;

      assert(!elves.count(fromTo[elve]));
      return true;
    };

    auto moveWest = [&](auto & elve) -> bool
    {
      if (elves.count(elve.GetLeft()) || elves.count(elve.GetTopLeft()) ||
          elves.count(elve.GetBottomLeft()))
      {
        return false;
      }

      fromTo[elve] = elve.GetLeft();
      toFrequency[elve.GetLeft()]++;

      assert(!elves.count(fromTo[elve]));
      return true;
    };

    auto moveEast = [&](auto & elve) -> bool
    {
      if (elves.count(elve.GetRight()) || elves.count(elve.GetTopRight()) ||
          elves.count(elve.GetBottomRight()))
      {
        return false;
      }

      fromTo[elve] = elve.GetRight();
      toFrequency[elve.GetRight()]++;

      assert(!elves.count(fromTo[elve]));
      return true;
    };

    for (const auto & [elve, direction] : elves)
    {
      bool neitherOne = true;
      for (auto & neighbour : elve.GetAll2DNeighbours())
      {
        if (elves.count(neighbour))
        {
          neitherOne = false;
          break;
        }
      }

      if (neitherOne)
        continue;

      vector<function<bool(const AOC::Point & elve)>> directions{ moveTop, moveBottom, moveWest,
                                                                  moveEast };

      rotate(begin(directions), begin(directions) + (direction % 4), end(directions));
      for (auto & goToFunction : directions)
      {
        if (goToFunction(elve))
          break;
      }
    }

    bool moved = false;

    map<AOC::Point, int> nextElves;
    for (const auto & [elve, direction] : elves)
    {
      if (fromTo.count(elve) && toFrequency[fromTo[elve]] == 1)
      {
        // move
        nextElves[fromTo[elve]] = direction + 1;
        moved                   = true;
      }
      else
      {
        // do not move
        nextElves[elve] = direction + 1;
      }
    }

    if (!moved)
    {
      cout << round << endl << endl;
      break;
    }

    assert(elves.size() == nextElves.size());
    elves = nextElves;

    // print(out, elves);
  }

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

  print(out, elves);
  cout << (maxX - minX + 1) * (maxY - minY + 1) - elves.size();

  return 0;
}
