# Rust

Once Linus Torvalds blesses something, that thing is destined to achieve world domination - exemplified by Linux and Git. Now Rust has received the royal nod - it has been accepted as another language for Linux kernel development. Efforts are underway to rewrite everything in Rust. I will talk about how Rust is different and similar to other mainstream languages. Then I compare implementation of a simple microservice in Python, Javascript, C++, C#, Java, and Rust, and race them against each other to compare their performance.


## Rust rationales

 - Type safety
 - Value safety!
 - Simplicity?


## How?

Take some C++ ideas

 - Const correctness
 - Scopes, RAII, smart pointers
 - Lifetime management

but

 - Remove exceptions - pervasively use variant<> and optional<>, with some syntactic sugar added
 - Remove template metaprogramming, keep parameterized types
 - Add stronger (and more complicated) preprocessor
 - No null pointers
 - Add a lot of love and care for the users: package repository, tools


## Microservice speed test

 - Asynchronous web servers - support 10k connections (unfinished requests)
 - Python, NodeJS, C++, C#, Java, Rust
 - Note the lack of language support for asynchronicity in Java
 - The example service is absolutely minimal, is async (controlled by the event loop), but does not use the asynchronicity.
   That is to measure only the speed of the server and not the speed of dependent services, json serializers, etc


## Speed test results

Roughly ordered by the amount of RAM consumed

|   VIRT |   RES |   SHR | %CPU | %MEM  |Rate| COMMAND                                                     |
|--------|-------|-------|------|-------|----|-------------------------------------------------------------|
| 817708 |  3244 |  2940 |155.0 |  0.0  | 75k| target/release/warp_server                                  |
|   8272 |  4668 |  4180 | 99.7 |  0.0  | 47k| ./boost-beast 0.0.0.0 3030 1                               |
|  81504 |  4360 |  4072 |200.0 |  0.0  | 58k| ./boost-beast 0.0.0.0 3030 2                               |
| 155228 |  4404 |  4120 |295.7 |  0.0  | 72k| ./boost-beast 0.0.0.0 3030 3                               |
| 150040 | 37876 | 16112 |100.0 |  0.2  |  9k| .venv/bin/python3.8 .venv/bin/uvicorn demo_server:app       |
| 152896 | 41496 | 17348 |100.0 |  0.3  | 11k| .venv/bin/python3.11 .venv/bin/uvicorn demo_server:app      |
| 619840 | 77376 | 30088 |106.0 |  0.5  | 10k| node index.js                                               |
|6450576 |134620 | 27484 |100.3 |  0.8  | 61k| java -XX:ActiveProcessorCount=1 -jar bayou-microservice.jar |
|8438220 |270052 | 28140 |233.6 |  1.7  | 69k| java -jar bayou-microservice.jar                            |
| 263.1g |253468 | 64360 |327.9 |  1.6  | 62k| ./bin/release/net7.0/asp.net                                |
