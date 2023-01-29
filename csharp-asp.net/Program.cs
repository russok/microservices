var builder = WebApplication.CreateBuilder(args);

var app = builder.Build();

app.MapGet("/add/{int1}/{int2}", async (int int1, int int2) =>
    $"{int1} + {int2} = {int1 + int2}\n");

app.Run();
