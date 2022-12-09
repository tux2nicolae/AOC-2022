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
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          v = reader.ReadLines();

  // part 1 : use 2 instead of 10
  vector<AOC::Point> knots(10, AOC::Point{});

  set<AOC::Point> visited;
  visited.insert(knots.front());

  // lines
  for (auto & line : v)
  {
    vector<string> tokens = AOC::Explode(line, ' ');

    string moveDirection = tokens[0];
    int    moveCount     = stoi(tokens[1]);

    while (moveCount--)
    {
      // move first head
      if (moveDirection == "L")
        knots[0] = knots[0].GetLeft();
      else if (moveDirection == "R")
        knots[0] = knots[0].GetRight();
      else if (moveDirection == "U")
        knots[0] = knots[0].GetTop();
      else if (moveDirection == "D")
        knots[0] = knots[0].GetBottom();

      // follow next point
      for (int i = 0; i < knots.size() - 1; i++)
      {
        auto & head = knots[i];
        auto & tail = knots[i + 1];

        auto neighbours = head.GetAll2DNeighbours();
        neighbours.push_back(head);

        while (find(neighbours.begin(), neighbours.end(), tail) == neighbours.end())
        {
          auto diff = head - tail;

          if (diff.x)
            diff.x /= AOC::Abs(diff.x);

          if (diff.y)
            diff.y /= AOC::Abs(diff.y);

          tail += diff;
        }
      }

      // mark tail as visited
      visited.insert(knots.back());
    }
  }

  cout << visited.size();
  return 0;
}
