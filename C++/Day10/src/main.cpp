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

int cicle = 0;
int x     = 1;

vector<int>    strengths;
vector<string> CRT(6, string(40, ' '));

void checkStrength(int cicle)
{
  if (cicle == 20 || cicle == 60 || cicle == 100 || cicle == 140 || cicle == 180 || cicle == 220)
  {
    strengths.push_back(cicle * x);
  }
}

void draw(int cicle, int sprite)
{
  cicle--;

  int i = cicle / 40;
  int j = cicle % 40;

  if (j == sprite || j == sprite - 1 || j == sprite + 1)
  {
    CRT[i][j] = '#';
  }
  else
  {
    CRT[i][j] = '.';
  }
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          v = reader.ReadLines();

  FStreamWriter writter(out);

  // lines
  for (auto & line : v)
  {
    vector<string> tokens = AOC::Explode(line, ' ');

    string instruction = tokens[0];

    if (instruction == "addx")
    {
      int value = stoi(tokens[1]);

      cicle += 1;

      draw(cicle, x);
      checkStrength(cicle);

      cicle += 1;
      draw(cicle, x);
      checkStrength(cicle);

      x += value;
    }
    else
    {
      cicle += 1;

      draw(cicle, x);
      checkStrength(cicle);
    }
  }

  // part 1
  cout << accumulate(begin(strengths), end(strengths), 0);

  // part 2 (see _output.out)
  for (const auto & line : CRT)
  {
    out << line << endl;
  }

  return 0;
}
