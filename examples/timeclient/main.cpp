/*------------------------------------------------------------------------------
                Copyright Butterfly Energy Systems 2014-2015.
           Distributed under the Boost Software License, Version 1.0.
              (See accompanying file LICENSE_1_0.txt or copy at
                    http://www.boost.org/LICENSE_1_0.txt)
------------------------------------------------------------------------------*/

#include <ctime>
#include <iostream>
#include <cppwamp/conversion.hpp>
#include <cppwamp/corosession.hpp>
#include <cppwamp/json.hpp>
#include <cppwamp/tcp.hpp>
#include <cppwamp/unpacker.hpp>

const std::string realm = "cppwamp.demo.time";
const std::string address = "localhost";
const short port = 12345;

namespace wamp
{
    // Convert a std::tm to/from an object variant.
    template <typename TConverter>
    void convert(TConverter& conv, std::tm& t)
    {
        conv ("sec",   t.tm_sec)
             ("min",   t.tm_min)
             ("hour",  t.tm_hour)
             ("mday",  t.tm_mday)
             ("mon",   t.tm_mon)
             ("year",  t.tm_year)
             ("wday",  t.tm_wday)
             ("yday",  t.tm_yday)
             ("isdst", t.tm_isdst);
    }
}

void onTimeTick(wamp::Event, std::tm time)
{
    std::cout << "The current time is: " << std::asctime(&time) << "\n";
}

//------------------------------------------------------------------------------
int main()
{
    wamp::AsioService iosvc;

#ifdef CPPWAMP_USE_LEGACY_CONNECTORS
    auto tcp = wamp::legacyConnector<wamp::Json>(iosvc,
                                                 wamp::TcpHost(address, port));
#else
    auto tcp = wamp::connector<wamp::Json>(iosvc, wamp::TcpHost(address, port));
#endif

    using namespace wamp;
    auto session = CoroSession<>::create(tcp);

    boost::asio::spawn(iosvc, [&](boost::asio::yield_context yield)
    {
        session->connect(yield);
        session->join(Realm(realm), yield);

        auto result = session->call(Rpc("get_time"), yield);
        auto time = result[0].to<std::tm>();
        std::cout << "The current time is: " << std::asctime(&time) << "\n";

        session->subscribe(Topic("time_tick"),
                           wamp::unpackedEvent<std::tm>(&onTimeTick),
                           yield);
    });

    iosvc.run();

    return 0;
}
