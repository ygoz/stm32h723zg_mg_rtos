import httpx
from httpx import TimeoutException, TransportError
from time import sleep
from rest_api.utils import request_handler
# from utils import request_handler
from typing import Optional
import asyncio
from rest_api.fota import ST_FOTA_manager






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
    
    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/api/ping")
    async def ping_st(response: httpx.Response, *args, **kwargs) -> str:
        return response
    
    @classmethod
    @request_handler("GET", f"{STM32_HTTP_SERVER}/index.html")
    async def get_ui(response: httpx.Response, *args, **kwargs) -> str:
        return response
    
    
    @classmethod
    async def fota_update(cls) -> str:
        try:
            # Try firmware update, request should timeout / socket disconnected
            response = ST_FOTA_manager.update_firmware(URL=f"{cls.STM32_HTTP_SERVER}/api/firmware/upload")
    
        except (TimeoutException, TransportError):
            # test ping after st reboots
            print("[INFO] Firmware update timed out as expected.")
            try:
                sleep(8) # st is booting...
                response = await cls.ping_st()
                return response
            
            except Exception as e:
                print(f"unexpected error {e}")

        except Exception as e:
            print(f"unexpected error {e}")


async def main():
    
    # set network settings
    # params = {"netmask" : "255.255.255.0", "gateway" : "192.168.1.2", "ip" : "192.168.1.10", "dhcp" : False}
    # result = await api_test_manager.set_network_settings(params=params)
    
    # # get network settings
    # result = await api_test_manager.get_network_settings()
    
    # # led
    # result = await api_test_manager.toggle_green_led()
    # result = await api_test_manager.get_green_led_status()
    
    # mem
    result = await api_test_manager.get_ui()


if __name__ == '__main__':
    asyncio.run(main())
