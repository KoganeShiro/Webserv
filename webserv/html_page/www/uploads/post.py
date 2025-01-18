-----------------------------37915976299330804724270690759
Content-Disposition: form-data; name="fileToUpload"; filename="post.py"
Content-Type: text/x-python

import cgi
import os
from pathlib import Path

form = cgi.FieldStorage()

filename = form.getvalue("filename")
content = form.getvalue("content")

directory = "/POST /uploads HTTP/1.1
Host: localhost:8034
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:131.0) Gecko/20100101 Firefox/131.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Content-Type: multipart/form-data; boundary=---------------------------37915976299330804724270690759
Content-Length: 1358
Origin: http://localhost:8034
Connection: keep-alive
Referer: http://localhost:8034/upload.html
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: same-origin
Sec-Fetch-User: ?1
Priority: u=0, i

-----------------------------37915976299330804724270690759
Content-Disposition: form-data; name="fileToUpload"; filename="post.py"
Content-Type: text/x-python

import cgi
import os
from pathlib import Path

form = cgi.FieldStorage()

filename = form.getvalue("filename")
content = form.getvalue("content")

directory = "/uploads"
filepath = os.path.join(directory, filename)

print("Content-Type: text/html\n")

# Function to save the content to the file
def save_file(filepath, content):
    try:
        with open(filepath, "w") as file:
            file.write(content)
        return f"The file: '{filename}' has been saved successfully!"
    except Exception as e:
        return f"ERROR: '{filename}' couldn't be saved. {str(e)}"

# Response value
if file.exists():
    value = save_file(filepath, content)
else:
    if not os.path.exists(directory):
        os.makedirs(directory)
    value = save_file(filepath, content)

# Output the response
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>File Management</title>
</head>
<body>
    <p>{value}</p>
    <a href="/index.html">Go back to homepage</a>
</body>
</html>
""")
-----------------------------37915976299330804724270690759
Content-Disposition: form-data; name="submit"

Upload File
-----------------------------37915976299330804724270690759--