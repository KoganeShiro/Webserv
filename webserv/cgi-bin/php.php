#!/usr/bin/env php
<?php



// Enable error display for debugging
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

// Print HTTP response headers
echo "Status: 200 OK\n";
echo "Content-Type: text/html\n\n";

// Start HTML content
echo "<html>";
echo "<head><title>CGI Environment and File Upload</title></head>";
echo "<body>";
echo "<h1>CGI Environment Variables and File Contents</h1>";

// Display all environment variables
echo "<h2>Environment Variables</h2>";
echo "<pre>";
foreach ($_SERVER as $key => $value) {
    echo htmlspecialchars("$key: $value") . "\n";
}
echo "</pre>";



echo "Content-Type: text/html\n\n";

// Try to read the raw input and output it for debugging
$raw_data = file_get_contents('php://input');
if ($raw_data === false) {
    echo "Unable to read php://input.\n";
} else {
    echo "Raw Data:\n<pre>" . htmlspecialchars($raw_data) . "</pre>\n";
}



$input = fopen('php://input', 'r');
$raw_data = '';
while (!feof($input)) {
    $raw_data .= fread($input, 1024);  // Read in chunks
}
fclose($input);

// Output the raw data for debugging
echo "<pre>";
echo htmlspecialchars($raw_data);  // Print the raw POST data
echo "</pre>";




// Read the raw POST body data from stdin (php://input)
// $raw_data = file_get_contents('php://input');

// Get the boundary from the Content-Type header
$boundary = '';
if (isset($_SERVER['CONTENT_TYPE'])) {
    preg_match('/boundary=([^;]+)/', $_SERVER['CONTENT_TYPE'], $matches);
    if (isset($matches[1])) {
        $boundary = $matches[1];
    }
}

// Split the raw data based on the boundary
$parts = explode('--' . $boundary, $raw_data);

// Parse the parts
foreach ($parts as $part) {
    if (empty($part)) {
        continue;
    }

    // Check if this part contains file data (look for Content-Disposition header)
    if (strpos($part, 'Content-Disposition: form-data;') !== false) {
        // Get the content disposition header
        preg_match('/Content-Disposition: form-data; name="([^"]+)"; filename="([^"]+)"/', $part, $matches);
        
        if (isset($matches[2])) {
            // File part found, extract the filename and content
            $filename = $matches[2];
            $file_data = substr($part, strpos($part, "\r\n\r\n") + 4); // Skip headers to get the file content
            
            // Remove trailing boundary markers
            $file_data = rtrim($file_data, "\r\n");

            echo "<h2>Uploaded File: $filename</h2>";
            echo "<pre>" . htmlspecialchars($file_data) . "</pre>";
        }
    }
}

// End HTML content
echo "</body>";
echo "</html>";
?>
