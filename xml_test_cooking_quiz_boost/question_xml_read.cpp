#include "question_xml_read.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <codecvt>
#include <stdexcept>
#include <type_traits>
question_xml_data_c::question_xml_data_c(std::wstring && question, std::wstring && explanation, bool is_not_for_biginner, int choices_num)
	: question(std::move(question)), explanation(std::move(explanation)), is_not_for_biginner(is_not_for_biginner), choices_num(choices_num) {}

template<typename T> static void replace_all(std::basic_string<T>& str, const std::basic_string<T>& old_str, const std::basic_string<T>& new_str) {
	for (size_t i = str.find_first_of(old_str); std::string::npos != i; i = str.find(old_str, i + new_str.length())) {
		str.replace(i, old_str.length(), new_str);
	}
}
template<typename T> static void replace_all(std::basic_string<T>& str, const T* old_str, const T* new_str) {
	replace_all(str, std::basic_string<T>(old_str), std::basic_string<T>(new_str));
}

static std::wstring get_text_from_node(boost::property_tree::ptree& p, const std::string& path) {
	auto target_node = p.get_optional<std::string>(path);
	if (!target_node) throw std::runtime_error("xmlのnodeの読み込みに失敗しました");
	static_assert(sizeof(wchar_t) == 2, "In function 'read_question' : Please check usage of 'std::codecvt_utf8_utf16'");
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	auto str = convert.from_bytes(target_node.get());
	replace_all(str, L"\\n", L"\n");
	return str;
}
static int get_int_from_node(boost::property_tree::ptree& p, const std::string& path) {
	auto target_node = p.get_optional<int>(path);
	if (!target_node) throw std::runtime_error("xmlのnodeの読み込みに失敗しました");
	return target_node.get();
}
std::vector<question_xml_data_c> read_question_xml(const std::string & xmlfilename){
	std::vector<question_xml_data_c> re;
	boost::property_tree::ptree pt;
	read_xml(xmlfilename, pt);
	for (auto& i : pt.get_child(u8"questionbook")) {
		re.emplace_back(
			get_text_from_node(i.second, u8"question"),
			get_text_from_node(i.second, u8"explanation"),
			0 != get_int_from_node(i.second, u8"IsNotForBiginner"),
			get_int_from_node(i.second, u8"choices_num")
		);
	}
	return re;
}
