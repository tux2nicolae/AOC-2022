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
  {
    numbers.push_back({ lines[i], i });
  }

  auto computeDestination = [&](vector<pair<int, int>>::iterator from, int count)
  {
    assert(from == begin(numbers) || from == end(numbers) - 1);

    auto fromPosition = from - begin(numbers);
    auto toPosition   = (fromPosition + count + 10 * numbers.size()) % numbers.size();

    return begin(numbers) + toPosition;
  };

  for (int i = 0; i < numbers.size(); i++)
  {
    auto it = find_if(begin(numbers), end(numbers),
                      [=](pair<int, int> nr)
                      {
                        return nr.second == i;
                      });

    assert(it != end(numbers));

    if (0 <= it->first)
    {
      auto myNumber = it->first;

      // bring to front
      std::rotate(begin(numbers), it, end(numbers));
      assert(numbers.front().first == myNumber);

      auto from = begin(numbers);
      auto to   = computeDestination(from, numbers.front().first);

      assert(from <= to);
      if (from < to)
      {
        std::rotate(from, from + 1, to + 1);
      }
    }
    else
    {
      auto myNumber = it->first;

      // bring to back
      if (it != end(numbers) - 1)
      {
        std::rotate(begin(numbers), it + 1, end(numbers));
      }

      assert((numbers.end() - 1)->first == myNumber);

      auto from = computeDestination(end(numbers) - 1, numbers.back().first);
      auto to   = end(numbers) - 1;

      assert(from <= to);
      if (from < to)
      {
        std::rotate(from, to, to + 1);
      }
    }

    for (int i = 0; i < numbers.size(); i++)
    {
      out << numbers[i].first << " ";
    }

    out << endl;
  }

  auto zeroIt = find_if(begin(numbers), end(numbers),
                        [](pair<int, int> nr)
                        {
                          return nr.first == 0;
                        });

  auto count = count_if(begin(numbers), end(numbers),
                        [=](pair<int, int> nr)
                        {
                          return nr.second == 0;
                        });

  assert(count == 1);
  std::rotate(begin(numbers), zeroIt, end(numbers));

  // auto a = numbers[1000].first;
  // auto b = numbers[2000].first;
  // auto c = numbers[3000].first;

  // std::cout << endl;
  // std::cout << a << endl;
  // std::cout << b << endl;
  // std::cout << c << endl;

  // std::cout << endl;
  // cout << a + b + c;

  return 0;
}
