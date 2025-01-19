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