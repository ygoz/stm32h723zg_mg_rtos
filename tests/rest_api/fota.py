import httpx
import os
from rest_api.utils import find_bin_file_path

URL = f"http://192.168.1.10/api/firmware/upload"



class ST_FOTA_manager():
    firmware_file = "rtos_mg.bin"
    file_path = find_bin_file_path(firmware_file)
    total_firmware_size = os.path.getsize(file_path)
    
    chunk_size = 1024  # Define the chunk size, can be modified based on your needs
    FIRMWARE_UPLOAD_STATUS = 0
    
    @classmethod
    def update_firmware(cls, URL):
        with open(cls.file_path, "rb") as firmware_file:
            offset = 0  
            with httpx.Client() as client:
                
                while True:
                    
                    chunk_data = firmware_file.read(cls.chunk_size)
                    if not chunk_data:
                        # send one last empty request to let the ST know we are done :)
                        response = client.post(url=URL, params=params)
                        print("Firmware upload completed!")
                        cls.FIRMWARE_UPLOAD_STATUS = 1
                        return response
                        break

                    # Set up the query parameters for the request
                    params = {
                        "name": f"{firmware_file.name}",
                        "offset": str(offset),
                        "total": str(cls.total_firmware_size),
                    }

                    response = client.post(url=URL, params=params, data=chunk_data)
                    if response.status_code == 200:
                        print(f"Chunk uploaded successfully, offset: {offset}")
                    else:
                        print(f"\n\nFailed to upload chunk at offset {offset}, status code: {response.status_code}, reason: {response.text}")
                        break  # Stop the upload if an error occurs
                    
                    offset += len(chunk_data)
                    

                    

                    
                    
if __name__ == '__main__':
    try:
        res = ST_FOTA_manager.update_firmware(URL=URL)
    except Exception as e:
        print(e)