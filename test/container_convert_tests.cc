/*
 * Repository:  https://github.com/kingkybel/ContainerConvert
 * File Name:   test/container_convert_tests.cc
 * Description: Unit tests for any utilities.
 *
 * Copyright (C) 2023 Dieter J Kybelksties <github@kybelksties.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * @date: 2023-08-28
 * @author: Dieter J Kybelksties
 */
#include "container_convert.h"
#include <compare>

#include <deque>
#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace util;

class ContainerConvertTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // just in case
    }

    void TearDown() override
    {
        // just in case
    }
};

TEST_F(ContainerConvertTest, erase_remove_test)
{
    auto que = deque<int>{5, 3, 6, 2, 7, 1, 4};
    util::eraseRemove(que, [](int i) { return i < 4; });
    auto expected_que = deque<int>{5, 6, 7, 4};
    ASSERT_EQ(que.size(), expected_que.size());
    for (size_t i = 0; i < expected_que.size(); ++i)
    {
        ASSERT_EQ(que[i], expected_que[i]);
    }

    auto vec = vector<int>{5, 3, 6, 2, 7, 1, 4};
    util::eraseRemove(que, [](int i) { return i < 4; });
    auto expected_vec = deque<int>{5, 6, 7, 4};
    ASSERT_EQ(que.size(), expected_vec.size());
    for (size_t i = 0; i < expected_vec.size(); ++i)
    {
        ASSERT_EQ(que[i], expected_vec[i]);
    }
}

TEST_F(ContainerConvertTest, erase_test)
{
    auto str_int_map = map<string, int, less<>>{
        {"one",   1},
        {"two",   2},
        {"three", 3},
        {"four",  4},
        {"five",  5}
    };
    ASSERT_TRUE(str_int_map.contains("one"));
    ASSERT_TRUE(str_int_map.contains("two"));
    ASSERT_TRUE(str_int_map.contains("three"));
    ASSERT_TRUE(str_int_map.contains("four"));
    ASSERT_TRUE(str_int_map.contains("five"));
    // remove all where key contains 'o'
    eraseByKey(str_int_map, [](std::string_view key) { return key.contains('o'); });
    ASSERT_FALSE(str_int_map.contains("one"));
    ASSERT_FALSE(str_int_map.contains("two"));
    ASSERT_TRUE(str_int_map.contains("three"));
    ASSERT_FALSE(str_int_map.contains("four"));
    ASSERT_TRUE(str_int_map.contains("five"));

    str_int_map = map<string, int, less<>>{
        {"one",   1},
        {"two",   2},
        {"three", 3},
        {"four",  4},
        {"five",  5}
    };
    ASSERT_TRUE(str_int_map.contains("one"));
    ASSERT_TRUE(str_int_map.contains("two"));
    ASSERT_TRUE(str_int_map.contains("three"));
    ASSERT_TRUE(str_int_map.contains("four"));
    ASSERT_TRUE(str_int_map.contains("five"));
    eraseByValue(str_int_map, [](int const& value) { return value < 3; });
    ASSERT_FALSE(str_int_map.contains("one"));
    ASSERT_FALSE(str_int_map.contains("two"));
    ASSERT_TRUE(str_int_map.contains("three"));
    ASSERT_TRUE(str_int_map.contains("four"));
    ASSERT_TRUE(str_int_map.contains("five"));
}

TEST_F(ContainerConvertTest, moveElementsTo_test)
{
    auto vec1 = vector<int>{
        {1, 2, 4, 8, 16, 32, 64, 128, 256}
    };
    auto vec2 = vector<int>{
        {-1, 333}
    };
    auto gt20 = [](int i) { return i > 20; };
    moveElementsTo(vec1, vec2, gt20);

    auto expVec1 = vector<int>{
        {1, 2, 4, 8, 16}
    };
    ASSERT_EQ(vec1, expVec1);
    auto expVec2 = vector<int>{
        {-1, 333, 32, 64, 128, 256}
    };
    ASSERT_EQ(vec2, expVec2);

    moveElementsTo(vec1, vec2);
    expVec1 = vector<int>{};
    ASSERT_EQ(vec1, expVec1);
    expVec2 = vector<int>{
        {-1, 333, 32, 64, 128, 256, 1, 2, 4, 8, 16}
    };
    ASSERT_EQ(vec2, expVec2);
}

TEST_F(ContainerConvertTest, toVector_test)
{
    auto dblSet = set<double, less<>>{
        {-2.5, -1e5, 0.0, 10.0, 4711.0}
    };
    auto vec = toVector(dblSet);
    ASSERT_EQ(vec.size(), dblSet.size());

    for (auto const& el: vec)
    {
        ASSERT_TRUE(dblSet.contains(el));
    }

    auto chrQue = deque{
        {'a', 'b', 'c'}
    };
    auto chrVec = toVector(chrQue);
    auto expVec = vector{
        {'a', 'b', 'c'}
    };
    ASSERT_EQ(chrVec, expVec);
}

TEST_F(ContainerConvertTest, toDeque_test)
{
    auto strVec = vector<string>{
        {"abc", "efg", "hij"}
    };
    auto que = toDeque(strVec);
    ASSERT_EQ(que.size(), strVec.size());

    for (size_t i = 0; i < que.size(); i++)
    {
        ASSERT_EQ(que[i], strVec[i]);
    }
}

struct NonPodType
{
    int    i;
    double d;

    auto operator<=>(NonPodType const&) const = default;
};

namespace std
{
template <> struct hash<NonPodType>
{
    std::size_t operator()(NonPodType const& v) const
    {
        hash<double> hasher_d;
        hash<int>    hasher_i;

        return (hasher_i(v.i) + hasher_d(v.d));
    }
};
}; //  namespace std

