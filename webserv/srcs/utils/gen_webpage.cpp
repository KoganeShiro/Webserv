

//create the appropriate error page ?
    //array of the code and the appropriate string (map)
//  OR
//return the appropriate error page to the worker

const std::string &gen_htmlbody();
{
    //if method == GET, check if the file exist else
    //check what status_code and call generate_page
}

std::string gen_htmlbody(const Request& request)
{
    std::string method = request.get_method(); // Get the HTTP method from the request
    std::string path = request.get_path(); // Get the requested path

    // Check if the method is GET
    if (method == "GET") {
        // Check if the requested file exists
        struct stat buffer;   
        if (stat(path.c_str(), &buffer) == 0) {
            // File exists, read its content and return as HTML body
            return read_file_content(path); // Function to read file content into a string
        } else {
            // File does not exist, generate an error page (e.g., 404 Not Found)
            return generate_page(404); // Call generate_page with 404 status code
        }
    }

    // Handle other methods (e.g., POST, DELETE) or return a default response
    return generate_page(405); // Method Not Allowed for unsupported methods
}

// Function to read file content into a string
std::string read_file_content(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return ""; // Return empty string if file cannot be opened
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content; // Return the content of the file
}

std::string generate_page(int status_code)
{
    // Generate appropriate error page based on status code
    // Return Response object with error page content
    std::string error_message = get_status_message(status_code);
    std::string html_template = read_html_file("html_page/template/error/index.html");
    
    // Replace placeholders in the template
    replace_string(html_template, "{{STATUS_CODE}}", std::to_string(status_code));
    replace_string(html_template, "{{ERROR_MESSAGE}}", error_message);
    replace_string(html_template, "{{METHOD}}", std::to_string(status_code));
    
    return (html_template);
}
