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

struct Rock
{
  void TranslateToPosition(AOC::Point position)
  {
    for (auto & point : points)
      point += position;
  }

  void MoveDown()
  {
    for (auto & point : points)
      point.x++;
  }

  void Move(char direction)
  {
    for (auto & point : points)
    {
      if (direction == '<')
        point.y--;
      else if (direction == '>')
        point.y++;
      else
      {
        assert(false);
      }
    }
  }

  int                height{};
  int                width{};
  vector<AOC::Point> points;
};

Rock rockMinus = { 1,
                   4,
                   {

                     { 0, 0 },
                     { 0, 1 },
                     { 0, 2 },
                     { 0, 3 },
                   } };

Rock rockPlus = { 3,
                  3,
                  {
                    { 0, 1 },
                    { 1, 0 },
                    { 1, 1 },
                    { 1, 2 },
                    { 2, 1 },
                  } };

Rock rockL = { 3,
               3,
               {
                 { 0, 2 },
                 { 1, 2 },
                 { 2, 0 },
                 { 2, 1 },
                 { 2, 2 },
               } };

Rock rockMinusVertical = { 4,
                           1,
                           {
                             { 0, 0 },
                             { 1, 0 },
                             { 2, 0 },
                             { 3, 0 },
                           } };

Rock rockCube = { 2,
                  2,
                  {
                    { 0, 0 },
                    { 0, 1 },
                    { 1, 0 },
                    { 1, 1 },
                  } };

vector<Rock> rockPatterns{ rockMinus, rockPlus, rockL, rockMinusVertical, rockCube };

struct JetPattern
{
  JetPattern(const string & pattern)
    : pattern(pattern)
  {
  }

  char GetNextDirection()
  {
    pos = (pos + 1) % pattern.size();
    return pattern[pos];
  };

  int    pos{ -1 };
  string pattern;
};

struct RockGenerator
{
  Rock GetNextRock()
  {
    pos = (pos + 1) % rockPatterns.size();
    return rockPatterns[pos];
  }

  int pos{ -1 };
};

struct Chamber
{
  AOC::Point GetApearPosition(const Rock & rock)
  {
    AOC::Point nextRockPosition;
    nextRockPosition.x = height - rock.height - 3;
    nextRockPosition.y += 3;

    return nextRockPosition;
  }

  bool CanMove(const Rock & rock)
  {
    for (const auto & rockPoint : rock.points)
    {
      if (rockPoint.x >= 0)
        return false;

      if (rockPoint.y <= 0)
        return false;

      if (rockPoint.y >= 8)
        return false;

      if (points.find(rockPoint) != points.end())
        return false;
    }

    return true;
  }

  void Settle(const Rock & rock)
  {
    for (const auto & rockPoint : rock.points)
    {
      assert(points.find(rockPoint) == points.end());
      points[rockPoint] = '#';

      height = min(height, rockPoint.x);
    }
  }

  long long             height{ 0 };
  map<AOC::Point, char> points;
};

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();
  assert(lines.size() == 1);

  Chamber       chamber;
  RockGenerator rockGenerator;
  JetPattern    pattern(lines[0]);

  int previousheight = 0;

  auto runFor = [&](int count)
  {
    for (int i = 1; i <= count; i++)
    {
      auto rock = rockGenerator.GetNextRock();
      rock.TranslateToPosition(chamber.GetApearPosition(rock));

      while (true)
      {
        auto nextRock = rock;
        nextRock.Move(pattern.GetNextDirection());

        if (chamber.CanMove(nextRock))
          rock = nextRock;

        nextRock = rock;
        nextRock.MoveDown();

        if (chamber.CanMove(nextRock))
        {
          rock = nextRock;
          continue;
        }
        else
        {
          chamber.Settle(rock);
          break;
        }
      }
    }
  };

  runFor(43000);  // 67554
  out << AOC::Abs(chamber.height) << endl;

  runFor(69000);  // 175914
  out << AOC::Abs(chamber.height) << endl;
  assert(AOC::Abs(chamber.height) == 67554 + 1ll * 108360);

  runFor(69000);  // 284274
  assert(AOC::Abs(chamber.height) == 67554 + 2ll * 108360);

  // 43000 + 14492753 * 69000 = 1000000000000
  cout << 67554 + (14492753ll * 108360) << endl;

  return 0;
}
