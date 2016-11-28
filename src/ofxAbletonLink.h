/*! @file ofxAbletonLink.h
 *  @copyright 2016, Akihiro Komori. All rights reserved.
 *  @brief Library for openFrameworks addon
 *
 *  @license:
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
//#include "AudioPlatform.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
// #if LINK_PLATFORM_UNIX
// #include <termios.h>
// #endif

class ofxAbletonLinkListener;

class ofxAbletonLink
{
	public:
    struct Result {
        double beat;
        double phase;
        Result() : beat(0.0), phase(0.0) {}
    };
		ofxAbletonLink();
        ~ofxAbletonLink();

        ofxAbletonLink(const ofxAbletonLink&) = delete;
        ofxAbletonLink& operator=(const ableton::Link&) = delete;
		ofxAbletonLink(ofxAbletonLink&&) = delete;
		ofxAbletonLink& operator=(ofxAbletonLink&&) = delete;

        void setup(double tempo, ofxAbletonLinkListener* listener = 0);
    
		void setTempo(double, std::chrono::microseconds atTime);
		double tempo();

        void setQuantum(double quantum);
        double quantum();
    
        bool isEnabled() const;
        void enable(bool bEnable);
    
		unsigned long numberOfPeers();

		Result update();
    
	private:
		ableton::Link* link;
		ofxAbletonLinkListener* listener;
        double quantum_;
};

class ofxAbletonLinkListener
{
	public:
        ofxAbletonLinkListener() {}
        virtual ~ofxAbletonLinkListener() {}
		virtual void onNumberOfPeersChanged(unsigned long peers) = 0;
		virtual void onTempoChanged(double tempo) = 0;
		ofxAbletonLinkListener(const ofxAbletonLinkListener&) = delete;
		ofxAbletonLinkListener(ofxAbletonLinkListener&&) = delete;
		ofxAbletonLinkListener& operator=(ofxAbletonLinkListener&&) = delete;
};

