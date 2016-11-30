/* Copyright 2016, Ableton AG, Berlin. All rights reserved.
 *
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
 *
 *  If you would like to incorporate Link into a proprietary software application,
 *  please contact <link-devs@ableton.com>.
 */

#include <ableton/link/Peers.hpp>
#include <ableton/test/CatchWrapper.hpp>
#include <ableton/test/serial_io/Fixture.hpp>

namespace ableton
{
namespace link
{
namespace
{

struct SessionMembershipCallback
{
  void operator()()
  {
    ++calls;
  }

  std::size_t calls = 0;
};

struct SessionTimelineCallback
{
  void operator()(const SessionId& session, const Timeline& tl)
  {
    sessionTimelines.push_back(std::make_pair(session, tl));
  }

  std::vector<std::pair<SessionId, Timeline>> sessionTimelines;
};

const auto fooPeer =
  PeerState{{NodeId::random(), NodeId::random(),
              Timeline{Tempo{60.}, Beats{1.}, std::chrono::microseconds{1234}}},
    {}};

const auto barPeer =
  PeerState{{NodeId::random(), NodeId::random(),
              Timeline{Tempo{120.}, Beats{10.}, std::chrono::microseconds{500}}},
    {}};

const auto bazPeer =
  PeerState{{NodeId::random(), NodeId::random(),
              Timeline{Tempo{100.}, Beats{4.}, std::chrono::microseconds{100}}},
    {}};

const auto gateway1 = asio::ip::address::from_string("123.123.123.123");
const auto gateway2 = asio::ip::address::from_string("210.210.210.210");

using PeerVector = std::vector<typename Peers<test::serial_io::Context,
  SessionMembershipCallback,
  SessionTimelineCallback>::Peer>;

void expectPeers(PeerVector expected, PeerVector actual)
{
  CHECK(expected == actual);
}

void expectSessionTimelines(
  std::vector<std::pair<SessionId, Timeline>> expected, SessionTimelineCallback callback)
{
  CHECK(expected == callback.sessionTimelines);
}

} // anonymous namespace

TEST_CASE("Peers | EmptySessionPeersAfterInit", "[Peers]")
{
  test::serial_io::Fixture io;
  auto peers = makePeers(util::injectVal(io.makeIoContext()), SessionMembershipCallback{},
    SessionTimelineCallback{});
  io.flush();
  expectPeers({}, peers.sessionPeers(fooPeer.sessionId()));
}

TEST_CASE("Peers | AddAndFindPeer", "[Peers]")
{
  test::serial_io::Fixture io;
  auto membership = SessionMembershipCallback{};
  auto sessions = SessionTimelineCallback{};
  auto peers = makePeers(
    util::injectVal(io.makeIoContext()), std::ref(membership), std::ref(sessions));
  auto observer = makeGatewayObserver(peers, gateway1);

  sawPeer(observer, fooPeer);
  io.flush();

  expectPeers({{fooPeer, gateway1}}, peers.sessionPeers(fooPeer.sessionId()));
  CHECK(1u == membership.calls);
  expectSessionTimelines({make_pair(fooPeer.sessionId(), fooPeer.timeline())}, sessions);
}

TEST_CASE("Peers | AddAndRemovePeer", "[Peers]")
{
  test::serial_io::Fixture io;
  auto membership = SessionMembershipCallback{};
  auto peers = makePeers(
    util::injectVal(io.makeIoContext()), std::ref(membership), SessionTimelineCallback{});
  auto observer = makeGatewayObserver(peers, gateway1);

  sawPeer(observer, fooPeer);
  peerLeft(observer, fooPeer.ident());
  io.flush();

  expectPeers({}, peers.sessionPeers(fooPeer.sessionId()));
  CHECK(2u == membership.calls);
}

TEST_CASE("Peers | AddTwoPeersRemoveOne", "[Peers]")
{
  test::serial_io::Fixture io;
  auto membership = SessionMembershipCallback{};
  auto sessions = SessionTimelineCallback{};
  auto peers = makePeers(
    util::injectVal(io.makeIoContext()), std::ref(membership), std::ref(sessions));
  auto observer = makeGatewayObserver(peers, gateway1);

  sawPeer(observer, fooPeer);
  sawPeer(observer, barPeer);
  peerLeft(observer, fooPeer.ident());
  io.flush();

  expectPeers({}, peers.sessionPeers(fooPeer.sessionId()));
  expectPeers({{barPeer, gateway1}}, peers.sessionPeers(barPeer.sessionId()));
  CHECK(3u == membership.calls);
}

TEST_CASE("Peers | AddThreePeersTwoOnSameGateway", "[Peers]")
{
  test::serial_io::Fixture io;
  auto membership = SessionMembershipCallback{};
  auto sessions = SessionTimelineCallback{};
  auto peers = makePeers(
    util::injectVal(io.makeIoContext()), std::ref(membership), std::ref(sessions));
  auto observer1 = makeGatewayObserver(peers, gateway1);
  auto observer2 = makeGatewayObserver(peers, gateway2);

  sawPeer(observer1, fooPeer);
  sawPeer(observer2, fooPeer);
  sawPeer(observer1, barPeer);
  sawPeer(observer1, bazPeer);
  io.flush();

  expectPeers(
    {{fooPeer, gateway1}, {fooPeer, gateway2}}, peers.sessionPeers(fooPeer.sessionId()));
  CHECK(3 == membership.calls);
}

TEST_CASE("Peers | CloseGateway", "[Peers]")
{
  test::serial_io::Fixture io;
  auto membership = SessionMembershipCallback{};
  auto sessions = SessionTimelineCallback{};
  auto peers = makePeers(
    util::injectVal(io.makeIoContext()), std::ref(membership), std::ref(sessions));
  auto observer1 = makeGatewayObserver(peers, gateway1);
  {
    // The observer will close the gateway when it goes out of scope
    auto observer2 = makeGatewayObserver(peers, gateway2);
    sawPeer(observer2, fooPeer);
    sawPeer(observer2, barPeer);
    sawPeer(observer1, fooPeer);
    sawPeer(observer2, bazPeer);
  }
  io.flush();

  expectPeers({{fooPeer, gateway1}}, peers.sessionPeers(fooPeer.sessionId()));
  CHECK(4 == membership.calls);
}

} // namespace link
} // namespace ableton
