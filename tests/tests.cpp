

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "set.cpp"
#include<algorithm>
#include<random>
#include<set>

TEST_CASE("sample") {
    Set<int> q;
    q.insert(3);
    q.insert(1);
    q.insert(0);
    auto it = q.begin();
    REQUIRE(q.find(0) == it);
    REQUIRE(*q.lower_bound(2) == 3);
    REQUIRE(q.lower_bound(4) == q.end());
    q.erase(*q.begin());
    q.insert(2);
    q.insert(2);
    q.erase(3);
    q.insert(13);
    REQUIRE(*q.lower_bound(4) == 13);
}

TEST_CASE("find in empty") {
    Set<int> q;
    REQUIRE(q.find(3) == q.end());
    REQUIRE(q.lower_bound(3) == q.end());
    Set<int> z = q;
}

TEST_CASE("all_functions") {
    std::set<int> right;
    Set<int> my;
    int queries = 6e4;
    while (queries--) {
        int type = rand() % 4;
        if (type == 0) { //add
            int x = rand() % 1000;
            my.insert(x);
            right.insert(x);
        }
        if (type == 1) {
            int x = rand() % 1000;
            my.erase(x);
            right.erase(x);
        }
        if (type == 2) {
            int x = rand() % 1000;
            if (right.lower_bound(x) != right.end()) {
                REQUIRE(*right.lower_bound(x) == *my.lower_bound(x));
            } else {
                REQUIRE(my.lower_bound(x) == my.end());
            }
        }
        if (type == 3) {
            int x = rand() % 1000;
            if (right.find(x) != right.end()) {
                REQUIRE(*right.find(x) == *my.find(x));
            } else {
                REQUIRE(my.find(x) == my.end());
            }
        }
    }
}

TEST_CASE("iterators") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = queries;
        right.insert(x);
        my.insert(x);
    }
    auto myiter = my.begin();
    auto rightiter = right.begin();
    while (myiter != my.end()) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    REQUIRE(rightiter == right.end());
}

TEST_CASE("iterators decrease") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = queries;
        right.insert(x);
        my.insert(x);
    }
    auto myiter = --my.end();
    auto rightiter = --right.end();
    while (myiter != my.begin()) {
        REQUIRE(*myiter == *rightiter);
        myiter--;
        rightiter--;
    }
    //REQUIRE(rightiter == right.end());
}

TEST_CASE("empty seet") {
    Set<int> q;
    REQUIRE(q.find(3) == q.end());
    REQUIRE(q.lower_bound(3) == q.end());
    Set<int> z = q;
    REQUIRE(z.begin() == z.end());
}

TEST_CASE("iterators 2") {
    std::set<int> right;
    Set<int> my;
    int queries = 10000;
    while (queries--) {
        int x = rand();
        right.insert(x);
        my.insert(x);
    }
    auto myiter = my.begin();
    auto rightiter = right.begin();
    while (true) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    queries = 10000;
    while (queries--) {
        int x = rand();
        right.insert(x);
        my.insert(x);
    }
    while (true) {
        REQUIRE(*myiter == *rightiter);
        myiter++;
        rightiter++;
    }
    REQUIRE(rightiter == right.end());
}