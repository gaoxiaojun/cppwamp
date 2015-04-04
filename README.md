<!-- ---------------------------------------------------------------------------
                Copyright Butterfly Energy Systems 2014-2015.
         Distributed under the Boost Software License, Version 1.0.
             (See accompanying file LICENSE_1_0.txt or copy at
                    http://www.boost.org/LICENSE_1_0.txt)
---------------------------------------------------------------------------- -->

CppWAMP
=======

C++11 client library for the [WAMP][wamp] protocol.

**Features**:
- Supports the [WAMP Basic Profile][wamp-basic]
- Roles: _Caller_, _Callee_, _Subscriber_, _Publisher_
- Transports: TCP and Unix domain raw sockets (with and without handshaking support)
- Serializations: JSON and MsgPack
- Provides both callback and co-routine based asynchronous APIs
- Easy conversion between static and dynamic types
- RPC and pub/sub event handlers can have static argument types
- Header-only, but may also be optionally compiled
- Unit tested
- Permissive license (Boost)

**Dependencies**:
- [Boost.Asio][boost-asio] for raw socket transport
- [Boost.Endian][boost-endian] (requires [Boost][boost] 1.57.0 or greater)
- [RapidJSON][rapidjson]
- [msgpack-c][msgpack-c]
- (optional) [Boost.Coroutine][boost-coroutine] and
  [Boost.Context][boost-context]
- (for testing) [CMake][cmake] and the [Catch][catch] unit test framework

[wamp]: http://wamp.ws/
[wamp-basic]: https://github.com/tavendo/WAMP/blob/master/spec/basic.md
[boost-asio]: http://www.boost.org/doc/libs/release/doc/html/boost_asio.html
[boost-endian]: https://github.com/boostorg/endian
[boost]: http://boost.org
[rapidjson]: https://github.com/miloyip/rapidjson
[msgpack-c]: https://github.com/msgpack/msgpack-c
[boost-coroutine]: http://www.boost.org/doc/libs/release/libs/coroutine/doc/html/index.html
[boost-context]: http://www.boost.org/doc/libs/release/libs/context/doc/html/index.html
[cmake]: http://www.cmake.org/
[catch]: https://github.com/philsquared/Catch

Project Status
--------------

**THIS LIBRARY IS STILL IN A PRELIMINARY STATE, AND ITS API IS SUBJECT TO
CHANGE WITHOUT WARNING.**

Installation
-------------
- [Using as a Header-Only Library](./doc/usingheaderonly.md)
- [Building Library, Unit Tests, and Examples](./doc/building.md)

Documentation
-------------
- [Tutorial](./doc/tutorial.md)
- [Reference Documentation](http://ecorm.github.io/cppwamp/doc/index.html)

Usage Examples
--------------

### Establishing a WAMP session
```c++
wamp::AsioService iosvc;
boost::asio::spawn(iosvc, [&](boost::asio::yield_context yield)
{
    // Specify a TCP transport and JSON serialization
    auto tcp = wamp::legacy::TcpConnector::create(iosvc, "localhost",
                                                  8001, wamp::Json::id());

    auto client = wamp::CoroClient<>::create(tcp);
    client->connect(yield);
    auto sid = client->join("realm", yield);
    std::cout << "Client joined. Session ID = " << sid << "\n";

    // etc.
});
iosvc.run();
```

### Registering a remote procedure
```c++
void add(wamp::Invocation inv, int n, int m)
{
    inv.yield({n + m});
}

boost::asio::spawn(iosvc, [&](boost::asio::yield_context yield)
{
    ...
    auto reg = client->enroll<int, int>("add", &add, yield);
    ...
});
```

### Calling a remote procedure
```c++
auto result = client->call("add", {2, 2}, yield);
int sum = 0;
result.to(sum);
std::cout << "2 + 2 is " << sum << "\n";
```

### Subscribing to a topic
```c++
void sensorSampled(wamp::PublicationId pid, float value)
{
    std::cout << "Sensor sampled, publication ID = " << pid
              << " value = " << value << "\n";
}

boost::asio::spawn(iosvc, [&](boost::asio::yield_context yield)
{
    ...
    auto sub = client->subscribe<float>("sensorSampled", &sensorSampled,
                                        yield);
    ...
});
```

### Publishing an event
```c++
float value = std::rand() % 10;
client->publish("sensorSampled", {value});
```

Questions, Discussions, and Issues
----------------------------------

For general questions and discussion regarding CppWAMP, please use the
[CppWAMP Google Group][googlegroup] (membership required).

For reporting bugs or for suggesting enhancements, please use the GitHub
[issue tracker][issues].

[googlegroup]: https://groups.google.com/forum/#!forum/cppwamp
[issues]: https://github.com/ecorm/cppwamp/issues


License
-------

```
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```

* * *
_Copyright &copy; Butterfly Energy Systems, 2014-2015_
