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

int shapeToScore(char shape)
{
  assert('A' <= shape && shape <= 'Z');
  return shape - 'A' + 1;
};

char convertToABC(int a)
{
  if (a == 'X')
    return 'A';

  if (a == 'Y')
    return 'B';

  if (a == 'Z')
    return 'C';

  assert(false);
  return 0;
}

int wonScore(char a, char b)
{
  if (a == 'X' && b == 'A')
    return 3;

  if (a == 'Y' && b == 'B')
    return 3;

  if (a == 'Z' && b == 'C')
    return 3;

  if (a == 'X' && b == 'C')
    return 6;

  if (a == 'Y' && b == 'A')
    return 6;

  if (a == 'Z' && b == 'B')
    return 6;

  return 0;
}

int outCome(char a)
{
  if (a == 'X')
    return 0;

  if (a == 'Y')
    return 3;

  if (a == 'Z')
    return 6;

  return 0;
}

char chose(char a, char b)
{
  if (a == 'A')
  {
    if (b == 'X')
      return 'C';

    if (b == 'Y')
      return 'A';

    if (b == 'Z')
      return 'B';
  }

  //
  if (a == 'B')
  {
    if (b == 'X')
      return 'A';

    if (b == 'Y')
      return 'B';

    if (b == 'Z')
      return 'C';
  }

  //
  if (a == 'C')
  {
    if (b == 'X')
      return 'B';

    if (b == 'Y')
      return 'C';

    if (b == 'Z')
      return 'A';
  }

  assert(false);
  return 0;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  // ofstream out("..\\src\\_output.out");

  FStreamReader  reader(in);
  vector<string> v = reader.ReadLines();

  vector<int> e;

  int part1 = 0;
  int part2 = 0;

  // part 1
  for (const auto & line : v)
  {
    char a = line[0];
    char b = line[2];

    part1 += shapeToScore(convertToABC(b)) + wonScore(b, a);
    part2 += shapeToScore(chose(a, b)) + outCome(b);
  }

  cout << part1 << endl;
  cout << part2;

  // out
  // FStreamWriter writer(out);
  // writer.WriteVector(v);

  return 0;
}
