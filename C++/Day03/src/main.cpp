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

int getPriority(char c)
{
  int priority = 0;

  if ('A' <= c && c <= 'Z')
  {
    priority = 27 + (c - 'A');
  }
  else if ('a' <= c && c <= 'z')
  {
    priority = 1 + (c - 'a');
  }
  else
  {
    assert(false);
  }

  return priority;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader  reader(in);
  vector<string> v = reader.ReadLines();

  int s = 0;

  // part 2
  for (int i = 0; i < v.size(); i += 3)
  {
    // auto a = line.substr(0, line.size() / 2);
    // auto b = line.substr(line.size() / 2);

    auto & line1 = v[i + 0];
    auto & line2 = v[i + 1];
    auto & line3 = v[i + 2];

    for (auto c : line1)
    {
      bool found1 = (string::npos != line2.find(c));
      bool found2 = (string::npos != line3.find(c));

      if (found1 && found2)
      {
        s += getPriority(c);
        break;
      }
    }
  }

  cout << s << endl;
  // cout << part2;

  // out
  // FStreamWriter writer(out);
  // writer.WriteVector(v);

  return 0;
}
