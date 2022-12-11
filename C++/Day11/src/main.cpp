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

struct Monkey
{
  string    operation{};
  long long operationNumber{};

  long long testNumber{};
  long long testTrue{};
  long long testFalse{};

  vector<long long> items;

  long long inspectedItemsTimes{};
};

vector<Monkey> ParseMonkeys(const vector<string> & v)
{
  Monkey         currentMonkey;
  vector<Monkey> monkeys;

  for (auto & line : v)
  {
    if (line.empty())
    {
      monkeys.push_back(currentMonkey);
      currentMonkey = Monkey();
      continue;
    }

    if (line._Starts_with("Monkey"))
    {
      continue;
    }
    else if (line._Starts_with("  Starting items:"))
    {
      auto matches = AOC::ExtractMatches(line, "  Starting items: (.*)");
      assert(matches.size() == 1);

      vector<string> startingItems = AOC::Explode(matches[0], ", ");
      for (const auto & startingItem : startingItems)
      {
        currentMonkey.items.push_back(atoi(startingItem.c_str()));
      }
    }
    else if (line._Starts_with("  Operation:"))
    {
      auto matches = AOC::ExtractMatches(line, "  Operation: new = old (.*) (.*)");
      assert(matches.size() == 2);
      assert(matches[0].size() == 1);

      currentMonkey.operation = matches[0][0];

      if (matches[1] == "old")
      {
        currentMonkey.operation += "old";
      }
      else
      {
        currentMonkey.operationNumber = stoi(matches[1].c_str());
      }
    }
    else if (line._Starts_with("  Test:"))
    {
      auto matches = AOC::ExtractMatches(line, "  Test: divisible by (.*)");
      assert(matches.size() == 1);

      currentMonkey.testNumber = stoi(matches[0].c_str());
    }
    else if (line._Starts_with("    If true:"))
    {
      auto matches = AOC::ExtractMatches(line, "    If true: throw to monkey (.*)");
      assert(matches.size() == 1);

      currentMonkey.testTrue = stoi(matches[0].c_str());
    }
    else if (line._Starts_with("    If false:"))
    {
      auto matches = AOC::ExtractMatches(line, "    If false: throw to monkey (.*)");
      assert(matches.size() == 1);

      currentMonkey.testFalse = stoi(matches[0].c_str());
    }
  }

  monkeys.push_back(currentMonkey);
  return monkeys;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          v = reader.ReadLines();

  FStreamWriter writter(out);

  vector<Monkey> monkeys = ParseMonkeys(v);

  long long cmmmc = 1;
  for (const auto & monkey : monkeys)
  {
    if (monkey.testNumber)
      cmmmc = AOC::Cmmmc(cmmmc, monkey.testNumber);
  }

  // part 1
  // constexpr int kRounds = 20;

  // part 2
  constexpr int kRounds = 10000;

  for (int round = 1; round <= kRounds; round++)
  {
    for (int i = 0; i < monkeys.size(); i++)
    {
      auto & monkey = monkeys[i];

      for (auto & item : monkey.items)
      {
        if (monkey.operation == "+")
          item += monkey.operationNumber;
        else if (monkey.operation == "*")
          item *= monkey.operationNumber;
        else if (monkey.operation == "+old")
          item = item + item;
        else if (monkey.operation == "*old")
          item = item * item;
        else
          assert(false);

        // part 1
        // item /= 3;

        // part 2
        item = item % cmmmc;

        if (item % monkey.testNumber == 0)
        {
          monkeys[monkey.testTrue].items.push_back(item);
        }
        else
        {
          // item = item / monkey.testNumber;
          monkeys[monkey.testFalse].items.push_back(item);
        }

        monkey.inspectedItemsTimes++;
      }

      monkey.items.clear();
    }
  }

  vector<long long> levels;
  for (const auto & monkey : monkeys)
  {
    levels.push_back(monkey.inspectedItemsTimes);
  }

  sort(begin(levels), end(levels), greater<long long>());
  cout << levels[0] * levels[1];

  return 0;
}
