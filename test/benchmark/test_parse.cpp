/*
 * This file is part of the headcode.space url.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <chrono>

#include <gtest/gtest.h>

#include <headcode/benchmark/benchmark.hpp>
#include <headcode/url/url.hpp>


TEST(BenchmarkParse, Regular) {

    auto loop_count = 100'000u;
    auto url = "url://user:password@address:1234/this/is/a/path?with&a&query=param#and_a_fragment";

    auto time_start = std::chrono::high_resolution_clock::now();
    for (std::uint64_t i = 0; i < loop_count; ++i) {
        EXPECT_TRUE(headcode::url::URL{url}.IsValid());
    }

    headcode::benchmark::Throughput throughput{headcode::benchmark::GetElapsedMicroSeconds(time_start),
                                               std::string{url}.size() * loop_count};
    std::cout << StreamPerformanceIndicators(throughput, "BenchmarkParse::Regular ");
}
