//
//  Clock.hpp
//  fornani
//
//  Created by Alex Frasca on 12/26/22.
//

#pragma once

#include <memory>
#include <random>
#include <chrono>

namespace util {

const int ms_delay{0};

class Clock {
    
public:
    
    using Clk = std::chrono::steady_clock;
    using Tpt = std::chrono::time_point<std::chrono::steady_clock>;
    using Time = std::chrono::duration<float>;
    
    Clock() = default;
    
    void tick() {
        auto new_time = Clk::now();
        auto frame_time = Time{new_time - current_time};
        current_time = new_time;
        elapsed_time += frame_time;
        seconds += elapsed_time.count();
        
        if(elapsed_time.count() > time_step.count()) {
            elapsed_time = Time::zero();
        }
        ++frame;
    }
    
    bool every_x_frames(const int freq) {
        return frame % freq == 0;
    }
    
    bool every_x_seconds(const int num_seconds) {
        
    }
    
    bool every_x_milliseconds(int num_milliseconds) {
        
    }
    
    int seconds_int() { return static_cast<int>(std::floor(seconds)); }
    
    Time elapsed_time{};
    Tpt current_time = Clk::now();
    float seconds{0.0f};
    Time time_step{std::chrono::milliseconds(ms_delay)};
    int frame{0};
    
    
}; // End Clock

}

 /* Clock_hpp */
