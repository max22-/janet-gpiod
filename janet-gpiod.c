#include <janet.h>
#include <gpiod.h>

static Janet chip_open(int32_t argc, Janet *argv) {
  const char *chipname;
  struct gpiod_chip *chip;
  janet_fixarity(argc, 1);
  chipname = janet_unwrap_string(argv[0]);
  chip = gpiod_chip_open_by_name(chipname);
  if(!chip)
    janet_panic("Failed to open chip");
  return janet_wrap_pointer(chip);
}

static Janet get_line(int32_t argc, Janet *argv) {
  struct gpiod_chip *chip;
  struct gpiod_line *line;
  int line_num;
  janet_fixarity(argc, 2);
  chip = janet_unwrap_pointer(argv[0]);
  line_num = janet_unwrap_integer(argv[1]);
  line = gpiod_chip_get_line(chip, line_num);
  if (!line)
    janet_panic("Failed to get line");
  return janet_wrap_pointer(line);
}

static Janet request_output(int argc, Janet *argv) {
  struct gpiod_line *line;
  const char *consumer = "janet";
  int initial_value = 0;
  janet_arity(argc, 1, 3);
  line = janet_unwrap_pointer(argv[0]);
  switch(argc) {
  case 1:
    break;
  case 2:
    consumer = janet_unwrap_string(argv[1]);;
    break;
  case 3:
    initial_value = janet_unwrap_integer(argv[2]);
    break;
  default:
    janet_panic("unreachable");
  }
  if(gpiod_line_request_output(line, consumer, initial_value) != 0)
    janet_panic("Failed to request line for output");
  return janet_wrap_nil();
}

static Janet request_input(int argc, Janet *argv) {
  struct gpiod_line *line;
  const char *consumer = "janet";
  janet_arity(argc, 1, 2);
  line = janet_unwrap_pointer(argv[0]);
  if(argc == 2)
    consumer = janet_unwrap_string(argv[1]);;
  if(gpiod_line_request_input(line, consumer) != 0)
    janet_panic("Failed to request line for input");
  return janet_wrap_nil();
}

static Janet set_value(int argc, Janet *argv) {
  struct gpiod_line *line;
  int value;
  janet_fixarity(argc, 2);
  line = janet_unwrap_pointer(argv[0]);
  value = janet_unwrap_integer(argv[1]);
  if(gpiod_line_set_value(line, value) != 0)
    janet_panic("Failed to set line value");
  return janet_wrap_nil();
}

static Janet get_value(int argc, Janet *argv) {
  struct gpiod_line *line;
  int value;
  janet_fixarity(argc, 1);
  line = janet_unwrap_pointer(argv[0]);
  value = gpiod_line_get_value(line);
  if(value == -1)
    janet_panic("Failed to get line value");
  return janet_wrap_integer(value);
}

static Janet line_release(int argc, Janet *argv) {
  struct gpiod_line *line;
  janet_fixarity(argc, 1);
  line = janet_unwrap_pointer(argv[0]);
  gpiod_line_release(line);
  return janet_wrap_nil();
}

static Janet chip_close(int32_t argc, Janet *argv) {
  struct gpiod_chip *chip;
  janet_fixarity(argc, 1);
  chip = janet_unwrap_pointer(argv[0]);
  gpiod_chip_close(chip);
  return janet_wrap_nil();
}

static const JanetReg cfuns[] = {
  {"chip-open", chip_open, "(gpiod/chip-open chipname)\n\nOpen a gpiochip by name.\nExample : (gpiod/chip-open \"gpiochip0\")"},
  {"get-line", get_line, "(gpiod/get-line chip line-number)\n\nGet the handle to the GPIO line at given offset."},
  {"request-output", request_output, "(gpiod/request-output line consumer initial-value)\n\nReserve a single line, set the direction to output."},
  {"request-input", request_input, "(gpiod/request-input line consumer)\n\nReserve a single line, set the direction to input."},
  {"set-value", set_value, "(gpiod/set-value line value)\n\nSet the value of a single GPIO line."},
  {"get-value", get_value, "(gpiod/get-value line)\n\nGet the value of a single GPIO line."},
  {"line-release", line_release, "(gpiod/line-release line)\n\nRelease a previously reserved line."},
  {"chip-close", chip_close, "(gpiod/chip-close chip)\n\nClose a GPIO chip handle and release all allocated resources."},
  {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
    janet_cfuns(env, "gpiod", cfuns);
}
