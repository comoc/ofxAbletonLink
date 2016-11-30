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

