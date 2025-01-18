-----------------------------281335630533533934813239005319
Content-Disposition: form-data; name="fileToUpload"; filename="delete.py"
Content-Type: text/x-python

from flask import Flask, request, jsonify
import os

app = Flask(__name__)

UPLOAD_DIRECTORY = "/uploads"

@app.route('/uploads', methods=['GET'])
def list_fPOST /uploads HTTP/1.1
Host: localhost:8034
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:131.0) Gecko/20100101 Firefox/131.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Content-Type: multipart/form-data; boundary=---------------------------281335630533533934813239005319
Content-Length: 1252
Origin: http://localhost:8034
Connection: keep-alive
Referer: http://localhost:8034/upload.html
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: same-origin
Sec-Fetch-User: ?1
Priority: u=0, i

-----------------------------281335630533533934813239005319
Content-Disposition: form-data; name="fileToUpload"; filename="delete.py"
Content-Type: text/x-python

from flask import Flask, request, jsonify
import os

app = Flask(__name__)

UPLOAD_DIRECTORY = "/uploads"

@app.route('/uploads', methods=['GET'])
def list_files():
    try:
        files = os.listdir(UPLOAD_DIRECTORY)
        return jsonify(files), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/uploads', methods=['DELETE'])
def delete_file():
    requested_path = request.args.get('file')
    full_path = os.path.join(UPLOAD_DIRECTORY, requested_path.lstrip('/'))
    try:
        if os.path.exists(full_path):
            os.remove(full_path)
            return jsonify({'message': 'File deleted successfully'}), 200
        else:
            return jsonify({'message': 'File not found'}), 404
    except Exception as e:
        return jsonify({'message': f'Error deleting file: {str(e)}')}), 500

if __name__ == '__main__':
    app.run(debug=True)
-----------------------------281335630533533934813239005319
Content-Disposition: form-data; name="submit"

Upload File
-----------------------------281335630533533934813239005319--