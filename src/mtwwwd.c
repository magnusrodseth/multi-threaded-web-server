#include "../include/app.h"
#include "../include/logger.h"
#include "../include/server.h"

int main(int argc, char **argv) {

  command_args_t command_args = parse_command_args(argc, argv);
  app_t *app = initialize_app(command_args);
  run_app(app, command_args);
  destroy_app(app);

  return 0;
}
