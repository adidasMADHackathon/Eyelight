import requests
import json
import sys

final_url="http://localhost/"

payload = sys.argv
payload.pop(0)
payload = json.dumps(payload)
print(str(payload))
response = requests.post(final_url, data=payload)

print(response.text) #TEXT/HTML
print(response.status_code, response.reason) #HTTP
