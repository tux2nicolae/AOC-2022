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

vector<vector<AOC::Point>> ParseInput(const vector<string> & lines)
{
  vector<vector<AOC::Point>> rocksCoordinates;

  for (int i = 0; i < lines.size(); i++)
  {
    auto & line = lines[i];

    vector<AOC::Point> rocksLine;

    auto tokens = AOC::Explode(line, " -> ");
    for (auto token : tokens)
    {
      auto coorinates = AOC::Explode(token, ",");
      rocksLine.push_back(AOC::Point{ atoi(coorinates[0].c_str()), atoi(coorinates[1].c_str()) });
    }

    rocksCoordinates.push_back(rocksLine);
  }

  return rocksCoordinates;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  AOC::Point source{ 500, 0 };
  auto       rocksLines = ParseInput(lines);

  map<AOC::Point, char> waterfall;

  int maxY = 0;

  // mark #
  for (auto & rocks : rocksLines)
  {
    for (int i = 0; i < rocks.size() - 1; i++)
    {
      const auto & from = rocks[i];
      const auto & to   = rocks[i + 1];

      // go on y
      if (from.x == to.x)
      {
        for (int k = min(from.y, to.y); k <= max(from.y, to.y); k++)
        {
          waterfall[AOC::Point{ from.x, k }] = '#';

          maxY = max(maxY, k);
        }
      }
      // go on x
      else
      {
        for (int k = min(from.x, to.x); k <= max(from.x, to.x); k++)
        {
          waterfall[AOC::Point{ k, from.y }] = '#';

          maxY = max(maxY, (int)from.y);
        }
      }
    }
  }

  // part2
  int floorY = maxY + 2;

  bool stop = false;
  while (!stop)
  {
    auto falling = source;

    if (waterfall[falling] == 'o')
      break;

    while (!stop)
    {
      if (falling.y + 1 == floorY)
      {
        waterfall[falling] = 'o';
        break;
      }
      else if (!waterfall[falling + AOC::Point{ 0, 1 }])
      {
        falling += AOC::Point{ 0, 1 };
      }
      else if (!waterfall[falling + AOC::Point{ -1, 1 }])
      {
        falling += AOC::Point{ -1, 1 };
      }
      else if (!waterfall[falling + AOC::Point{ 1, 1 }])
      {
        falling += AOC::Point{ 1, 1 };
      }
      else
      {
        waterfall[falling] = 'o';
        break;
      }

      if (falling.y > floorY)
      {
        stop = true;
      }
    }
  }

  int sum = 0;
  for (auto [point, type] : waterfall)
  {
    if (type == 'o')
      sum++;
  }

  cout << sum;

  return 0;
}
