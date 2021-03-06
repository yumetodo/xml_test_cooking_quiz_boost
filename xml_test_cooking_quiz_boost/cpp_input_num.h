﻿#pragma once
#include <iostream>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <limits>
#include <climits>
namespace detail {
	using std::is_same;
	template <bool Con, class Then, class Else> struct IF;
	template <class Then, class Else> struct IF<true, Then, Else> { typedef Then type; };
	template <class Then, class Else> struct IF<false, Then, Else> { typedef Else type; };
	template<typename T_> using int8_conv_buf_t = typename IF<
		(CHAR_MAX == SCHAR_MAX && is_same<T_, char>::value) || is_same<T_, signed char>::value,
		int,
		typename IF<
		is_same<T_, unsigned char>::value || is_same<T_, char>::value || is_same<T_, bool>::value,
		unsigned int,
		T_
		>::type
	>::type;
	template<typename Out_, typename In_, bool is_integer = std::is_integral<In_>::value> struct cast_c {
		Out_ operator()(In_ n) { return static_cast<Out_>(n); }
	};
	template<typename In_> struct cast_c<bool, In_, true> {
		bool operator()(In_ n) { return static_cast<In_>(0) != n; }
	};
	void echo_str(const char* echo_str);
	void echo_str(const wchar_t* echo_str);

}
template<typename T_> using limit = std::numeric_limits<T_>;//create new type. C++11:alias declaration
template<typename T_, typename std::enable_if<!std::is_same<T_, bool>::value, std::nullptr_t>::type = nullptr> T_ inputA(const char* echo_str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) noexcept {
	static_assert(std::is_arithmetic<T_>::value, "unexpected type T_");//T_が整数か浮動小数点型でないならばコンパイルエラーを出す
	detail::int8_conv_buf_t<T_> buf;
	try {
		detail::echo_str(echo_str);
		std::cin >> buf;//入力を受ける
		if (max < buf || buf < min) throw std::out_of_range("input is iligal");//範囲チェック
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;//エラーメッセージ表示
		return inputA("再入力してください。", max, min);//エラー時は再帰する方向で
	}
	return detail::cast_c<T_, decltype(buf)>()(buf);
}
template<typename T_, typename std::enable_if<!std::is_same<T_, bool>::value, std::nullptr_t>::type = nullptr> T_ input(const wchar_t* echo_str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) noexcept {
	static_assert(std::is_arithmetic<T_>::value, "unexpected type T_");//T_が整数か浮動小数点型でないならばコンパイルエラーを出す
	detail::int8_conv_buf_t<T_> buf;
	try {
		detail::echo_str(echo_str);
		std::cin >> buf;//入力を受ける
		if (max < buf || buf < min) throw std::out_of_range("input is iligal");//範囲チェック
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;//エラーメッセージ表示
		return input(L"再入力してください。", max, min);//エラー時は再帰する方向で
	}
	return detail::cast_c<T_, decltype(buf)>()(buf);
}
