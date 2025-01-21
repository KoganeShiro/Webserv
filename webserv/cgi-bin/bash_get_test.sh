#!/bin/bash

# Enable CGI error handling
set -e

# Print HTTP response headers
echo "Status: 200 OK"
echo "Content-Type: text/html"
echo ""

# Start HTML content
echo "<html>"
echo "<head><title>CGI Environment for get request with Bash Script</title></head>"
echo "<body>"
echo "<h1>CGI Environment Variables for get request with Bash Script</h1>"

# Display all environment variables
echo "<h2>Environment Variables</h2>"
echo "<pre>"
env | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g' # Sanitize HTML
echo "</pre>"


# Read the Content-Type header to determine boundary
CONTENT_TYPE=$(echo "$CONTENT_TYPE" | tr -d '\r')
BOUNDARY=$(echo "$CONTENT_TYPE" | sed -n 's/.*boundary=//p')

# End HTML content
echo "</body>"
echo "</html>"