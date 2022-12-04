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

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader  reader(in);
  vector<string> v = reader.ReadLines();

  int s  = 0;
  int s2 = 0;

  // part 2
  for (int i = 0; i < v.size(); i++)
  {
    auto & line    = v[i];
    auto   matches = AOC::ExtractMatches(line, "(.*)-(.*),(.*)-(.*)");

    pair<int, int> a, b;
    a.first  = stoi(matches[1]);
    a.second = stoi(matches[2]);
    b.first  = stoi(matches[3]);
    b.second = stoi(matches[4]);

    // b is in a
    if (a.first <= b.first && b.second <= a.second)
    {
      s++;
    }
    // a is in b
    else if (b.first <= a.first && a.second <= b.second)
    {
      s++;
    }

    if (a.first <= b.first && b.first <= a.second)
    {
      s2++;
    }
    // a is in b
    else if (b.first <= a.first && a.first <= b.second)
    {
      s2++;
    }
  }

  cout << s << endl;
  cout << s2 << endl;

  return 0;
}
