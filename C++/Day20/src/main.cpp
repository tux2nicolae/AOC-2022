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
  auto          lines = reader.ReadDataAs<long long>();

  vector<pair<long long, long long>> numbers;
  for (long long i = 0; i < lines.size(); i++)
  {
    numbers.push_back({ lines[i] * 811589153, i });
  }

  auto computeDestination = [&](vector<pair<long long, long long>>::iterator from, long long count)
  {
    assert(from == begin(numbers) || from == end(numbers) - 1);

    auto fromPosition = from - begin(numbers);
    auto toPosition   = (fromPosition + count + 10 * numbers.size()) % numbers.size();

    return begin(numbers) + toPosition;
  };

  long long mixCount = 10;
  while (mixCount--)
  {
    for (long long i = 0; i < numbers.size(); i++)
    {
      auto it = find_if(begin(numbers), end(numbers),
                        [=](pair<long long, long long> nr)
                        {
                          return nr.second == i;
                        });

      assert(it != end(numbers));

      if (0 <= it->first)
      {
        long long count = it->first % (numbers.size() - 1);

        // bring to front
        std::rotate(begin(numbers), it, end(numbers));

        auto current = 0;
        while (count--)
        {
          auto next = current + 1;
          if (next == numbers.size())
            next = 0;

          swap(numbers[current], numbers[next]);

          current++;
          if (current == numbers.size())
            current = 0;
        }
      }
      else
      {
        auto count = AOC::Abs(it->first) % (numbers.size() - 1);

        // bring to back
        if (it != end(numbers) - 1)
        {
          std::rotate(begin(numbers), it + 1, end(numbers));
        }

        long long current = numbers.size() - 1;
        while (count--)
        {
          auto prev = current - 1;
          if (prev == -1)
            prev = numbers.size() - 1;

          swap(numbers[current], numbers[prev]);

          current--;
          if (current < 0)
            current = numbers.size() - 1;
        }
      }

      // for (long long i = 0; i < numbers.size(); i++)
      //{
      //   out << numbers[i].first << " ";
      // }

      // out << endl;
    }
  }

  auto zeroIt = find_if(begin(numbers), end(numbers),
                        [](pair<long long, long long> nr)
                        {
                          return nr.first == 0;
                        });

  auto count = count_if(begin(numbers), end(numbers),
                        [=](pair<long long, long long> nr)
                        {
                          return nr.second == 0;
                        });

  assert(count == 1);
  std::rotate(begin(numbers), zeroIt, end(numbers));

  auto a = numbers[1000].first;
  auto b = numbers[2000].first;
  auto c = numbers[3000].first;

  std::cout << endl;
  std::cout << a << endl;
  std::cout << b << endl;
  std::cout << c << endl;

  std::cout << endl;
  cout << a + b + c;

  return 0;
}
