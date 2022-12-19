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

enum MineralType
{
  ore,
  clay,
  obsidian,
  geode
};

struct RobotCost
{
  int ore{};
  int clay{};
  int obsidian{};

  vector<pair<MineralType, int>> getCosts()
  {
    vector<pair<MineralType, int>> cost;
    cost.push_back({ MineralType::ore, ore });
    cost.push_back({ MineralType::clay, clay });
    cost.push_back({ MineralType::obsidian, obsidian });

    return cost;
  }
};

struct Blueprint
{
  int       id{};
  RobotCost oreRobot;
  RobotCost clayRobot;
  RobotCost obsidianRobot;
  RobotCost geodeRobot;
};

struct Robot
{
  int                            count;
  MineralType                    type;
  vector<pair<MineralType, int>> price;
};

vector<Blueprint> ParseInput(const vector<string> & lines)
{
  vector<Blueprint> blueprints;

  for (int i = 0; i < lines.size(); i++)
  {
    const auto & line = lines[i];

    auto matches = AOC::ExtractMatches(
      line,
      "Blueprint (.*): Each ore robot costs (.*) ore. Each clay robot costs (.*) ore. Each "
      "obsidian "
      "robot costs (.*) ore and (.*) clay. Each geode robot costs (.*) ore and (.*) obsidian.");

    assert(matches.size() == 7);

    Blueprint blueprint;
    blueprint.id            = atoi(matches[0].c_str());
    blueprint.oreRobot      = RobotCost{ atoi(matches[1].c_str()), 0, 0 };
    blueprint.clayRobot     = RobotCost{ atoi(matches[2].c_str()), 0, 0 };
    blueprint.obsidianRobot = RobotCost{ atoi(matches[3].c_str()), atoi(matches[4].c_str()), 0 };
    blueprint.geodeRobot    = RobotCost{ atoi(matches[5].c_str()), 0, atoi(matches[6].c_str()) };

    blueprints.push_back(blueprint);
  }

  return blueprints;
}

using Hash = tuple<int, int, int, int, int, int>;
using Memo = vector<map<Hash, int>>;

Hash ComputeMemoHash(const vector<Robot> & robots, const vector<int> & minerals)
{
  return tie(robots[0].count, robots[1].count, robots[2].count, minerals[0], minerals[1],
             minerals[2]);
}

optional<int> GetMemoCost(Memo &                memo,
                          const vector<Robot> & robots,
                          const vector<int> &   minerals,
                          int                   minute)
{
  auto hash = ComputeMemoHash(robots, minerals);

  auto it = memo[minute].find(hash);
  if (it != memo[minute].end())
    return (*it).second;

  return nullopt;
}

void SetMemoCost(
  Memo & memo, const vector<Robot> & robots, const vector<int> & minerals, int minute, int cost)
{
  auto hash          = ComputeMemoHash(robots, minerals);
  memo[minute][hash] = cost;
}

int collectMinerals(Memo & memo, vector<Robot> & robots, vector<int> & minerals, int minute)
{
  if (minute == 0)
    return minerals[MineralType::geode];

  auto memoCost = GetMemoCost(memo, robots, minerals, minute);
  if (memoCost)
    return *memoCost;

  bool continueWithoutBuilding = true;

  // build a new robot
  int maxGeode = 0;
  for (int i = (int)robots.size() - 1; i >= 0; i--)
  {
    auto & robot    = robots[i];
    bool   wasBuild = true;

    // start building
    for (auto & [mineralType, cost] : robot.price)
    {
      minerals[mineralType] -= cost;
      if (minerals[mineralType] < 0)
        wasBuild = false;
    }

    if (wasBuild)
    {
      // comment for part 1
      continueWithoutBuilding = false;

      // collect minerals
      for (const auto & robot : robots)
      {
        minerals[robot.type] += robot.count;
      }

      // add robot
      robot.count++;

      // add new robot
      maxGeode = max(maxGeode, collectMinerals(memo, robots, minerals, minute - 1));

      // reverse robot
      robot.count--;

      // reverse minerals
      for (const auto & robot : robots)
      {
        minerals[robot.type] -= robot.count;
      }
    }

    // reverse building
    for (auto & [mineralType, cost] : robot.price)
    {
      minerals[mineralType] += cost;
    }
  }

  // without building a robot
  if (continueWithoutBuilding)
  {
    for (const auto & robot : robots)
    {
      minerals[robot.type] += robot.count;
    }

    maxGeode = max(maxGeode, collectMinerals(memo, robots, minerals, minute - 1));

    // reverse minerals
    for (const auto & robot : robots)
    {
      minerals[robot.type] -= robot.count;
    }
  }

  SetMemoCost(memo, robots, minerals, minute, maxGeode);
  return maxGeode;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  int       sum     = 0;
  long long product = 1;

  auto blueprints = ParseInput(lines);
  for (auto & blueprint : blueprints)
  {
    Memo memo(35);

    vector<Robot> robots;
    robots.push_back(Robot{ 1, MineralType::ore, blueprint.oreRobot.getCosts() });
    robots.push_back(Robot{ 0, MineralType::clay, blueprint.clayRobot.getCosts() });
    robots.push_back(Robot{ 0, MineralType::obsidian, blueprint.obsidianRobot.getCosts() });
    robots.push_back(Robot{ 0, MineralType::geode, blueprint.geodeRobot.getCosts() });

    vector<int> minerals(5);

    // part1
    // const auto deep = 24;

    // part2
    const auto deep = 32;

    auto geode = collectMinerals(memo, robots, minerals, deep);

    cout << blueprint.id << "*" << geode << "=" << blueprint.id * geode << endl;
    sum += blueprint.id * geode;
    product *= geode;
  }

  cout << endl;
  cout << endl;
  cout << sum;
  cout << endl;
  cout << product;

  return 0;
}
