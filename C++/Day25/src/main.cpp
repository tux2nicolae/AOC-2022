/**
 * Advent of code 2022
 * @author : Nicolae Telechi
 */
#include <algorithm>
#include <assert.h>
#include <bitset>
#include <deque>
#include <fstream>
#include <functional>
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

long long charToDigit(char c)
{
  if (c == '-')
    return -1;
  else if (c == '=')
    return -2;
  else
    return c - '0';
}

long long SNAFUToNumber(const string & snafu)
{
  long long multiply = 1;
  long long res      = 0;

  for (int i = (int)snafu.size() - 1; i >= 0; i--)
  {
    res += charToDigit(snafu[i]) * multiply;
    multiply *= 5;
  }

  return res;
}

void NumberToSNAFU(long long number, long long multiply, string str)
{
  if (!multiply)
  {
    if (number == 0)
      cout << str << endl;

    return;
  }

  if (AOC::Abs(number - multiply) >= AOC::Abs(number) &&
      AOC::Abs(number + multiply) >= AOC::Abs(number))
  {
    NumberToSNAFU(number, multiply / 5, str + "0");
  }
  else if (number > 0)
  {
    NumberToSNAFU(number - multiply, multiply / 5, str + "1");
    NumberToSNAFU(number - multiply * 2, multiply / 5, str + "2");
  }
  else
  {
    NumberToSNAFU(number + multiply, multiply / 5, str + "-");
    NumberToSNAFU(number + multiply * 2, multiply / 5, str + "=");
  }
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          fuelRequirements = reader.ReadLines();

  long long sum = 0;
  for (int i = 0; i < fuelRequirements.size(); i++)
  {
    sum += SNAFUToNumber(fuelRequirements[i]);
  }

  cout << sum << endl;

  NumberToSNAFU(sum, pow(5, 20), "");
  assert(sum == SNAFUToNumber("122-12==0-01=00-0=02"));

  return 0;
}
