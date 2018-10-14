/**
 * timer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <chrono>
#include <iostream>
#include <string>

/// Run a lambda inside of a timer, and print the time in ms.
template<typename f>
void run_timed(const std::string &name, f func, std::ostream &out = std::cout) {
    out << "Beginning " << name << '\n';
    const auto start = std::chrono::system_clock::now();
    func();
    const auto stop = std::chrono::system_clock::now();
    out << "Completed: " << (stop - start).count() / 1e9 << " seconds.\n";
}
