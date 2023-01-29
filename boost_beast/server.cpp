//------------------------------------------------------------------------------
//
// Example: HTTP server, coroutine
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/config.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
template<
    class Body, class Allocator,
    class Send>
void
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    auto const respond =
    [&req, &send](beast::string_view body, http::status status)
    {
        http::response<http::string_body> res{status, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(body);
        res.content_length(body.size());
        res.prepare_payload();
        return send(std::move(res));
    };


    // Make sure we can handle the method
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
        return respond("Unknown HTTP-method.", http::status::bad_request);

    auto path{req.target()};

    // Request path must be absolute and not contain "..".
    if( path.empty() ||
        path[0] != '/' ||
        path.find("..") != beast::string_view::npos)
        return respond("Illegal request-target.", http::status::bad_request);

    static const std::string prefix{"/add/"};
    if(path.find(prefix) != 0)
        return respond("The resource '" + std::string(path) + "' was not found.", http::status::not_found);

    path.remove_prefix(prefix.size());

    std::vector<std::string> args;
    boost::split(args, path, boost::is_any_of("/"));

    if(args.size() != 2)
        return respond("Wrong number of arguments (" + std::to_string(args.size()) + ") in the path `" + std::string(path) + "`.", http::status::bad_request);

    const int arg1 = std::stoi(args[0]);
    const int arg2 = std::stoi(args[1]);

    const std::string& response = std::to_string(arg1) + " + " + std::to_string(arg2) + " = " + std::to_string(arg1 + arg2) + "\n";

    // Respond to GET request
    return respond(response, http::status::ok);
}

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// This is the C++11 equivalent of a generic lambda.
// The function object is used to send an HTTP message.
struct send_lambda
{
    beast::tcp_stream& stream_;
    bool& close_;
    beast::error_code& ec_;
    net::yield_context yield_;

    send_lambda(
        beast::tcp_stream& stream,
        bool& close,
        beast::error_code& ec,
        net::yield_context yield)
        : stream_(stream)
        , close_(close)
        , ec_(ec)
        , yield_(yield)
    {
    }

    template<bool isRequest, class Body, class Fields>
    void
    operator()(http::message<isRequest, Body, Fields>&& msg) const
    {
        // Determine if we should close the connection after
        close_ = msg.need_eof();

        // We need the serializer here because the serializer requires
        // a non-const file_body, and the message oriented version of
        // http::write only works with const messages.
        http::serializer<isRequest, Body, Fields> sr{msg};
        http::async_write(stream_, sr, yield_[ec_]);
    }
};

// Handles an HTTP server connection
void
do_session(
    beast::tcp_stream& stream,
    net::yield_context yield)
{
    bool close = false;
    beast::error_code ec;

    // This buffer is required to persist across reads
    beast::flat_buffer buffer;

    // This lambda is used to send messages
    send_lambda lambda{stream, close, ec, yield};

    for(;;)
    {
        // Set the timeout.
        stream.expires_after(std::chrono::seconds(30));

        // Read a request
        http::request<http::string_body> req;
        http::async_read(stream, buffer, req, yield[ec]);
        if(ec == http::error::end_of_stream)
            break;
        if(ec)
            return fail(ec, "read");

        // Send the response
        handle_request(std::move(req), lambda);
        if(ec)
            return fail(ec, "write");
        if(close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            break;
        }
    }

    // Send a TCP shutdown
    stream.socket().shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
void
do_listen(
    net::io_context& ioc,
    tcp::endpoint endpoint,
    net::yield_context yield)
{
    beast::error_code ec;

    // Open the acceptor
    tcp::acceptor acceptor(ioc);
    acceptor.open(endpoint.protocol(), ec);
    if(ec)
        return fail(ec, "open");

    // Allow address reuse
    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if(ec)
        return fail(ec, "set_option");

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if(ec)
        return fail(ec, "bind");

    // Start listening for connections
    acceptor.listen(net::socket_base::max_listen_connections, ec);
    if(ec)
        return fail(ec, "listen");

    for(;;)
    {
        tcp::socket socket(ioc);
        acceptor.async_accept(socket, yield[ec]);
        if(ec)
            fail(ec, "accept");
        else
            boost::asio::spawn(
                acceptor.get_executor(),
                std::bind(
                    &do_session,
                    beast::tcp_stream(std::move(socket)),
                    std::placeholders::_1));
    }
}

int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc > 4)
    {
        std::cerr <<
            "Usage: beast-server <address> <port> <number of threads>\n" <<
            "Example:\n" <<
            "    http-server-coro 0.0.0.0 3030\n";
        return EXIT_FAILURE;
    }
    auto const address = argc > 1 ? argv[1] : "0.0.0.0";
    auto const port = static_cast<unsigned short>(argc > 2 ? std::atoi(argv[2]) : 3030);
    auto const threads = std::max<int>(1, argc > 3 ? std::atoi(argv[3]) : std::thread::hardware_concurrency());

    std::cout<<"Bound to "<<address<<":"<<port<<"\nRunning with "<<threads<<" threads"<<std::endl;

    // The io_context is required for all I/O
    net::io_context ioc;

    // Spawn a listening port
    boost::asio::spawn(ioc,
        std::bind(
            &do_listen,
            std::ref(ioc),
            tcp::endpoint{net::ip::make_address(address), port},
            std::placeholders::_1));
    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back([&ioc]{ ioc.run(); });
    ioc.run();

    return EXIT_SUCCESS;
}
