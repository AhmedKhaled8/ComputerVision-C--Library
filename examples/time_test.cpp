#include <chrono>

auto t_start = std::chrono::high_resolution_clock::now();
// the work...
auto t_end = std::chrono::high_resolution_clock::now();

double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
