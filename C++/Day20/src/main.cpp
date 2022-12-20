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

  // part 1
  // auto mixCount      = 1;
  // auto decriptionKey = 1;

  // part 2
  auto mixCount      = 10;
  auto decriptionKey = 811589153ll;

  vector<pair<long long, long long>> numbers;
  for (long long i = 0; i < lines.size(); i++)
  {
    numbers.push_back({ lines[i] * decriptionKey, i });
  }

  auto computeDestination =
    [&](vector<pair<long long, long long>>::iterator fromIt, long long count)
  {
    const long long modulo = numbers.size() - 1;

    auto fromPosition = fromIt - begin(numbers);
    auto toPosition   = (fromPosition + count % modulo + modulo) % modulo;

    return begin(numbers) + toPosition;
  };

  while (mixCount--)
  {
    for (long long i = 0; i < numbers.size(); i++)
    {
      auto from = find_if(begin(numbers), end(numbers),
                          [=](pair<long long, long long> nr)
                          {
                            return nr.second == i;
                          });

      assert(from != end(numbers));

      auto to = computeDestination(from, from->first);
      if (from < to)
      {
        std::rotate(from, from + 1, to + 1);
      }
      else
      {
        std::rotate(to, from, from + 1);
      }
    }
  }

  auto zeroIt = find_if(begin(numbers), end(numbers),
                        [](pair<long long, long long> nr)
                        {
                          return nr.first == 0;
                        });

  auto a = numbers[(distance(numbers.begin(), zeroIt) + 1000) % numbers.size()].first;
  auto b = numbers[(distance(numbers.begin(), zeroIt) + 2000) % numbers.size()].first;
  auto c = numbers[(distance(numbers.begin(), zeroIt) + 3000) % numbers.size()].first;

  std::cout << endl;
  std::cout << a << " + " << b << " + " << c << " = " << a + b + c << endl;

  return 0;
}
