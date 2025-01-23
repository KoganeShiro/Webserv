#!/bin/bash

# Enable CGI error handling
set -e

# Print HTTP response headers
echo "Status: 200 OK"
echo "Content-Type: text/html"
echo ""

# Start HTML content
echo "<html>"
echo "<head><title>CGI Environment and File Upload with Bash Script</title></head>"
echo "<body>"
echo "<h1>CGI Environment Variables and File Contents with Bash Script</h1>"

# Display all environment variables
echo "<h2>Environment Variables</h2>"
echo "<pre>"
env | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g' # Sanitize HTML
echo "</pre>"

# Handle POST data (including file uploads)
echo "<h2>Uploaded File Contents (if any)</h2>"

# Read the Content-Type header to determine boundary
CONTENT_TYPE=$(echo "$CONTENT_TYPE" | tr -d '\r')
BOUNDARY=$(echo "$CONTENT_TYPE" | sed -n 's/.*boundary=//p')

if [ -z "$BOUNDARY" ]; then
    echo "<p>No file was uploaded or invalid request format.</p>"
else
    # Read and process the multipart/form-data content
    BODY=$(cat)
    FILE_CONTENT=$(echo "$BODY" | awk -v boundary="--$BOUNDARY" '
    BEGIN { in_file = 0 }
    {
        if (index($0, boundary) == 1) {
            in_file = 0;
        } else if ($0 ~ /Content-Disposition: form-data;.*filename="[^"]+"/) {
            in_file = 1;
        } else if (in_file) {
            print;
        }
    }')

    if [ -n "$FILE_CONTENT" ]; then
        echo "<pre>"
        echo "$FILE_CONTENT" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g' # Sanitize HTML
        echo "</pre>"
    else
        echo "<p>No file content found.</p>"
    fi
fi

# End HTML content
echo "</body>"
echo "</html>"