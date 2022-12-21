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

tuple<long long, bool, string> yell(unordered_map<string, Monkey> & graph, const string & monkey)
{
  auto & currentMonkey = graph[monkey];
  if (currentMonkey.value)
  {
    if (currentMonkey.name == "humn")
      return make_tuple(*currentMonkey.value, true, "x");

    return make_tuple(*currentMonkey.value, false, to_string(*currentMonkey.value));
  }

  assert(currentMonkey.childs.size() == 2);

  const auto & child1 = yell(graph, currentMonkey.childs[0]);
  const auto & child2 = yell(graph, currentMonkey.childs[1]);

  long long result = 0;
  string    str;

  if (currentMonkey.operation == "+")
  {
    result = get<0>(child1) + get<0>(child2);
    str    = "(" + get<2>(child1) + "+" + get<2>(child2) + ")";
  }
  else if (currentMonkey.operation == "-")
  {
    result = get<0>(child1) - get<0>(child2);
    str    = "(" + get<2>(child1) + "-" + get<2>(child2) + ")";
  }
  else if (currentMonkey.operation == "*")
  {
    result = get<0>(child1) * get<0>(child2);
    str    = "(" + get<2>(child1) + "*" + get<2>(child2) + ")";
  }
  else if (currentMonkey.operation == "/")
  {
    result = get<0>(child1) / get<0>(child2);
    str    = "(" + get<2>(child1) + "/" + get<2>(child2) + ")";
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

  return make_tuple(result, fromHumn, str);
};

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  auto graph = ParseInput(lines);
  cout << get<0>(yell(graph, "root")) << endl;

  // part 2
  graph["humn"].value = 3876907167495;

  const auto & child1 = yell(graph, graph["root"].childs[0]);
  const auto & child2 = yell(graph, graph["root"].childs[1]);

  out << get<2>(child1).c_str() << endl;
  out << get<2>(child2).c_str() << endl;

  if (get<0>(child1) == get<0>(child2))
  {
    cout << *graph["humn"].value << endl;
    return 0;
  }

  return 0;
}
