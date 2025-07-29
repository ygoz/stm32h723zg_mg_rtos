import pytest
from rest_api.api_manager import api_test_manager
from httpx import Response

    

@pytest.mark.asyncio
async def test_set_fota_update():
    response = await api_test_manager.fota_update()
    print(response)
    assert response.status_code == 200

@pytest.mark.asyncio
async def test_ui():
    response = await api_test_manager.get_ui()
    assert response.status_code == 200


@pytest.mark.asyncio
async def test_set_network_settings():
    params = {"netmask": "255.255.255.0", "gateway": "192.168.1.8", "ip": "192.168.1.10", "dhcp": False}
    response = await api_test_manager.set_network_settings(params=params)
    assert response.status_code == 200
    assert "ok" in response.text.lower()

@pytest.mark.asyncio
async def test_get_network_settings():
    response = await api_test_manager.get_network_settings()
    assert response.status_code == 200
    assert "ip" in response.text.lower() # check for exact response

@pytest.mark.asyncio
async def test_toggle_green_led():
    response = await api_test_manager.toggle_green_led()
    assert response.status_code == 200
    assert "true" in response.text.lower()

@pytest.mark.asyncio
async def test_get_green_led_status():
    response = await api_test_manager.get_green_led_status()
    assert response.status_code == 200
    assert response.text.strip() in {"0", "1"}

@pytest.mark.asyncio
async def test_get_ram():
    response = await api_test_manager.get_ram()
    assert response.status_code == 200
    json_data = response.json()
    assert "free_memory" in json_data and "total_memory" in json_data
    assert json_data["free_memory"] <= json_data["total_memory"]

@pytest.mark.asyncio
async def test_eeprom():
    
    addr = 0x0000
    text = "testing the eeprom"
    
    write_params = {"addr" : addr , "slave" : 0xa0, "text" : text} 
    response = await api_test_manager.write_eeprom(params=write_params)
    assert response.status_code == 200
    
    read_params = {"addr" : addr , "slave" : 0xa0, "size" : len(text)}
    response = await api_test_manager.read_eeprom(params=read_params)
    
    assert response.status_code == 200
    assert text in response.text
    
    
@pytest.mark.asyncio
async def test_adcs():
    response = await api_test_manager.get_adc4()
    assert response.status_code == 404
    response = await api_test_manager.get_adc3()
    assert response.status_code == 200  
    
    
    assert "adc" in response.text
    response = await api_test_manager.get_adc2()
    assert response.status_code == 200
    assert "adc" in response.text
    response = await api_test_manager.get_adc1()
    assert response.status_code == 200
    assert "adc" in response.text
    
    