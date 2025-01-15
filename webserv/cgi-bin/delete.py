import os
import urllib.parse

upload_dir = '/uploads'

if os.environ['REQUEST_METHOD'] == 'DELETE':
    query_string = os.environ.get('QUERY_STRING', '')
    params = urllib.parse.parse_qs(query_string)
    resource = params.get('resource', [''])[0]

    file_path = os.path.join(upload_dir, resource)
    print(f"file_path: {file_path}")
    print(f"resource: {resource}")

    if resource and os.path.exists(file_path):
        try:
            os.remove(file_path)
            print(f"Resource '{resource}' has been deleted successfully.")
        except Exception as e:
            print(f"Failed to delete the resource '{resource}': {e}")
    else:
        print(f"Resource '{resource}' does not exist.")
else:
    print("Invalid request method. Use DELETE.")