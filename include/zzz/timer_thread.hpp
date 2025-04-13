#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <stop_token>
#include <thread>
#include <utility>

namespace zzz {

/**
 * A thread of execution that waits for a given duration and then calls a callback fn.
 * @details This creates a new thread of execution on construction with the non-default
 * constructor.
 */
class TimerThread {
   public:
    using CallbackType = std::function<void()>;
    using ClockType = std::chrono::steady_clock;

   public:
    /**
     * Create a placeholder TimerThread that does nothing.
     * @details You'll want to move assign over this to launch a TimerThread.
     */
    TimerThread() = default;

    /**
     * Create a TimerThread that will call the callback after the duration in a loop.
     * @details This will launch a thread immediately.
     * @param duration The periodic duration to wait before calling the callback
     * @param callback The function to call each time the duration has elapsed
     */
    explicit TimerThread(std::chrono::milliseconds duration, CallbackType callback)
        : thread_{[cb = std::move(callback), d = std::move(duration)](auto st) {
              TimerThread::run(st, cb, d);
          }}
    {}

    TimerThread(TimerThread const&) = delete;
    TimerThread(TimerThread&&) = default;

    auto operator=(TimerThread const&) -> TimerThread& = delete;
    auto operator=(TimerThread&& x) -> TimerThread& = default;

   public:
    /**
     * Request the TimerThread to stop, returns immediately.
     * @details This will request the thread to stop, it does not wait for it to stop
     * and will return immediately.
     */
    void request_stop() { thread_.request_stop(); }

   private:
    /**
     * Run the TimerThread, calling the callback after the given duration and repeating
     * until st.stop_requested() is true.
     * @param st The stop_token to check for stop_requested().
     * @param callback The function to call each time the duration has elapsed
     * @param duration The periodic duration to wait before calling the callback
     */
    static void run(std::stop_token st,
                    CallbackType const& callback,
                    std::chrono::milliseconds duration)
    {
        constexpr auto max_sleep = std::chrono::duration_cast<ClockType::duration>(
            std::chrono::milliseconds{16});

        auto next_time = ClockType::now() + duration;
        while (!st.stop_requested()) {
            auto const now = ClockType::now();

            if (now >= next_time) {
                callback();
                next_time += duration;
            }

            auto const sleep_duration = std::min(next_time - now, max_sleep);
            std::this_thread::sleep_for(sleep_duration);
        }
    }

   private:
    std::jthread thread_;
};

}  // namespace zzz