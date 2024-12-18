# Webserv

# Table of Contents
1. [Rules](#rules)
2. [What is a Web Server ?](#what-is-a-web-server)
3. [HTTP](#http)
5. [Pseudo code](#pseudo-code)
4. [Ressources](#ressources-on-webserv)

## Rules
**Summary**:
    No crashing: The server must not crash, even if it runs out of memory.
    Makefile: Required for building the project.
    Library Restrictions: External and Boost libraries are forbidden.
    Allowed Functions: Use only specified C++98 functions and system calls.
    Executable: Write an HTTP server in C++98, executable with `./webserv [configuration file (or default path)]`.
    Pre-requisites: Read the RFC, test with telnet and NGINX.
**Constraints**:
     Do not execve another web server.
     Must be non-blocking; use only one poll() (or equivalent) for all I/O operations.
	 Must not read/write without poll().
     Must check read and write simultaneously with poll().
     No checking errno after read/write operations.
     Must be compatible with a web browser.
     Accurate HTTP response status codes.
     Default error pages if none are provided.
     Use fork only for CGI.
     Serve static websites.
     Support GET, POST, DELETE methods.
     Handle file uploads.
     Stress test to ensure availability.
     Listen on multiple ports (configured via file).

**Configuration File**:
    Define port and host for each server.
    Optionally set server names.
    Set default server for host:port.
    Default error pages and client body size limits.
**Route configuration**:
     Accepted HTTP methods.
     HTTP redirection.
     Directory/file roots.
     Directory listing toggle.
     Default file for directories.
     Execute CGI based on file extension.
     Handle POST and GET for CGI.
     Accept file uploads and configure save location.

### Useful functions
    Process and Execution Management:
       `execve`, `fork`, `waitpid`, `kill`, `signal`: Useful for creating and managing processes, especially for handling CGI scripts.

    File and Directory Operations:
       `open`, `close`, `read`, `write`, `stat`, `opendir`, `readdir`, `closedir`, `access`: Essential for handling file I/O operations, serving static files, and managing directory listings.

    Network Operations:
       `socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`, `setsockopt`, `getsockname`, `getaddrinfo`, `freeaddrinfo`, `socketpair`, `htons`, `htonl`, `ntohs`, `ntohl`: Crucial for setting up server sockets, accepting client connections, and handling network communication.

    Error Handling:
       `strerror`, gai_strerror, errno: Helpful for interpreting error codes and handling errors gracefully.

    Multiplexing and I/O Management:
       `select`, `poll`, `epoll` (epoll_create, epoll_ctl, epoll_wait), `kqueue` (kqueue, kevent), `dup`, `dup2`, `pipe`: Necessary for implementing non-blocking I/O and managing multiple client connections efficiently.

    Utility Functions:
       `fcntl`: Useful for manipulating file descriptors, such as setting non-blocking modes.
       `chdir`: Changes the current working directory, useful for CGI execution.
       `getprotobyname`: Retrieves protocol information, useful for setting up sockets.


## What is a Web Server
### Definition
A web server is a computer system or software that hosts websites and delivers web pages to users over the internet or an intranet. It handles incoming requests from clients (such as browsers) and serves them the requested web pages.

### Why is a web server important?
Web servers are essential because they make it possible to access websites and web applications. Without web servers, the internet as we know it would not function, as there would be no way to deliver web content to users.

### How does a web server work?
A web server works by accepting requests from clients (typically web browsers) and responding with the appropriate web pages or resources. When you type a URL into your browser, the browser sends a request to the web server, which processes the request and sends back the requested HTML page, images, or other files.

### In Summary
A web server is a critical component of the internet, responsible for delivering web content to users. It is essential for anyone involved in web development or IT to understand how web servers operate to create and manage web-based services.

### Other Notes
Web servers can be software (like Apache, Nginx, or IIS) or dedicated hardware. They can serve static content (like HTML files) or dynamic content (generated by web applications).

### Resources
- [Mozilla Developer Network (MDN) - Introduction to web servers](https://developer.mozilla.org/en-US/docs/Learn/Common_questions/What_is_a_web_server)
- [Wikipedia - Web server](https://en.wikipedia.org/wiki/Web_server)
- [How Web Servers Work - HowStuffWorks](https://computer.howstuffworks.com/web-server.htm)

## HTTP
### Definition
HTTP (Hypertext Transfer Protocol) is the foundation of any data exchange on the Web and is a protocol used for transmitting hypertext requests and information between servers and browsers.

### How does HTTP work?
HTTP works as a request-response protocol between a client (browser) and a server. When you enter a URL in your browser, it sends an HTTP request to the server, which then processes the request and sends back the requested web page or resource in an HTTP response.

### In Summary
HTTP is a vital protocol that facilitates communication and data exchange on the web, making it possible for users to access and interact with web content.

### Other Notes
- HTTP is stateless, meaning each request is independent and does not retain information about previous interactions.
- HTTPS is a secure version of HTTP, which encrypts data to protect it during transmission.

### Resources
- [Mozilla Developer Network (MDN) - Introduction to HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)
- [Wikipedia - Hypertext Transfer Protocol](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol)
- [HTTP Methods](https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods)
- [starck overflow](https://stackoverflow.com/questions/60128646/http-in-simple-terms)


## Pseudo Code
Input parser: handles read
Worker: handles responses

in all messages : 
track session and client ID

Start Server
Parse Config file
Start Error Log process
Check Memory Checking process 
Open several sockets: one for each server (host:port), and a default one
Listen on all sockets
    select/poll to check if something happens on sockets and create new session
    fill buffer from sockets and check if the buffer contains indications for end of header /r/n/r/n OR header too long OR timeout 
    if indication that end of header is reached , send the first part of buffer (till end of header) to the input-parser, keep 
        the rest of the buffer - and put this session on hold

    input parser: check the "type of message" (GET POST DELETE...) and parse the header
         -> if not compliant: send error request to WORKER (send errorpage and ask socket to close session)
         -> if compliant and body is not expected : send parsed request to WORKER (should send page/file) and inform SOCKET to resume session
         -> if compliant and body is expected : ask Socket for the body (indicating length of body)
               -> if body is compliant, parse it and send request to worker
               -> if not: send error request to WORKER (send errorpage and ask socket to close session)

Worker:
    
    manage queue 
        manage responses from messqges in queue-> error: Prepare errorcode and erropage ? output parse message...-> GET : output parse message...

                error (no permission)/ page not found etc.

                    static page found : OK 200 + header + page

                    dynamic page ?: call cgi ?

                -> POST : output parse message...

                    error (no permission)/ page not found etc.

                    standard Post : save page...

                    call CGI : OK 200 + header + page


        -> DELETE : output parse message...
            

        error (no permission)/ page not found etc.

            standard Post : save page...

            call CGI : OK 200 + header + page


CGI: 
    need to dig...


## Ressources on Webserv

- [List of HTTP status code](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)
- [CGI](https://stackoverflow.com/questions/2089271/what-is-common-gateway-interface-cgi)

- [Request, Response](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)

- [medium articles on non blocking webserv](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7)
- [medium general article on a cpp webserv](https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7)
- [french article](https://hackmd.io/@fttranscendance/H1mLWxbr_)
- [Programmation reseau via socket](https://www.codequoi.com/programmation-reseau-via-socket-en-c/)
- https://github.com/Kaydooo/Webserv_42
- https://github.com/cclaude42/webserv
