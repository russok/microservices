package com.quark4.app;

import bayou.http.HttpRequest;
import bayou.http.HttpResponse;
import bayou.http.HttpHandler;
import bayou.async.Async;

import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * Add two numbers
 *
 */
public class Adder implements HttpHandler
{
    private Pattern pattern = Pattern.compile("^/add/([+-]?\\d+)/([+-]?\\d+)/?");

    @Override
    public Async<HttpResponse> handle(HttpRequest request)
    {
        Matcher matcher = pattern.matcher(request.uriPath());
        if(!matcher.find())
            return HttpResponse.text(404, "Bad Request: " + request.uriPath());
        long int1 = Long.parseLong(matcher.group(1)), int2 = Long.parseLong(matcher.group(2));
        String text = String.format("%d + %d = %d\n", int1, int2, int1 + int2);
        return HttpResponse.text(200, text);
    }
}
