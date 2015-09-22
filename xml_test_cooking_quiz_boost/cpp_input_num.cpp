#include "cpp_input_num.h"
namespace detail {
	void echo_str(const char * echo_str){
		if (nullptr == echo_str) throw std::invalid_argument("echo_str is unexpected input");//エラー対策
		if ('\0' != echo_str[0]) std::cout << echo_str << std::endl;//文字列が空じゃなければ出力
	}
	void echo_str(const wchar_t * echo_str){
		if (nullptr == echo_str) throw std::invalid_argument("echo_str is unexpected input");//エラー対策
		if ('\0' != echo_str[0]) std::wcout << echo_str << std::endl;//文字列が空じゃなければ出力
	}
}
