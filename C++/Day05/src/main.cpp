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

void invertStacks(vector<stack<char>> & stacks)
{
  for (int i = 0; i < stacks.size(); i++)
  {
    stack<char> newStack;
    while (!stacks[i].empty())
    {
      newStack.push(stacks[i].top());
      stacks[i].pop();
    }

    stacks[i] = newStack;
  }
}

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader  reader(in);
  vector<string> v = reader.ReadLines();

  constexpr int       stacksSize = 9;
  vector<stack<char>> stacks(stacksSize, stack<char>());

  // part 2
  int i = 0;
  for (; i < v.size(); i++)
  {
    auto & line = v[i];
    if (line[1] == '1')
      break;

    for (int j = 0; j < 9; j++)
    {
      int pos = 1 + 4 * j;
      if (pos >= line.size())
        continue;

      assert('A' <= line[pos] && line[pos] <= 'Z' || line[pos] == ' ');

      if (line[pos] != ' ')
        stacks[j].push(line[pos]);
    }
  }

  invertStacks(stacks);

  // for space
  i += 2;

  for (; i < v.size(); i++)
  {
    auto & line = v[i];

    auto matches = AOC::ExtractMatches(line, "move (.*) from (.*) to (.*)");

    int count = stoi(matches[0]);
    int from  = stoi(matches[1]) - 1;
    int to    = stoi(matches[2]) - 1;

    // part 1
    /*   while (count--)
       {
         assert(!stacks[from].empty());

         stacks[to].push(stacks[from].top());
         stacks[from].pop();
       }*/

    // part 2
    stack<char> newStack;
    while (count--)
    {
      assert(!stacks[from].empty());

      newStack.push(stacks[from].top());
      stacks[from].pop();
    }

    while (!newStack.empty())
    {
      stacks[to].push(newStack.top());
      newStack.pop();
    }
  }

  for (int k = 0; k < stacksSize; k++)
  {
    cout << stacks[k].top();
  }

  return 0;
}
