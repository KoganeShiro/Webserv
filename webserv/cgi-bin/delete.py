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