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

vector<vector<AOC::Point>> ParseInput(const vector<string> & lines)
{
  vector<vector<AOC::Point>> sensorsBeacons;

  for (int i = 0; i < lines.size(); i++)
  {
    auto & line = lines[i];

    auto matches =
      AOC::ExtractMatches(line, "Sensor at x=(.*), y=(.*): closest beacon is at x=(.*), y=(.*)");

    vector<AOC::Point> sensorBeacon;
    sensorBeacon.push_back(AOC::Point{ atoi(matches[0].c_str()), atoi(matches[1].c_str()) });
    sensorBeacon.push_back(AOC::Point{ atoi(matches[2].c_str()), atoi(matches[3].c_str()) });

    sensorsBeacons.push_back(sensorBeacon);
  }

  return sensorsBeacons;
}

int main()
{
  ifstream in("..\\src\\_input.in");
  ofstream out("..\\src\\_output.out");

  FStreamReader reader(in);
  auto          lines = reader.ReadLines();

  AOC::Point source{ 500, 0 };
  auto       sensorsBeacons = ParseInput(lines);

  set<AOC::Point> existing;
  set<AOC::Point> detected;

  for (auto & sensorBeacon : sensorsBeacons)
  {
    assert(sensorBeacon.size() == 2);

    auto & sensor = sensorBeacon[0];
    auto & beacon = sensorBeacon[1];

    existing.insert(sensor);
    existing.insert(beacon);
  }

  // part 1
  // int s = 0;

  // long long y = 2000000;
  // for (long long x = -20000000; x < 20000000; x++)
  //{
  //   AOC::Point point{ x, y };

  //  for (auto & sensorBeacon : sensorsBeacons)
  //  {
  //    assert(sensorBeacon.size() == 2);

  //    auto & sensor = sensorBeacon[0];
  //    auto & beacon = sensorBeacon[1];

  //    auto a = AOC::ManhattenDistance(sensor, beacon);
  //    auto b = AOC::ManhattenDistance(sensor, point);

  //    if (a >= b && !existing.count(point))
  //    {
  //      s++;
  //      break;
  //    }
  //  }
  //}

  // cout << s;

  sort(begin(sensorsBeacons), end(sensorsBeacons),
       [](const vector<AOC::Point> & first, const vector<AOC::Point> & second)
       {
         return first[0].x < second[0].x;
       });

  int s = 0;

  for (long long y = 0; y <= 4000000; y++)
  {
    for (long long x = 0; x <= 4000000;)
    {
      AOC::Point point{ x, y };

      long long maxXDiff = 1;

      bool allSensors = true;
      for (auto & sensorBeacon : sensorsBeacons)
      {
        assert(sensorBeacon.size() == 2);

        auto & sensor = sensorBeacon[0];
        auto & beacon = sensorBeacon[1];

        auto sensorToBeacon = AOC::ManhattenDistance(sensor, beacon);
        auto sensorToPoint  = AOC::ManhattenDistance(sensor, point);

        // in range
        if (sensorToBeacon >= sensorToPoint && !existing.count(point))
        {
          allSensors = false;
          maxXDiff   = max(maxXDiff, sensorToBeacon - sensorToPoint);
        }
      }

      x += maxXDiff;

      if (allSensors && !existing.count(point))
      {
        // evrica here
        cout << "(" << point.x << "," << point.y << ")"
             << ":";

        cout << point.x * 4000000 + point.y << endl;
        break;
      }
    }
  }

  return 0;
}
