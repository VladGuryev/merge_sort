#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <list>
using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  const auto rangeLength = range_end - range_begin;
  if(rangeLength < 2)
    return;

  vector<typename RandomIt::value_type> range_copy(
                make_move_iterator(range_begin),
                make_move_iterator(range_end)
                );

  auto first_third = begin(range_copy) + rangeLength / 3;
  auto second_third = begin(range_copy) +  rangeLength * 2 / 3;

  MergeSort(begin(range_copy), first_third);
  MergeSort(first_third, second_third);
  MergeSort(second_third, end(range_copy));

   vector<typename RandomIt::value_type> buffer;
  merge(make_move_iterator(begin(range_copy)), make_move_iterator(first_third),
        make_move_iterator(first_third), make_move_iterator(second_third),
        back_inserter(buffer));
  merge(make_move_iterator(begin(buffer)), make_move_iterator(end(buffer)),
        make_move_iterator(second_third), make_move_iterator(end(range_copy)),
        range_begin);
}

class StringNonCopyable : public string {
public:
  using string::string;
  StringNonCopyable(string&& other) : string(move(other)) {}
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestMovableMergeSort() {
  {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
  }
  {
    vector<StringNonCopyable> vs;
    vs.push_back("3");
    vs.push_back("2");
    vs.push_back("1");
    vs.push_back("9");
    MergeSort(begin(vs), end(vs));
    ASSERT(is_sorted(begin(vs), end(vs)));
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestMovableMergeSort);
  return 0;
}
