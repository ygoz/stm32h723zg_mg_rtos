import httpx
from time import sleep

# Replace with your STM32 HTTP server IP address
STM32_HTTP_SERVER = "http://192.168.1.10"  # Change to your actual server IP
data = {"key": 30, "popoo" : "ba"*2}


if __name__ == '__main__':
    try:
        # for i in range(3000):
        #     sleep(0.2)
        # Send GET request to /index.html
        # response = httpx.get(f"{STM32_HTTP_SERVER}/index.html")
        
        
        response = httpx.get(f"{STM32_HTTP_SERVER}/flash/settings/get")
        
        
        # query_params = {"netmask" : "255.255.255.0", "gateway" : "192.168.1.2", "ip" : "192.168.1.10", "dhcp" : False}
        # response = httpx.post(
        #     f"{STM32_HTTP_SERVER}/flash/settings/set",
        #     params=query_params
        # )

        # Print the response
        # if response.status_code == 200:
            # print("Response received successfully!")
            # print("Response content:")
            # print(f"round {i}, res: {response.text}")
        print(f"status code {response.status_code}")
        print(response.text)  # Print HTML content
    # else:
    except httpx.ReadTimeout as e:
        # print(f"lasted {i - 1} reqs, {e}")
        print(f"lasted {0} reqs, {e}")
        
    except httpx.ConnectTimeout as e:
        print(f"FELL before the handshake: HTTPSERVER.C, {e}")