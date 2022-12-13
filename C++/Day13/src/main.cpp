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

struct Node
{
  bool                     isDividerPacket{ false };
  optional<int>            value;
  vector<unique_ptr<Node>> childs;
};

unique_ptr<Node> ParseList(string_view & str)
{
  auto node = make_unique<Node>();

  if (str._Starts_with("["))
  {
    str.remove_prefix(1);

    do
    {
      node->childs.push_back(ParseList(str));
      if (str._Starts_with(","))
        str.remove_prefix(1);

    } while (str.size() && str.front() != ']');

    assert(str.front() == ']');
    str.remove_prefix(1);
  }
  else if (str.size() && str.front() != ']')
  {
    int value = 0;
    while ('0' <= str.front() && str.front() <= '9')
    {
      value = value * 10 + (str.front() - '0');
      str.remove_prefix(1);
    }

    node->value = value;
  }

  return node;
}

enum class CompareResult
{
  less    = 1,
  equal   = 0,
  greater = -1,
};

CompareResult CompareNodes(Node * root1, Node * root2)
{
  // both values
  if (root1->value && root2->value)
  {
    if (*(root1->value) < *(root2->value))
      return CompareResult::less;
    else if (*(root1->value) > *(root2->value))
      return CompareResult::greater;
    else
      return CompareResult::equal;
  }
  // both lists
  else if (!root1->value && !root2->value)
  {
    for (int i = 0; i < root1->childs.size(); i++)
    {
      // right runs out
      if (i >= root2->childs.size())
        return CompareResult::greater;

      auto compare = CompareNodes(root1->childs[i].get(), root2->childs[i].get());
      if (compare != CompareResult::equal)
        return compare;
    }

    // left runs out
    if (root1->childs.size() < root2->childs.size())
      return CompareResult::less;

    return CompareResult::equal;
  }
  // list, value
  else if (!root1->value && root2->value)
  {
    if (root1->childs.empty())
      return CompareResult::less;

    auto compare = CompareNodes(root1->childs.front().get(), root2);
    if (compare == CompareResult::equal && root1->childs.size() > 1)
      return CompareResult::greater;

    return compare;
  }
  // value, list
  else if (root1->value && !root2->value)
  {
    if (root2->childs.empty())
      return CompareResult::greater;

    auto compare = CompareNodes(root1, root2->childs.front().get());
    if (compare == CompareResult::equal && root2->childs.size() > 1)
      return CompareResult::less;

    return compare;
  }

  assert(false);
  return CompareResult::less;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  vector<unique_ptr<Node>> nodes;

  int sum = 0;

  int pairIndex = 1;
  for (int i = 0; i < lines.size(); i += 3, pairIndex += 1)
  {
    auto & line1 = lines[i];
    auto & line2 = lines[i + 1];

    assert(!line1.empty() && !line2.empty());

    string_view str1 = line1;
    string_view str2 = line2;

    auto root1 = ParseList(str1);
    auto root2 = ParseList(str2);

    auto compare = CompareNodes(root1.get(), root2.get());
    if (compare == CompareResult::less)
    {
      cout << pairIndex << " ";
      sum += pairIndex;
    }

    assert(compare != CompareResult::equal);

    nodes.push_back(move(root1));
    nodes.push_back(move(root2));
  }

  auto divider2             = make_unique<Node>();
  divider2->isDividerPacket = true;
  divider2->childs.push_back(make_unique<Node>(Node{ true, 2, {} }));

  auto divider6             = make_unique<Node>();
  divider6->isDividerPacket = true;
  divider6->childs.push_back(make_unique<Node>(Node{ true, 6, {} }));

  nodes.push_back(move(divider2));
  nodes.push_back(move(divider6));

  sort(begin(nodes), end(nodes),
       [](const unique_ptr<Node> & root1, const unique_ptr<Node> & root2)
       {
         return CompareResult::less == CompareNodes(root1.get(), root2.get());
       });

  int multiply = 1;
  for (int i = 1; i < nodes.size(); i++)
  {
    if (nodes[i]->isDividerPacket)
      multiply *= (i + 1);
  }

  cout << endl << endl;
  cout << "Part1 : " << sum << endl;
  cout << "Part2 : " << multiply;

  return 0;
}
