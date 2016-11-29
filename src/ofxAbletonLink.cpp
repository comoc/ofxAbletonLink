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

ofxAbletonLink::ofxAbletonLink()
    : link(nullptr),
    listener(nullptr),
    quantum_(4.0){
}

ofxAbletonLink::~ofxAbletonLink(){
    if (link != nullptr) {
        link->enable(false);
        delete link;
    }
}

void ofxAbletonLink::setup(double tempo, ofxAbletonLinkListener* listener){
    if (link != nullptr) {
        link->enable(false);
        delete link;
    }
    link = new ableton::Link(tempo);
    listener = listener;
    link->setNumPeersCallback([this](std::size_t peers) {
            if (this->listener != nullptr)
            this->listener->onNumberOfPeersChanged(static_cast<unsigned long>(peers));
            });
    link->setTempoCallback([this](const double bpm) {
            if (this->listener != nullptr)
            this->listener->onTempoChanged(bpm);
            });
    link->enable(true);
}

void ofxAbletonLink::setTempo(double bpm, std::chrono::microseconds atTime){
    if (link == nullptr) {
        return;
    }
    auto timeline = link->captureAppTimeline();
    timeline.setTempo(bpm, atTime);
}

double ofxAbletonLink::tempo(){
    if (link == nullptr) {
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
    if (link == nullptr) {
        return false;
    }
    return link->isEnabled();
}

void ofxAbletonLink::enable(bool bEnable){
    if (link == nullptr) {
        return false;
    }
    return link->enable(bEnable);
}

std::size_t ofxAbletonLink::numPeers(){
    if (link == nullptr) {
        return 0;
    }
    return link->numPeers();
}

ofxAbletonLink::Status ofxAbletonLink::update(){
    Status status;
    if (link == nullptr) {
        return status;
    }
    const auto time = link->clock().micros();
    auto timeline = link->captureAppTimeline();

    status.beat  = timeline.beatAtTime(time, quantum_);
    status.phase = timeline.phaseAtTime(time, quantum_);
    return status;
}

