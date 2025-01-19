#!/usr/bin/env python3
# import cgi
# import cgitb
# import os
# import json

# cgitb.enable()

# UPLOAD_FOLDER = '../uploads'
# ALLOWED_EXTENSIONS = {'txt', 'md', 'csv', 'log'}

# def allowed_file(filename):
#     return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

# def list_files():
#     files = os.listdir(UPLOAD_FOLDER)
#     return files

# print("Content-Type: application/json\n")

# form = cgi.FieldStorage()
# path_info = os.environ.get('PATH_INFO', '')

# if path_info == '/list':
#     files = list_files()
#     print(json.dumps({'files': files}))
# elif path_info == '/create':
#     filename = form.getvalue('filename')
#     content = form.getvalue('content')
#     if filename and allowed_file(filename):
#         filepath = os.path.join(UPLOAD_FOLDER, filename)
#         with open(filepath, 'w') as f:
#             f.write(content)
#         print(json.dumps({'message': f'File {filename} created successfully'}))
#     else:
#         print(json.dumps({'message': 'Invalid file name or extension'}), 400)
# elif path_info == '/edit':
#     data = json.load(sys.stdin)
#     filename = data.get('filename')
#     content = data.get('content')
#     if filename and allowed_file(filename):
#         filepath = os.path.join(UPLOAD_FOLDER, filename)
#         with open(filepath, 'w') as f:
#             f.write(content)
#         print(json.dumps({'message': f'File {filename} updated successfully'}))
#     else:
#         print(json.dumps({'message': 'Invalid file name or extension'}), 400)
# elif path_info == '/delete':
#     data = json.load(sys.stdin)
#     filename = data.get('filename')
#     if filename and allowed_file(filename):
#         filepath = os.path.join(UPLOAD_FOLDER, filename)
#         if os.path.exists(filepath):
#             os.remove(filepath)
#             print(json.dumps({'message': f'File {filename} deleted successfully'}))
#         else:
#             print(json.dumps({'message': 'File not found'}), 404)
#     else:
#         print(json.dumps({'message': 'Invalid file name or extension'}), 400)
# else:
#     print(json.dumps({'message': 'Invalid action'}), 400)



import cgi
import os
from pathlib import Path

form = cgi.FieldStorage()

filename = form.getvalue("filename")
content = form.getvalue("content")

directory = "./../html_page/www/uploads/"
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
    <a href="/path_to_your_html_file/index.html">Go back to homepage</a>
</body>
</html>
""")