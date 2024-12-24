import cgi
import os
import cgitb

cgitb.enable()

upload_dir = './../html_page/www/uploads/'

form = cgi.FieldStorage()

file_item = form['file']

print("Content-Type: text/html; charset=utf-8")
print()

if file_item.filename:
    filename = os.path.basename(file_item.filename)
    filepath = os.path.join(upload_dir, filename)

    try:
        with open(filepath, 'wb') as output_file:
            while True:
                chunk = file_item.file.read(1024)
                if not chunk:
                    break
                output_file.write(chunk)

        value = f"'{filename}' is upload to '{upload_dir}'"
    except Exception as e:
        value = f"ERROR when uploading : {e}"
else:
    value = "No file"

