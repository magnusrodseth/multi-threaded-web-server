.PHONY: app compile run

app: compile run $(serve) $(port) $(worker_threads_count) $(buffer_slots)

compile:
	@ gcc src/app.c src/bbuffer.c src/command_args.c src/logger.c \
		src/mtwwwd.c src/sem.c src/server.c src/thread_pool.c \
		src/worker.c src/file_parser.c -o mtwwwd

run:
	@ ./mtwwwd $(serve) $(port) $(worker_threads_count) $(buffer_slots)

help:
	@ echo "Usage    :    make <target>"
	@ echo "Targets  :"
	@ echo "    app - Compile and run the application."
	@ echo ""
	@ echo "\tThe 'app' target takes 4 arguments: serve, port, worker_threads_count, and buffer_slots."
	@ echo "\tserve - the directory that the server should base on when serving files."
	@ echo "\tport - the port that the server should run on."
	@ echo "\tworker_threads_count - the number of worker threads to handle requests."
	@ echo "\tbuffer_slots - the number of slots in the bounded buffer."
	@ echo ""
	@ echo "    compile - Compile the application."
	@ echo ""
	@ echo "    run - Run the application."



