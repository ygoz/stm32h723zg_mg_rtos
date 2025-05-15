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
        """
        Performs a firmware-over-the-air (FOTA) update on the STM32 device to test the update process.

        This function locates the `.bin` firmware file and sends it to the STM32 device in binary chunks
        over a REST API. The firmware is uploaded in parts to the specified `URL`, and once all chunks 
        are successfully sent, an empty chunk is transmitted to indicate completion.

        Upon receiving the empty chunk, the STM32 device is expected to:
        - Confirm full firmware reception
        - Reboot the system
        - Boot from the new firmware

        Args:
            URL (str): The REST API endpoint on the STM32 device that handles firmware uploads (e.g. `/api/firmware/upload`).

        Returns:
            None or response object: Depending on implementation, may return the final HTTP response or nothing.

        Notes:
            This function is intended for testing the FOTA functionality on the embedded STM32 device.
        """
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