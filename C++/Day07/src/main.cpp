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

constexpr long long kSystemSize = 70000000;
constexpr long long kUnusedSize = 30000000;

struct Node
{
  bool                                    isDir{ true };
  string                                  name;
  optional<long long>                     size;
  Node *                                  parent{};
  unordered_map<string, unique_ptr<Node>> childs;
};

long long computeDirSize(Node * root)
{
  if (root->size)
    return *root->size;

  long long s = 0;
  for (auto & [_, child] : root->childs)
  {
    if (child->isDir)
    {
      s += computeDirSize(child.get());
    }
    else
    {
      assert(child->size);
      s += *child->size;
    }
  }

  root->size = s;
  return s;
}

long long globalTotalSizePart1 = 0;

void computeTotalSizePart1(Node * root)
{
  for (auto & [_, child] : root->childs)
  {
    if (child->isDir)
    {
      auto directoryTotalSize = computeDirSize(child.get());
      if (directoryTotalSize <= 100000)
      {
        globalTotalSizePart1 += directoryTotalSize;
      }

      computeTotalSizePart1(child.get());
    }
  }
}

Node *         globalRoot{};
vector<Node *> globalMatchingDirs;

void findSmallestPart2(Node * root)
{
  for (auto & [_, child] : root->childs)
  {
    if (child->isDir)
    {
      auto directoryTotalSize = computeDirSize(child.get());
      if (*globalRoot->size - directoryTotalSize <= kSystemSize - kUnusedSize)
      {
        globalMatchingDirs.push_back(child.get());
      }

      findSmallestPart2(child.get());
    }
  }
}

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader  reader(in);
  vector<string> v = reader.ReadLines();

  // part 2
  auto & line = v[0];

  Node root;
  root.isDir = true;
  root.name  = "/";

  Node * currentNode = &root;

  for (auto & line : v)
  {
    auto matches = AOC::ExtractMatches(line, "(.*) (.*)");
    if (matches[0] == "$ cd")
    {
      if (matches[1] == "/")
      {
        currentNode = &root;
      }
      else if (matches[1] == "..")
      {
        currentNode = currentNode->parent;
      }
      else
      {
        currentNode = currentNode->childs[matches[1]].get();
      }
    }
    else if (matches[0] == "$" && matches[1] == "ls")
    {
    }
    else if (matches[0] == "dir")
    {
      if (!currentNode->childs[matches[1]])
      {
        auto node    = make_unique<Node>();
        node->isDir  = true;
        node->name   = matches[1];
        node->parent = currentNode;

        currentNode->childs[matches[1]] = move(node);
      }
    }
    // file and size
    else
    {
      if (!currentNode->childs[matches[1]])
      {
        auto node    = make_unique<Node>();
        node->isDir  = false;
        node->size   = stoi(matches[0]);
        node->name   = matches[1];
        node->parent = currentNode;

        currentNode->childs[matches[1]] = move(node);
      }
    }
  }

  computeDirSize(&root);
  computeTotalSizePart1(&root);

  cout << globalTotalSizePart1 << endl;

  globalRoot = &root;
  findSmallestPart2(&root);

  auto minDir = min_element(begin(globalMatchingDirs), end(globalMatchingDirs),
                            [](Node * first, Node * second)
                            {
                              return *first->size < *second->size;
                            });

  assert(minDir != globalMatchingDirs.end());
  cout << *(*minDir)->size << endl;

  return 0;
}
