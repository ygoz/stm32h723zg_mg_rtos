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
    # params = {"channel" : 0, "value" : -3800}
    # result = await api_test_manager.get_comp1()
    # addr = 0x0000
    # text = "TED the POOPOO son :)"
    
    # write_params = {"addr" : addr , "slave" : 0xa0, "text" : text} 
    # response = await api_test_manager.write_eeprom(params=write_params)
    
    # read_params = {"addr" : addr , "slave" : 0xa0, "size" : len(text)+10}
    # response = await api_test_manager.read_eeprom(params=read_params)
    # response = await api_test_manager.fota_update()
    response = await api_test_manager.get_hil_serial_comm()


if __name__ == '__main__':
    asyncio.run(main())
