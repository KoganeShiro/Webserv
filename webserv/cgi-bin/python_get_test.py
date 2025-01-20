#!/usr/bin/env python3
import os
import cgi

# Set up the HTTP response
print("Status: 200 OK")  # HTTP status header
print("Content-Type: text/html")  # Content type header
print()  # End of headers

# Start HTML content
print("<html>")
print("<head><title>CGI Environment Variables and GET Request</title></head>")
print("<body>")
print("<h1>CGI Environment Variables and GET Request</h1>")

# Display environment variables
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")

# Display the PATH_INFO (if any)
print("<h2>PATH_INFO</h2>")
path_info = os.environ.get('PATH_INFO', 'No PATH_INFO provided')
print("<pre>")
print(f"PATH_INFO: {path_info}")
print("</pre>")

# Display GET request parameters (if any)
print("<h2>GET Request Parameters</h2>")
form = cgi.FieldStorage()
if len(form) > 0:
    print("<pre>")
    for key in form.keys():
        value = form.getvalue(key)
        print(f"{key}: {value}")
    print("</pre>")
else:
    print("<p>No GET parameters were passed.</p>")

# End HTML content
print("</body>")
print("</html>")
