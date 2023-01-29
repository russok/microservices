use warp::Filter;

#[tokio::main]
async fn main() {
    // GET /add/347248/293898 => 200 OK with plain/text body "347248 + 293898 = 641146"
    let adder = warp::path!("add" / i64 / i64)
        .map(|arg1, arg2| format!("{} + {} = {}\n", arg1, arg2, arg1 + arg2));

    warp::serve(adder)
        .run(([127, 0, 0, 1], 3030))
        .await;
}