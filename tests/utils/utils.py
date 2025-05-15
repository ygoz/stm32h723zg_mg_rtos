import httpx
from functools import wraps
from typing import Callable, Dict


def request_handler(method: str, url: str):
    def decorator(func: Callable):
        @wraps(func)
        async def wrapper(*args, **kwargs):
            # Extract headers separately
            # headers = kwargs.pop('headers', {})

            try:
                async with httpx.AsyncClient() as client:
                    # response = await client.request(method, url, headers=headers, **kwargs)
                    response = await client.request(method, url, **kwargs)
                    response.raise_for_status()
                    print("\n" + "=" * 50)
                    print("📡 HTTP Request Log")
                    print("=" * 50)
                    print(f"🔹 Method      : {method}")
                    print(f"🔹 URL         : {url}")
                    print(f"🔹 Status Code : {response.status_code}")
                    print(f"🔹 Response Body:\n{response.text}")
                    print("=" * 50 + "\n")

                
                # Call the original function with the HTTP response
                return await func(response, *args, **kwargs)
            except httpx.HTTPStatusError as e:
                print(f"\nstatus error:\t {e}\n")
        return wrapper
    return decorator