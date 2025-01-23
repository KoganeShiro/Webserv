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
    # Create a temporary file to store the uploaded content
    TEMP_FILE=$(mktemp)
    cat > "$TEMP_FILE"

    # Extract filename and file content
    FILENAME=$(sed -n 's/.*filename="\([^"]*\)".*/\1/p' "$TEMP_FILE")
    START_LINE=$(grep -n -m1 '^$' "$TEMP_FILE" | cut -d ':' -f1)
    START_LINE=$((START_LINE + 1))
    END_LINE=$(($(wc -l < "$TEMP_FILE") - 1))
    
    if [ -n "$FILENAME" ]; then
        echo "<p>Uploaded file: $FILENAME</p>"
        echo "<pre>"
        sed -n "${START_LINE},${END_LINE}p" "$TEMP_FILE" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g' # Sanitize HTML
        echo "</pre>"

        # Create the file
        UPLOAD_DIR="./../html_page/www/uploads"
        mkdir -p "$UPLOAD_DIR"
        SAFE_FILENAME=$(basename "$FILENAME")
        FILE_PATH="$UPLOAD_DIR/$SAFE_FILENAME"
        sed -n "${START_LINE},${END_LINE}p" "$TEMP_FILE" > "$FILE_PATH"
        echo "<p>File created successfully at: $FILE_PATH</p>"
    else
        echo "<p>No file content found or filename is missing.</p>"
    fi

    # Clean up temporary file
    rm "$TEMP_FILE"
fi

# End HTML content
echo "</body>"
echo "</html>"
