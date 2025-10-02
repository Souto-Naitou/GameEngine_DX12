#pragma once

template <class T>
class Range
{
public:
    Range() : start_(), end_() {}
    explicit Range(T _start, T _end) : start_(_start), end_(_end) {}

    T& start() { return start_; }
    T& end() { return end_; }
    const T& start() const { return start_; }
    const T& end() const { return end_; }


private:
    T start_;
    T end_;
};