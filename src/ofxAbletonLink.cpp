/*! @file ofxAbletonLink.cpp
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

#include "ofxAbletonLink.h"
#include <algorithm>

ofxAbletonLink::ofxAbletonLink()
    : link(nullptr),
    quantum_(4.0){
}

ofxAbletonLink::~ofxAbletonLink(){
    if(link != nullptr){
        link->enable(false);
        delete link;
    }
}

void ofxAbletonLink::setup(double tempo){
    if(link != nullptr){
        link->enable(false);
        delete link;
    }
    link = new ableton::Link(tempo);
    link->setNumPeersCallback([this](std::size_t /*peers*/){
        // nop
    });
    link->setTempoCallback([this](const double /*bpm*/){
        // nop
    });
    link->enable(true);
}

void ofxAbletonLink::setTempo(double bpm){
    if (link == nullptr){
        return;
    }
    auto timeline = link->captureAppTimeline();
    const auto time = link->clock().micros();
    timeline.setTempo(bpm, time);
    link->commitAppTimeline(timeline);
    
}

double ofxAbletonLink::tempo(){
    if(link == nullptr){
        return 0.0;
    }
    return link->captureAppTimeline().tempo();
}

void ofxAbletonLink::setQuantum(double quantum){
    this->quantum_ = quantum;
}

double ofxAbletonLink::quantum(){
    return quantum_;
}

bool ofxAbletonLink::isEnabled() const{
    if(link == nullptr){
        return false;
    }
    return link->isEnabled();
}

void ofxAbletonLink::enable(bool bEnable){
    if(link == nullptr){
        return false;
    }
    return link->enable(bEnable);
}

std::size_t ofxAbletonLink::numPeers(){
    if(link == nullptr){
        return 0;
    }
    return link->numPeers();
}

ofxAbletonLink::Status ofxAbletonLink::update(){
    Status status;
    if(link == nullptr){
        return status;
    }
    const auto time = link->clock().micros();
    auto timeline = link->captureAppTimeline();
    status.beat  = timeline.beatAtTime(time, quantum_);
    status.phase = timeline.phaseAtTime(time, quantum_);
    return status;
}

