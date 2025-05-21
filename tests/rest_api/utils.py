import httpx
from functools import wraps
from typing import Callable, Dict
import os

def request_handler(method: str, url: str) -> httpx.Response:
    def decorator(func: Callable):
        @wraps(func)
        async def wrapper(*args, **kwargs):
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
                # print(f"\nstatus error:\t {e}\n")
                response = e.response  # Get the actual response from the exception
                print("\n" + "=" * 50)
                print("❗ HTTP Status Error")
                print("=" * 50)
                print(f"🔹 Method      : {method}")
                print(f"🔹 URL         : {url}")
                print(f"🔹 Status Code : {response.status_code}")
                print(f"🔹 Response Body:\n{response.text}")
                print("=" * 50 + "\n")
                return await func(response, *args, **kwargs)
        return wrapper
    return decorator



def find_bin_file_path(filename) -> str:
    '''
    Search inside tests directory for bin file and return its path
    '''
    search_dir = os.getcwd() 
    for root, dirs, files in os.walk(search_dir):
        if filename in files:
            return os.path.join(root, filename)
    return None