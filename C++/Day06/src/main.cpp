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

  // part 2
  auto & line = v[0];

  map<char, int> message;
  constexpr int  kMessageSize = 14;

  for (int i = 0; i < kMessageSize; i++)
  {
    message[line[i]]++;
  }

  for (int i = kMessageSize; i < line.size(); i++)
  {
    message[line[i]]++;
    message[line[i - kMessageSize]]--;

    if (message[line[i - kMessageSize]] == 0)
    {
      message.erase(line[i - kMessageSize]);
    }

    if (message.size() == kMessageSize)
    {
      cout << i + 1;
      break;
    }
  }

  return 0;
}
