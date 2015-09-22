#pragma once
#include <string>
#include <vector>
struct question_xml_data_c {
	question_xml_data_c(std::wstring&& question, std::wstring&& explanation, bool is_not_for_biginner, int choices_num);
	std::wstring question;
	std::wstring explanation;
	bool is_not_for_biginner;
	int choices_num;//負の数も想定する
};
std::vector<question_xml_data_c> read_question_xml(const std::string& xmlfilename);
