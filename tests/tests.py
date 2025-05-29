import pytest
from rest_api.api_manager import api_test_manager
from httpx import Response
import asyncio


async def main():
    
    # set network settings
    # params = {"netmask" : "255.255.255.0", "gateway" : "192.168.1.2", "ip" : "192.168.1.10", "dhcp" : False}
    # result = await api_test_manager.set_network_settings(params=params)
    
    # # get network settings
    # response = await api_test_manager.fota_update()
    # print(response)
    # # led
    # result = await api_test_manager.toggle_green_led()
    # result = await api_test_manager.get_green_led_status()
    result = await api_test_manager.get_adc3()
    result = await api_test_manager.get_adc1()


if __name__ == '__main__':
    asyncio.run(main())