TEST_F(ContainerConvertTest, toSet_test)
{
    auto strVec = vector<string>{
        {"abc", "efg", "hij", "OnlyOnce", "abc", "efg", "hij", "efg", "hij"}
    };
    auto strSet = toSet(strVec);
    for (auto const& el: strVec)
    {
        ASSERT_TRUE(strSet.contains(el));
    }

    auto nonPodUSet = unordered_set<NonPodType>{
        {{1, 1.0}, {-1, 17.0}, {20, 1e-2}, {-666, 3.4e5}, {3, 3.1415}}
    };
    auto nonPodSet = toSet(nonPodUSet);
    ASSERT_EQ(nonPodSet.size(), nonPodUSet.size());

    for (auto const& el: nonPodUSet)
    {
        ASSERT_TRUE(nonPodSet.contains(el));
    }
}

struct StringHash
{
    using is_transparent = void;

    size_t operator()(string_view sv) const
    {
        return hash<string_view>{}(sv);
    }
};

TEST_F(ContainerConvertTest, toMap_test)
{
    auto strIntMap = unordered_map<string, int, StringHash, equal_to<>>{
        {"Ground", 1  },
        {"Floor",  666},
        {"Table",  -3 }
    };
    auto strIntUMap = toMap(strIntMap);
    ASSERT_EQ(strIntMap.size(), strIntUMap.size());
    for (auto const& [key, value]: strIntMap)
    {
        ASSERT_EQ(strIntUMap[key], value);
    }
}

TEST_F(ContainerConvertTest, toOrderedKeySet_test)
{
    auto strIntUMap = unordered_map<string, int, StringHash, equal_to<>>{
        {"Ground", 1  },
        {"Floor",  666},
        {"Table",  -3 }
    };
    auto strSet = toOrderedKeySet(strIntUMap);
    ASSERT_EQ(strSet.size(), strIntUMap.size());
    for (auto const& [key, _]: strIntUMap)
    {
        ASSERT_TRUE(strSet.contains(key));
    }
}

TEST_F(ContainerConvertTest, vector_toSet_compile_error_test)
{
#ifdef VECTOR_TO_SET_COMPILATION_ERROR
    // Following will not compile which is what we want
    struct x // x is neither comparable by "==" nor "<"
    {
    };

    vector<x> xVec;

    auto xSet = toSet(xVec); // this requires comparability
#else
    cout << "Pass parameter '-DVECTOR_TO_SET_COMPILATION_ERROR' to verify static assert works" << endl;
#endif
}

TEST_F(ContainerConvertTest, unordered_set_toSet_compile_error_test)
{
#ifdef UNORDERED_SET_TO_SET_COMPILATION_ERROR
    // Following will not compile which is what we want
    struct x // x is neither comparable by "==" nor "<"
    {
    };

    unordered_set<x> xVec;

    auto xSet = toSet(xVec); // this requires comparability
#else
    cout << "Pass parameter '-DUNORDERED_SET_TO_SET_COMPILATION_ERROR' to verify static assert works" << endl;
#endif
}

TEST_F(ContainerConvertTest, eraseByKey_truePredicate_removesAll)
{
    auto values = map<string, int, less<>>{
        {"one",   1},
        {"two",   2},
        {"three", 3}
    };
    eraseByKey(values, true_pred<string>{});
    ASSERT_TRUE(values.empty());
}

TEST_F(ContainerConvertTest, eraseByValue_truePredicate_removesAll)
{
    auto values = map<string, int, less<>>{
        {"one",   1},
        {"two",   2},
        {"three", 3}
    };
    eraseByValue(values, true_pred<int>{});
    ASSERT_TRUE(values.empty());
}

TEST_F(ContainerConvertTest, eraseRemove_predicateMatchesNothing_keepsContainer)
{
    auto vec = vector<int>{1, 2, 3, 4};
    eraseRemove(vec, [](int i) { return i > 100; });
    auto expected = vector<int>{1, 2, 3, 4};
    ASSERT_EQ(vec, expected);
}

TEST_F(ContainerConvertTest, moveElementsTo_predicateMatchesNothing_keepsSource)
{
    auto src = vector<int>{1, 2, 3};
    auto dst = vector<int>{10};
    moveElementsTo(src, dst, [](int i) { return i < 0; });
    auto expectedSrc = vector<int>{1, 2, 3};
    auto expectedDst = vector<int>{10};
    ASSERT_EQ(src, expectedSrc);
    ASSERT_EQ(dst, expectedDst);
}

TEST_F(ContainerConvertTest, emptyContainerConversions_returnEmptyContainers)
{
    auto emptySet  = set<int, less<>>{};
    auto emptyQue  = deque<int>{};
    auto emptyVec  = vector<int>{};
    auto emptyUSet = unordered_set<int>{};
    auto emptyUMap = unordered_map<string, int, StringHash, equal_to<>>{};

    auto vecFromSet  = toVector(emptySet);
    auto vecFromQue  = toVector(emptyQue);
    auto queFromVec  = toDeque(emptyVec);
    auto setFromVec  = toSet(emptyVec);
    auto setFromUSet = toSet(emptyUSet);
    auto mapFromUMap = toMap(emptyUMap);
    auto keysFromMap = toOrderedKeySet(emptyUMap);

    ASSERT_TRUE(vecFromSet.empty());
    ASSERT_TRUE(vecFromQue.empty());
    ASSERT_TRUE(queFromVec.empty());
    ASSERT_TRUE(setFromVec.empty());
    ASSERT_TRUE(setFromUSet.empty());
    ASSERT_TRUE(mapFromUMap.empty());
    ASSERT_TRUE(keysFromMap.empty());
}
