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

unordered_map<string, int>            valveRates;
unordered_map<string, vector<string>> valveChilds;
unordered_map<string, bool>           valveOpened;

vector<unordered_map<string, pair<int, int>>> memo(30, unordered_map<string, pair<int, int>>());

unordered_set<string>      valves;
unordered_map<string, int> valveBits;

void ParseInput(const vector<string> & lines)
{
  for (int i = 0; i < lines.size(); i++)
  {
    const auto & line = lines[i];

    auto matches =
      AOC::ExtractMatches(line, "Valve (.*) has flow rate=(.*); tunnels? leads? to valves? (.*)");

    assert(matches.size() == 3);

    valveRates[matches[0]]  = atoi(matches[1].c_str());
    valveChilds[matches[0]] = AOC::Explode(matches[2], ", ");

    valves.insert(matches[0]);
    for (auto & child : valveChilds[matches[0]])
      valves.insert(child);
  }

  int i = 0;
  for (auto & valve : valves)
    valveBits[valve] = i++;
}

string ComputeMemoHash(const string &                      node1,
                       const string &                      node2,
                       const unordered_map<string, bool> & valveOpened)
{
  unsigned long long hashInt = 0;
  for (const auto & [valve, opened] : valveOpened)
  {
    if (opened)
      hashInt |= (1ll << valveBits[valve]);
  }

  return node1 + node2 + to_string(hashInt);
}

optional<int> GetMemoCost(const string & node1, const string & node2, int cost, int seconds)
{
  auto it = memo[seconds].find(ComputeMemoHash(node1, node2, valveOpened));
  if (it == memo[seconds].end())
    return nullopt;

  auto [inputCost, outputCost] = (it->second);
  if (inputCost >= cost)
    return outputCost;

  return nullopt;
}

void SetMemoCost(const string & node1, const string & node2, int cost, int seconds, int memocost)
{
  auto & [inputCost, outputCost] = memo[seconds][ComputeMemoHash(node1, node2, valveOpened)];
  if (outputCost < memocost)
  {
    assert(inputCost <= cost);

    inputCost  = cost;
    outputCost = memocost;
  }
}

unordered_map<string, int> visited;

int traverse(const string & node1, const string & node2, int cost, int seconds)
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
      if (visited[child] > 10)
        continue;

      visited[child]++;

      openMoveCost =
        max(openMoveCost, traverse(node1, child, cost + valveRates[node1], seconds - 1));

      visited[child]--;
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
      if (visited[child] > 10)
        continue;

      visited[child]++;

      moveOpenCost =
        max(moveOpenCost, traverse(child, node2, cost + valveRates[node2], seconds - 1));

      visited[child]--;
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
    if (visited[child1] > 10)
      continue;

    for (const auto & child2 : valveChilds[node2])
    {
      if (visited[child1] > 10)
        continue;

      visited[child1]++;
      visited[child2]++;

      traverseTraverseCost = max(traverseTraverseCost, traverse(child1, child2, cost, seconds - 1));

      visited[child1]--;
      visited[child2]--;
    }
  }

  int totalCost = max(max(openMoveCost, moveOpenCost), max(openOpenCost, traverseTraverseCost));

  SetMemoCost(node1, node2, cost, seconds, cost + totalCost);
  return cost + totalCost;
}

// part 1
// int traverse(string node1, int cost, int seconds)
//{
//   if (!seconds)
//     return 0;
//
//   auto memoCost = GetMemoCost(node, cost, seconds);
//   if (memoCost)
//     return *memoCost;
//
//   // open
//   int openedCost = 0;
//   if (valveRates[node] && !valveOpened[node])
//   {
//     valveOpened[node] = true;
//     openedCost = traverse(node, cost + valveRates[node], seconds - 1);
//     valveOpened[node] = false;
//   }
//
//   // move to another value
//   vector<int> traverseCosts;
//   for (auto child : valveChilds[node])
//   {
//     traverseCosts.push_back(traverse(child, cost, seconds - 1));
//   }
//
//   sort(begin(traverseCosts), end(traverseCosts), greater<int>());
//
//   // chose two actions
//   int optimalCost = max(openedCost, traverseCosts[0]);
//   // if (traverseCosts.size() > 1)
//   //{
//   //   optimalCost = max(optimalCost, traverseCosts[0] + traverseCosts[1]);
//   // }
//
//   SetMemoCost(node, cost, seconds, cost + optimalCost);
//   return cost + optimalCost;
// }

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  ParseInput(lines);
  cout << traverse("AA", "AA", 0, 26);

  return 0;
}
