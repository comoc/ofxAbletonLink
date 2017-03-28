# ofxAbletonLink

ofxAbletonLink is an [Ableton Link](https://github.com/Ableton/link) addon for [openFrameworks](http://openframeworks.cc)

## Installation

To install ofxAbletonLink, move the ofxAbletonLink folder to your `of/addons/` folder.
A compiler that supports C++ 11 is required.
ofxAbletonLink relies on `link`, `asio-standalone` and `catch` as submodules. After checking out the
main repositories, those submodules have to be loaded using

```
git submodule update --init --recursive
```

## Examples

- `example-basic`: The simplest demo. This demo acts with other Ableton Link applications.
- `example-animation`: A sprite animation speed control demo. This demo acts with other Ableton Link applications. (Thank you [Irasutoya](http://www.irasutoya.com/))

## Demo

This gif shows the state of tempo synchronization between our `example-basic` and  Ableton's `QLinkHut`.

<img src="https://i.gyazo.com/95d1fde2180d1f6b1156bfe96196c1c5.gif" alt="https://gyazo.com/95d1fde2180d1f6b1156bfe96196c1c5" width="600">

## Notes

This repository is including a copy of [Ableton/link](https://github.com/Ableton/link), [chriskohlhoff/asio](https://github.com/chriskohlhoff/asio) and [philsquared/Catch](https://github.com/philsquared/Catch).

This addon is tested on macOS Sierra only.
