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
    : data(200, vector<char>(200, ' ')){};

  vector<vector<char>> data;
};

AOC::Point GetInitialPosition(const MonkeyMap & monkyeMap)
{
  for (int i = 0; i < monkyeMap.data[0].size(); i++)
  {
    if (monkyeMap.data[0][i] == '.')
    {
      return { 0, i };
    }
  }

  assert(false);
  return { 0, 0 };
}

optional<AOC::Point> GetNextPosition(const MonkeyMap & monkyeMap, AOC::Ship & robot, int distance)
{
  auto moveOne = [&](AOC::Point point, char direction) -> AOC::Point
  {
    if (direction == '>')
      point.y = (point.y + 1) % monkyeMap.data[0].size();
    else if (direction == '<')
      point.y = (monkyeMap.data[0].size() + point.y - 1) % monkyeMap.data[0].size();
    else if (direction == 'v')
      point.x = (point.x + 1) % monkyeMap.data.size();
    else if (direction == '^')
      point.x = (monkyeMap.data.size() + point.x - 1) % monkyeMap.data.size();

    return point;
  };

  auto nextPosition = robot.GetPosition();
  auto candidate    = nextPosition;

  while (distance)
  {
    candidate = moveOne(candidate, robot.GetArrowDirection());
    if (monkyeMap.data[candidate.x][candidate.y] == '.')
    {
      distance--;
      nextPosition = candidate;
    }
    else if (monkyeMap.data[candidate.x][candidate.y] == ' ')
      continue;
    else if (monkyeMap.data[candidate.x][candidate.y] == '#')
      break;
    else
      assert(false);
  }

  if (robot.GetPosition() != nextPosition)
    return nextPosition;

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
        robot.SetPosition(*nextPosition);

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
    robot.SetPosition(*nextPosition);

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

  cout << (robot.GetPosition().x + 1) * 1000 + (robot.GetPosition().y + 1) * 4 +
            GetDirectionCost(robot.GetArrowDirection());

  return 0;
}
