/**
 * Advent of code 2022
 * @author : Nicolae Telechi
 */
#include <algorithm>
#include <assert.h>
#include <bitset>
#include <deque>
#include <fstream>
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

struct MonkeyMap
{
  MonkeyMap()
    : data(200, string(200, ' ')){};

  vector<string> data;
};

AOC::Point GetInitialPosition(const MonkeyMap & monkyeMap)
{
  return { 0, 50 };
}

pair<AOC::Point, char> teleport(const MonkeyMap & monkyeMap, AOC::Point & from, char direction)
{
  // teleport
  if (direction == '>')
  {
    if (from.x < 50)
    {
      auto diff = from.x;
      return { { 149 - diff, 99 }, '<' };
    }
    else if (from.x < 100)
    {
      auto diff = from.x - 50;
      return { { 49, 100 + diff }, '^' };
    }
    else if (from.x < 150)
    {
      auto diff = from.x - 100;
      return { { 49 - diff, 149 }, '<' };
    }
    else if (from.x < 200)
    {
      auto diff = from.x - 150;
      return { { 0, 149 - diff }, 'v' };
    }
    else
    {
      assert(false);
    }
  }
  else if (direction == '<')
  {
    if (from.x < 50)
    {
      auto diff = from.x;
      return { { 149 - diff, 50 }, '>' };
    }
    else if (from.x < 100)
    {
      auto diff = from.x - 50;
      return { { 49, 49 - diff }, '^' };
    }
    else if (from.x < 150)
    {
      auto diff = from.x - 100;
      return { { 49 - diff, 0 }, '>' };
    }
    else if (from.x < 200)
    {
      auto diff = from.x - 150;
      return { { 0, diff }, 'v' };
    }
    else
    {
      assert(false);
    }
  }
  else if (direction == '^')
  {
    if (from.y < 50)
    {
      auto diff = from.y;
      return { { 150 + diff, 50 }, '>' };
    }
    else if (from.y >= 100)
    {
      auto diff = from.y - 100;
      return { { 199 - diff, 99 }, '<' };
    }
    else
    {
      assert(false);
    }
  }
  else if (direction == 'v')
  {
    if (from.y < 50)
    {
      auto diff = from.y;
      return { { 99 - diff, 50 }, '>' };
    }
    else if (from.y >= 100)
    {
      auto diff = from.y - 100;
      return { { 50 + diff, 99 }, '<' };
    }
    else
    {
      assert(false);
    }
  }
}

optional<pair<AOC::Point, char>> GetNextPosition(const MonkeyMap & monkyeMap,
                                                 AOC::Ship &       robot,
                                                 int               distance)
{
  auto moveOne = [&](AOC::Point point, char direction) -> AOC::Point
  {
    if (direction == '>')
      point.y = (point.y + 1);
    else if (direction == '<')
      point.y = (point.y - 1);
    else if (direction == 'v')
      point.x = (point.x + 1) % monkyeMap.data.size();
    else if (direction == '^')
      point.x = (monkyeMap.data.size() + point.x - 1) % monkyeMap.data.size();

    return point;
  };

  auto nextPosition  = robot.GetPosition();
  auto nextDirection = robot.GetArrowDirection();

  auto candidate          = nextPosition;
  auto candidateDirection = nextDirection;

  while (distance)
  {
    candidate = moveOne(candidate, candidateDirection);

    // teleport
    if (candidate.y < 0 || candidate.y >= monkyeMap.data[0].size() ||
        monkyeMap.data[candidate.x][candidate.y] == ' ')
    {
      auto to = teleport(monkyeMap, candidate, candidateDirection);

      candidate          = to.first;
      candidateDirection = to.second;

      assert(monkyeMap.data[candidate.x][candidate.y] != ' ');
    }

    if (monkyeMap.data[candidate.x][candidate.y] == '.')
    {
      distance--;

      nextPosition  = candidate;
      nextDirection = candidateDirection;
    }
    else if (monkyeMap.data[candidate.x][candidate.y] == '#')
      break;
    else
      assert(false);
  }

  if (robot.GetPosition() != nextPosition)
    return make_pair(nextPosition, nextDirection);

  return nullopt;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  MonkeyMap monkyeMap;

  int i = 0;
  for (; !lines[i].empty(); i++)
  {
    auto & line = lines[i];
    copy(begin(line), end(line), monkyeMap.data[i].begin());
  }

  const auto & tiles = lines[i + 1];

  AOC::Ship robot;
  robot.SetPosition(GetInitialPosition(monkyeMap));
  robot.SetArrowDirection('>');

  int distance = 0;
  for (auto c : tiles)
  {
    if (c == 'R' || c == 'L')
    {
      auto nextPosition = GetNextPosition(monkyeMap, robot, distance);
      if (nextPosition)
      {
        robot.SetPosition(nextPosition->first);
        robot.SetArrowDirection(nextPosition->second);
      }

      if (c == 'L')
        robot.TurnLeft();
      else if (c == 'R')
        robot.TurnRight();

      distance = 0;
    }
    else
    {
      distance = distance * 10 + (c - '0');
    }
  }

  auto nextPosition = GetNextPosition(monkyeMap, robot, distance);
  if (nextPosition)
  {
    robot.SetPosition(nextPosition->first);
    robot.SetArrowDirection(nextPosition->second);
  }

  auto GetDirectionCost = [&](char direction)
  {
    switch (direction)
    {
    case '>':
      return 0;
    case 'v':
      return 1;
    case '<':
      return 2;
    case '^':
      return 3;
    default:
      break;
    }

    assert(false);
    return 0;
  };

  cout << robot.GetPosition().x + 1 << " " << robot.GetPosition().y + 1 << endl;

  assert(monkyeMap.data[robot.GetPosition().x][robot.GetPosition().y] == '.');
  monkyeMap.data[robot.GetPosition().x][robot.GetPosition().y] = robot.GetArrowDirection();

  cout << (robot.GetPosition().x + 1) * 1000 + (robot.GetPosition().y + 50 + 1) * 4 +
            GetDirectionCost(robot.GetArrowDirection());

  FStreamWriter writter(out);
  // writter.WriteLines(monkyeMap.data);

  // after translating back to original imput positions are
  // 127 * 1000 + 4*3 + 0 = 127012

  return 0;
}
