import httpx
from time import sleep
from utils.utils import request_handler
from typing import Optional
import asyncio

# Replace with your STM32 HTTP server IP address
  # Change to your actual server IP
data = {"key": 30, "popoo" : "ba"*2}
# STM32_HTTP_SERVER = "http://192.168.1.10"


class api_test_manager:
    _instance: Optional["api_test_manager"] = None
    STM32_HTTP_SERVER = "http://192.168.1.10"

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(api_test_manager, cls).__new__(cls)
        return cls._instance

    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/flash/settings/get")
    async def get_network_settings(response: httpx.Response, *args, **kwargs) -> str:
        return response
    
    @classmethod
    @request_handler("POST", f"{STM32_HTTP_SERVER}/flash/settings/set")
    async def set_network_settings(response: httpx.Response, *args, **kwargs) -> str:
        return response
    
    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/api/led/green/toggle")
    async def toggle_green_led(response: httpx.Response, *args, **kwargs) -> str:
        return response

    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/api/led/green/get") 
    async def get_green_led_status(response: httpx.Response, *args, **kwargs) -> str:
        return response

    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/mem/ram/get")
    async def get_ram(response: httpx.Response, *args, **kwargs) -> str:
        return response


async def main():
    
    # set network settings
    params = {"netmask" : "255.255.255.0", "gateway" : "192.168.1.2", "ip" : "192.168.1.10", "dhcp" : False}
    result = await api_test_manager.set_network_settings(params=params)
    
    # get network settings
    result = await api_test_manager.get_network_settings()
    
    # led
    result = await api_test_manager.toggle_green_led()
    result = await api_test_manager.get_green_led_status()
    
    # mem
    result = await api_test_manager.get_ram()


# if __name__ == '__main__':
    # asyncio.run(main())
