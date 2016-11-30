#pragma once

#include "ofConstants.h"

#if defined(TARGET_OSX)
#define LINK_PLATFORM_MACOSX 1
#elif defined(TARGET_LINUX)
#define LINK_PLATFORM_LINUX 1
#elif defined(TARGET_WIN32)
#define LINK_PLATFORM_WINDOWS 1
#endif

#include "Link.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

class ofxAbletonLinkListener;

class ofxAbletonLink{
    public:
        struct Status{
            double beat;
            double phase;
            Status() : beat(0.0), phase(0.0){}
        };
        ofxAbletonLink();
        ~ofxAbletonLink();

        ofxAbletonLink(const ofxAbletonLink&) = delete;
        ofxAbletonLink& operator=(const ableton::Link&) = delete;
        ofxAbletonLink(ofxAbletonLink&&) = delete;
        ofxAbletonLink& operator=(ofxAbletonLink&&) = delete;

        void setup(double bpm);

        void setTempo(double bpm);
        double tempo();

        void setQuantum(double quantum);
        double quantum();

        bool isEnabled() const;
        void enable(bool bEnable);

        std::size_t numPeers();

        Status update();

    private:
        ableton::Link* link;
        double quantum_;
};

