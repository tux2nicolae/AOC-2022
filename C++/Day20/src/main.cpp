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

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadDataAs<int>();

  vector<pair<int, int>> numbers;
  for (int i = 0; i < lines.size(); i++)
    numbers.push_back({ lines[i], i });

  auto computeDestination = [&](vector<pair<int, int>>::iterator from, int count)
  {
    auto fromPosition = from - begin(numbers);
    auto toPosition   = fromPosition + count;

    // some normalization
    if (toPosition <= 0 || numbers.size() <= toPosition)
    {
      toPosition += toPosition <= 0 ? -1 : +1;
    }

    toPosition += numbers.size();

    auto to = begin(numbers) + (toPosition % numbers.size());
    return to;
  };

  for (int i = 0; i < numbers.size(); i++)
  {
    auto from = find_if(begin(numbers), end(numbers),
                        [&](const pair<int, int> & number)
                        {
                          return number.second == i;
                        });

    assert(from != end(numbers));

    auto to = computeDestination(from, from->first);
    if (from < to)
    {
      // simple rotation to the left
      std::rotate(from, from + 1, to + 1);
    }
    else if (from > to)
    {
      // simple rotation to the right
      std::rotate(to, from, from + 1);
    }

    for (int i = 0; i < numbers.size(); i++)
    {
      std::cout << numbers[i].first << " ";
    }

    std::cout << endl;
  }

  auto zeroIt = find_if(begin(numbers), end(numbers),
                        [&](const pair<int, int> & number)
                        {
                          return number.first == 0;
                        });
  std::cout << endl;
  std::cout << computeDestination(zeroIt, 1000)->first << endl;
  std::cout << computeDestination(zeroIt, 2000)->first << endl;
  std::cout << computeDestination(zeroIt, 3000)->first << endl;

  return 0;
}
