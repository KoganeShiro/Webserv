#!/usr/bin/env python3
import os
import cgi
import cgitb

# Enable CGI error handling
cgitb.enable()

# Print HTTP response headers
print("Status: 200 OK")
print("Content-Type: text/html")
print()

# Start HTML content
print("<html>")
print("<head><title>CGI Environment for get request with python script</title></head>")
print("<body>")
print("<h1>CGI Environment Variables for get request with python script</h1>")

# Display all environment variables
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")


# End HTML content
print("</body>")
print("</html>")