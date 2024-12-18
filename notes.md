## github commit rules

Use a structured format:
<type>: <subject>
Types:

    feat: New feature
    add: add file
    fix: Bug fix
    docs: Documentation changes
    style: Code style changes
    refactor: Code refactoring
    test: Adding/modifying tests

Subject line rules:

    Use imperative mood (e.g., "Add" not "Added")
    Limit to 50 characters
    Capitalize first letter
    No period at the end

Optional body:

    Separate from subject with a blank line
    Explain the what and why
    Wrap at 72 characters

exemple:
```
feat: Implement basic socket operations

- Create Socket class for TCP connections
- Add methods for binding and listening
- Implement accept() for incoming connections
```

```
feat: Add HTTP request parsing

- Create Request class to represent HTTP requests
- Implement parsing of request headers and body
- Add support for GET and POST methods
```

```
feat: Implement HTTP response generation

- Create Response class for HTTP responses
- Add methods for setting status code and headers
- Implement basic content serving functionality

```

## norm used
snake_case for variable and functions
PascalCase for classes
SCREAMING_SNAKE_CLASS for constant

Avoid line too long
Avoid functions having more than 25 lines long

## TODO

- [ ] Create config_file
- [ ] Parsing of the config_file
- [ ] Fill the struct/class
- [ ] Server listening
- [ ] HTTP/1.1 header parsing (request parsing)
- [ ] Chunked Transfer (request body parsing)
- [ ] Create HTTP pages
- [ ] HTTP error code page (template)
- [ ] CGI (PHP, Python, Ruby...)
- [ ] Implement worker (response)
- [ ] GET, POST, DELETE methods
- [ ] Multiple Server Blocks in one config file
- [ ] Multiple Hosts in one Server Block

## tree
.
└── srcs/
    ├── response/
    │   ├── DeleteMethod.cpp
    │   ├── GetMethod.cpp
    │   └── PostMethod.cpp
    ├── request/
    │   └── request_parsing.cpp
    ├── server/
    │   ├── config_parsing.cpp
    │   ├── Server.cpp
    │   └── Socket.cpp
    ├── utils/
    │   └── error_page_handler.cpp
    ├── cgi/
    │   └── cgi_handler.cpp
    └── main.cpp