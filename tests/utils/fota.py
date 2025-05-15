import httpx
import os



class ST_FOTA_manager():
    firmware_file = "rtos_mg.bin"#   # Replace with the actual firmware file path
    total_firmware_size = os.path.getsize(firmware_file)
    
    STM32_HTTP_SERVER = "http://192.168.1.10" 
    URL = f"{STM32_HTTP_SERVER}/api/firmware/upload"  # Replace with the correct server URL
    
    chunk_size = 1024  # Define the chunk size, can be modified based on your needs
    FIRMWARE_UPLOAD_STATUS = 0
    
    @classmethod
    def update_firmware(cls):
        with open(cls.firmware_file, "rb") as firmware_file:
            offset = 0  
            with httpx.Client() as client:
                
                while True:
                    
                    chunk_data = firmware_file.read(cls.chunk_size)
                    if not chunk_data:
                        # send one last empty request to let the ST know we are done :)
                        response = client.post(url=cls.URL, params=params)
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

                    response = client.post(url=cls.URL, params=params, data=chunk_data)
                    if response.status_code == 200:
                        print(f"Chunk uploaded successfully, offset: {offset}")
                    else:
                        print(f"\n\nFailed to upload chunk at offset {offset}, status code: {response.status_code}, reason: {response.text}")
                        break  # Stop the upload if an error occurs
                    
                    offset += len(chunk_data)
                    
    # @classmethod
    # def commit_firmware(cls):
    #     assert (cls.FIRMWARE_UPLOAD_STATUS == 1), "please make sure you uploaded new firmware before commiting it"
    #     response = httpx.post(f"{cls.STM32_HTTP_SERVER}/api/firmware/commit")
    #     cls.FIRMWARE_UPLOAD_STATUS = 0
    #     return response
    
    # @classmethod
    # def ping(cls):
    #     response = httpx.post(f"{cls.STM32_HTTP_SERVER}/api/pingp")
    #     return response
            
                    
                    
if __name__ == '__main__':
    try:
        res = ST_FOTA_manager.update_firmware()
        print(f"\nstatus code = {res.status_code}\ntext = {res.text}")
    except Exception as e:
        print(e)