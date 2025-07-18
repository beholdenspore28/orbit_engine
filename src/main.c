#include "engine.h"

int main() {
  engine_start_x11();
  engine_run();
  engine_stop_x11();
}
