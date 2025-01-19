import cgi
import os
import cgitb

cgitb.enable()

upload_dir = '/uploads'

form = cgi.FieldStorage()

# Check if the file field is present in the form
if 'file' in form:
    file_item = form['file']

    # Check if the file item is actually a file
    if file_item.filename:
        # Strip leading path from file name to avoid directory traversal attacks
        filename = os.path.basename(file_item.filename)
        filepath = os.path.join(upload_dir, filename)

        # Write the file to the specified directory
        with open(filepath, 'wb') as f:
            f.write(file_item.file.read())

        message = f'The file "{filename}" was uploaded successfully.'
    else:
        message = 'No file was uploaded.'
else:
    message = 'No file field was found in the form.'

# Print the HTTP headers and the response message
print("Content-Type: text/html")
print()
print(f"<html><body><h1>{message}</h1>")
print(f'<a href="/index.html">Go back to homepage</a></body></html>')