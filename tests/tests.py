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
    # for i in range(5):
    params = {"channel" : 0, "value" : -3800}
    result = await api_test_manager.set_dac1(json=params)
    # result = await api_test_manager.get_dts()


if __name__ == '__main__':
    asyncio.run(main())
