package com.quark4.app;

import bayou.http.HttpServer;
import bayou.http.HttpHandler;

/**
 * Simple async server
 *
 */
public class App
{
    public static void main(String... args) throws Exception
    {
        HttpHandler handler = new Adder();

        HttpServer server = new HttpServer(handler);
        server.conf()
            .port(3030)
            //.trafficDump(System.out::print)
            ;
        server.start();
    }
}
