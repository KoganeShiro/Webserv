<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Delete Method</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            margin: 0;
            padding: 0;
        }

        .container {
            max-width: 800px;
            margin: 40px auto;
            padding: 20px;
            background-color: #fff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            border-radius: 8px;
        }

        h1 {
            color: #2c3e50;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            margin-bottom: 20px;
        }

        th, td {
            text-align: left;
            padding: 10px;
            border-bottom: 1px solid #ddd;
        }

        th {
            background-color: #f2f2f2;
        }

        .file-name {
            font-weight: bold;
            color: #2c3e50;
            font-size: 1.1em;
            padding: 5px;
            border-radius: 5px;
        }

        .delete-btn {
            color: #e74c3c;
            cursor: pointer;
            text-decoration: none;
        }

        .delete-btn:hover {
            text-decoration: underline;
        }

        .response-message {
            margin-top: 20px;
            font-weight: bold;
        }

        .back-link {
            display: inline-block;
            margin-top: 40px;
            color: #3498db;
            text-decoration: none;
        }

        .back-link:hover {
            text-decoration: underline;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Files you can remove...</h1>
        <p>Below is the list of files in the uploads directory. You can delete any file by clicking the delete link.</p>
        <table id="file-list">
            <thead>
                <tr>
                    <th>File Name</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td class="file-name">test-copy.txt</td>
                    <td><a onclick="deleteResource('test-copy.txt')" class="delete-btn">Delete</a></td>
                </tr>
                <tr>
                    <td class="file-name">test.txt</td>
                    <td><a onclick="deleteResource('test.txt')" class="delete-btn">Delete</a></td>
                </tr>
                <tr>
                    <td class="file-name">pokeball.png</td>
                    <td><a onclick="deleteResource('pokeball.png')" class="delete-btn">Delete</a></td>
                </tr>
                <tr>
                    <td class="file-name">inception.png</td>
                    <td><a onclick="deleteResource('inception.png')" class="delete-btn">Delete</a></td>
                </tr>
            </tbody>
        </table>
        <div id="responseMessage" class="response-message"></div>
        <a href="./index.html" class="back-link">Go back to homepage</a>
    </div>

    <script>
        async function deleteResource(fileName) {
            try {
                const response = await fetch('./../../cgi-bin/delete.py?file=' + encodeURIComponent(fileName), {
                    method: 'DELETE'
                });
                const data = await response.json();
                const responseMessage = document.getElementById('responseMessage');

                if (response.status !== 200) {
                    responseMessage.innerText = data.error;
                } else {
                    responseMessage.innerText = data.message;
                    document.querySelector(`a[onclick="deleteResource('${fileName}')"]`).parentElement.parentElement.remove();
                }
            } catch (error) {
                console.error('Error:', error);
            }
        }
    </script>
</body>
</html>