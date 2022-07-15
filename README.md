# Multi-threaded web server

## What is it?

This project is a multi-threaded web server able to serve static content,
written in C.

## Demo

![Demo](./media/demo.gif)

In the demo above, you can see the running multi-threaded web server on the left and a browser instance on the right. 

The web server logs running worker threads, when a producer adds the file descriptor to the bounded buffer, and when a consumer extracts a file descriptor from the bounded buffer, etc... The web server also logs if the client tries to access a non-existent static site.

The browser instance simply displays the HTML content that the web server hosts.

## Running the application

### With `make`

```sh
# Navigate to the project directory
cd assignment_2

# Compile and run application
make serve=DIRECTORY_TO_SERVE port=PORT worker_threads_count=WORKER_THREADS_COUNT buffer_slots=BUFFER_SLOTS_COUNT

# Example. Ensure that the directory to serve exists.
make serve="static" port=8000 worker_threads_count=5 buffer_slots=10

# See all 'make' targets
make help
```

### Without `make`

```sh
# Navigate to the project directory
cd assignment_2

# Compile
gcc src/app.c src/bbuffer.c src/command_args.c src/logger.c \
		src/mtwwwd.c src/sem.c src/server.c src/thread_pool.c \
		src/worker.c src/file_parser.c -o mtwwwd

# Run
./mtwwwd directory_to_serve port worker_threads_count buffer_slots

# Example. Ensure that the directory to serve exists.
./mtwwwd "static" 8000 5 10
```

## Security problems and fixes

The web server allows not only to access files under the `directory_to_serve`
directory, but enables access to **all** files in the file system of the server
that can be accessed by the client who started the web server process.

Find a way to exploit this security problem. Then, find **two different ways**
to prevent access to files outside of the `directory_to_serve` directory.

### Exploit

In order to exploit this security problem, we need to try to access files
outside the `directory_to_serve` directory.

Using Google Chrome, the browser stops you from exploiting this problem by
simply sending you to the file past the last `/`. For instance, trying to access
`localhost:8000/../../index.html` results in `localhost:8000/index.html`. Hence,
we need to exploit this another way.

We can use Netcat in the terminal. Typing `nc localhost 8000` will use Netcat to
connect to `localhost:8000`. From here, we can type `GET /index.html html/1.1`
to get the `index.html` page in the `directory_to_serve` directory. This also
means that we can exploit the problem by typing `GET /../../index.html`, and
successfully accessing some file outside `directory_to_serve`.

### Fixing the problem

#### (1) A naivë approach

A naivë solution would be to parse the client inputted path, and simply keep the
file at the end of the complete file path. That way, if a client tries to access
`index.html`, there would be no problem. If a client tries to access
`../../index.html`, the client would be re-routed to `index.html`.

#### (2) A more sophisticated approach

A more sophisticated approach would be to keep track of the number of
directories the client navigates up and down when parsing the inputted file
path. The moment we detect that the number of directories up from the
`directory_to_serve` is greater than the number of directories down from
`directory_to_serve`, we have an illegal path.

Please see [./src/file_parser.c](./src/file_parser.c) for implementation of this
approach, in particular the `is_illegal_path` function.
