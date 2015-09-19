#pragma once
#include <string>
#include <vector>
struct question_xml_data_c {
	question_xml_data_c(std::wstring&& question, std::wstring&& explanation, bool is_not_for_biginner, size_t choices_num);
	std::wstring question;
	std::wstring explanation;
	bool is_not_for_biginner;
	size_t choices_num;
};
std::vector<question_xml_data_c> read_question_xml(const std::string& xmlfilename);
