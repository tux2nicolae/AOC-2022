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

unordered_map<string, int> valveNameToId;
vector<int>                valveRates(100);
vector<vector<long long>>  valveChilds(100);

bitset<64> valveOpened;

vector<unordered_map<unsigned long long, pair<int, int>>> memo(
  30, unordered_map<unsigned long long, pair<int, int>>());

void ParseInput(const vector<string> & lines)
{
  unordered_map<string, int>            valveRatesStr;
  unordered_map<string, vector<string>> valveChildsStr;

  unordered_set<string> existingValves;
  for (int i = 0; i < lines.size(); i++)
  {
    const auto & line = lines[i];

    auto matches =
      AOC::ExtractMatches(line, "Valve (.*) has flow rate=(.*); tunnels? leads? to valves? (.*)");

    assert(matches.size() == 3);

    valveRatesStr[matches[0]]  = atoi(matches[1].c_str());
    valveChildsStr[matches[0]] = AOC::Explode(matches[2], ", ");

    existingValves.insert(matches[0]);
    for (auto & child : valveChildsStr[matches[0]])
      existingValves.insert(child);
  }

  int i = 0;
  for (auto & valve : existingValves)
    valveNameToId[valve] = i++;

  for (const auto & valve : valveRatesStr)
  {
    valveRates[valveNameToId[valve.first]] = valve.second;

    for (const auto & childStr : valveChildsStr[valve.first])
    {
      valveChilds[valveNameToId[valve.first]].push_back(valveNameToId[childStr]);
    }
  }
}

unsigned long long ComputeMemoHash(const long long    node1,
                                   const long long    node2,
                                   const bitset<64> & valveOpened)
{
  auto hashNode1 = node1;
  auto hashNode2 = node2;

  unsigned long long hashOpenedValves = valveOpened.to_ullong();
  return (hashNode1 * valveNameToId.size() + hashNode2) * (1ll << valveNameToId.size()) +
         hashOpenedValves;
}

optional<int> GetMemoCost(const long long node1,
                          const long long node2,
                          int             currentCost,
                          int             seconds)
{
  auto hash = ComputeMemoHash(node1, node2, valveOpened);

  auto it = memo[seconds].find(hash);
  if (it != memo[seconds].end())
    return (*it).second.second;

  return nullopt;
}

void SetMemoCost(long long node1, long long node2, int currentCost, int seconds, int memocost)
{
  auto hash = ComputeMemoHash(node1, node2, valveOpened);

  auto & [inputCostPlus, outputCostPlus] = memo[seconds][hash];
  if (outputCostPlus < memocost)
  {
    assert(inputCostPlus <= currentCost);

    inputCostPlus  = currentCost;
    outputCostPlus = memocost;
  }
}

int traverse(const long long node1, const long long node2, int cost, int seconds)
{
  if (!seconds)
    return 0;

  auto memoCost = GetMemoCost(node1, node2, cost, seconds);
  if (memoCost)
    return *memoCost;

  // first opens the valve -> second moves
  int openMoveCost = 0;
  if (valveRates[node1] && !valveOpened[node1])
  {
    valveOpened[node1] = true;

    for (const auto & child : valveChilds[node2])
    {
      openMoveCost =
        max(openMoveCost, traverse(node1, child, cost + valveRates[node1], seconds - 1));
    }

    valveOpened[node1] = false;
  }

  // first moves -> second opens the valve
  int moveOpenCost = 0;
  if (valveRates[node2] && !valveOpened[node2])
  {
    valveOpened[node2] = true;

    for (const auto & child : valveChilds[node1])
    {
      moveOpenCost =
        max(moveOpenCost, traverse(child, node2, cost + valveRates[node2], seconds - 1));
    }

    valveOpened[node2] = false;
  }

  // first opens the valve -> second opens the valve
  int openOpenCost = 0;
  if (node1 != node2 && (valveRates[node1] && !valveOpened[node1]) &&
      (valveRates[node2] && !valveOpened[node2]))
  {
    valveOpened[node1] = true;
    valveOpened[node2] = true;

    openOpenCost =
      traverse(node1, node2, cost + valveRates[node1] + valveRates[node2], seconds - 1);

    valveOpened[node1] = false;
    valveOpened[node2] = false;
  }

  // first traverse -> secod traverse
  int traverseTraverseCost = 0;
  for (const auto & child1 : valveChilds[node1])
  {
    for (const auto & child2 : valveChilds[node2])
    {
      traverseTraverseCost = max(traverseTraverseCost, traverse(child1, child2, cost, seconds - 1));
    }
  }

  int totalCost = max(max(openMoveCost, moveOpenCost), max(openOpenCost, traverseTraverseCost));

  SetMemoCost(node1, node2, cost, seconds, cost + totalCost);
  return cost + totalCost;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  ParseInput(lines);
  cout << traverse(valveNameToId["AA"], valveNameToId["AA"], 0, 26);

  return 0;
}
