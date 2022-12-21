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

struct Monkey
{
  string              name;
  optional<long long> value;
  string              operation;
  vector<string>      childs;
};

unordered_map<string, Monkey> ParseInput(const vector<string> & lines)
{
  unordered_map<string, Monkey> graph;

  for (auto & line : lines)
  {
    auto matches = AOC::ExtractMatches(line, "(.*): (.*) (.*) (.*)");
    if (matches.size())
    {
      Monkey monkey;
      monkey.name      = matches[0];
      monkey.operation = matches[2];
      monkey.childs    = vector<string>{ matches[1], matches[3] };

      graph[matches[0]] = monkey;
    }
    else
    {
      matches = AOC::ExtractMatches(line, "(.*): (.*)");

      Monkey monkey;
      monkey.name  = matches[0];
      monkey.value = stoi(matches[1].c_str());

      graph[matches[0]] = monkey;
    }
  }

  return graph;
}

tuple<long long, bool> yell(unordered_map<string, Monkey> & graph, const string & monkey)
{
  auto & currentMonkey = graph[monkey];
  if (currentMonkey.value)
  {
    if (currentMonkey.name == "humn")
      return make_tuple(*currentMonkey.value, true);

    return make_tuple(*currentMonkey.value, false);
  }

  assert(currentMonkey.childs.size() == 2);

  const auto & child1 = yell(graph, currentMonkey.childs[0]);
  const auto & child2 = yell(graph, currentMonkey.childs[1]);

  long long result = 0;
  string    str;

  if (currentMonkey.operation == "+")
  {
    result = get<0>(child1) + get<0>(child2);
  }
  else if (currentMonkey.operation == "-")
  {
    result = get<0>(child1) - get<0>(child2);
  }
  else if (currentMonkey.operation == "*")
  {
    result = get<0>(child1) * get<0>(child2);
  }
  else if (currentMonkey.operation == "/")
  {
    result = get<0>(child1) / get<0>(child2);
  }
  else
  {
    assert(false);
  }

  bool fromHumn = get<1>(child1) || get<1>(child2);
  if (!fromHumn)
  {
    currentMonkey.value = result;
  }

  return make_tuple(result, fromHumn);
};

long long solveEquation(optional<long long> a,
                        const string &      operation,
                        optional<long long> b,
                        long long           result)
{
  assert(a.has_value() ^ b.has_value());

  if (operation == "+")
  {
    if (!a)
      return result - *b;
    else
      return result - *a;
  }
  else if (operation == "*")
  {
    if (!a)
      return result / *b;
    else
      return result / *a;
  }
  else if (operation == "/")
  {
    if (!a)
      return result * *b;
    else
      return *a / result;
  }
  else if (operation == "-")
  {
    if (!a)
      return result + *b;
    else
      return -result + *a;
  }

  assert(false);
  return 0;
};

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  auto graph = ParseInput(lines);

  // part 1
  cout << get<0>(yell(graph, "root")) << endl;

  // part 2
  auto getChildMonkeys = [&](const string & node) -> pair<Monkey *, Monkey *>
  {
    return { &graph[graph[node].childs[0]], &graph[graph[node].childs[1]] };
  };

  string node                = "root"s;
  auto [rootLeft, rootRight] = getChildMonkeys(node);

  long long humn      = 2 * rootLeft->value.value_or(*rootRight->value);
  graph["humn"].value = nullopt;

  while (node != "humn")
  {
    auto [left, right] = getChildMonkeys(node);

    humn = solveEquation(left->value, graph[node].operation, right->value, humn);
    node = (!left->value.has_value() ? left->name : right->name);
  }

  cout << humn << endl;

  return 0;
}
