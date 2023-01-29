"""'Add two numbers' web service."""
from fastapi import FastAPI, Response

app = FastAPI(title="Demo service")


@app.get("/add/{int1}/{int2}")
async def summation(int1: int, int2: int) -> str:
    """Add two numbers."""
    result = f"{int1} + {int2} = {int1 + int2}\n"
    return Response(content=result, media_type="text/plain")

