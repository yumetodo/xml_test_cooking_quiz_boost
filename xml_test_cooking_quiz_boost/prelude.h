#pragma once
#include <cstddef>
#include <limits>
#include <cstdint>
#include <string>
#ifdef max
#undef max
#endif

size_t choose_one_from_two(const wchar_t* echo_str) noexcept;
uint8_t prelude_question();
