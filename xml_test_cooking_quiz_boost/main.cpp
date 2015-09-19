#include "question_xml_read.h"
#include <iostream>
#include <locale>
int main(void) {
	try {
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("japanese"));//locale設定
#else
		std::wcout.imbue(std::locale(""));//locale設定
#endif
		const auto xml = read_question_xml(u8"question.xml");
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;
		return -1;
	}
	return 0;
}