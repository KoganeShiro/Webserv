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
print("<head><title>CGI Environment and File Upload with python script</title></head>")
print("<body>")
print("<h1>CGI Environment Variables and File Contents with python script</h1>")

# Display all environment variables
print("<h2>Environment Variables</h2>")
print("<pre>")
for key, value in os.environ.items():
    print(f"{key}: {value}")
print("</pre>")

# Handle POST data (including file uploads)
print("<h2>Uploaded File Contents (if any)</h2>")
form = cgi.FieldStorage()
if "file" in form:
    file_item = form["file"]
    if file_item.file and file_item.filename:
        print(f"<p>Uploaded file: {file_item.filename}</p>")
        print("<pre>")
        try:
            # Read and display the contents of the uploaded file
            file_contents = file_item.file.read().decode("utf-8")
            print(file_contents)
        except Exception as e:
            print(f"Error reading file: {e}")
        print("</pre>")
    else:
        print("<p>No file was uploaded or file is empty.</p>")
else:
    print("<p>No file was uploaded.</p>")

# End HTML content
print("</body>")
print("</html>")