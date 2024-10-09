export module tr:chrono;

import std;

export namespace tr {
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;
    template <class T> using Seconds = std::chrono::duration<T>;
    using SecondsF = Seconds<float>;
    using SecondsD = Seconds<double>;
    template <class T> using Milliseconds = std::chrono::duration<T, std::milli>;
    using MillisecondsF = Milliseconds<float>;
    using MillisecondsD = Milliseconds<double>;
    template <class T> using Microseconds = std::chrono::duration<T, std::micro>;
    using MicrosecondsF = Microseconds<float>;
    using MicrosecondsD = Microseconds<double>;
    template <class T> using Nanoseconds = std::chrono::duration<T, std::nano>;
    using NanosecondsF = Nanoseconds<float>;
    using NanosecondsD = Nanoseconds<double>;
}