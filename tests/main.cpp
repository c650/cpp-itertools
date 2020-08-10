#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>

#include <itertools.hpp>

TEST_CASE("Filter only even numbers", "[filter]") {
  std::vector<int> a{1, 2, 3, 4};
  std::vector<int> b{2, 4};

  REQUIRE(
      itertools::base_iter<int, std::vector<int>::iterator>(a.begin(), a.end())
          .filter([](int a) { return a % 2 == 0; })
          .collect<std::vector<int>>() == b);
};

TEST_CASE("Map numbers", "[map]") {
  std::vector<int> a{1, 2, 3, 4};
  std::vector<int> b{3, 6, 9, 12};

  REQUIRE(
      itertools::base_iter<int, std::vector<int>::iterator>(a.begin(), a.end())
          .map<int>([](int a) { return a * 3; })
          .collect<std::vector<int>>() == b);
};

TEST_CASE("Reduce numbers", "[reduce]") {
  std::vector<int> a{1, 2, 3, 4};

  REQUIRE(
      itertools::base_iter<int, std::vector<int>::iterator>(a.begin(), a.end())
          .reduce<int>(0, [](int& acc, int x) { return acc + x; }) == 10);
};

TEST_CASE("Map and reduce numbers", "[map][reduce]") {
  std::vector<int> a{1, 2, 3, 4};

  REQUIRE(
      itertools::base_iter<int, std::vector<int>::iterator>(a.begin(), a.end())
          .map<int>([](int a) { return 3 * a; })
          .reduce<int>(0, [](int& acc, int x) { return acc + x; }) == 30);
};

TEST_CASE("Filter and reduce numbers", "[filter][reduce]") {
  std::vector<int> a{1, 2, 3, 4};

  REQUIRE(
      itertools::base_iter<int, std::vector<int>::iterator>(a.begin(), a.end())
          .filter([](int a) { return a % 2 == 0; })
          .reduce<int>(0, [](int& acc, int x) { return acc + x; }) == 6);
};
