#include "question_xml_read.h"
#include <array>
#include "make_array.h"
#include "Environment-dependent.h"
#include "prelude.h"
#include <iostream>
#include <locale>
#include <exception>
struct answer {
	int user_answer;
	bool jude_correct;
};
using ans_v = std::vector<answer>;
#include <iostream>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <limits>
#include <climits>
template <bool Con, class Then, class Else> struct IF;
template <class Then, class Else> struct IF<true, Then, Else> { typedef Then type; };
template <class Then, class Else> struct IF<false, Then, Else> { typedef Else type; };
template<typename T_> using limit = std::numeric_limits<T_>;//create new type. C++11:alias declaration
template<typename T_> using int8_conv_buf_t = typename IF<
	(CHAR_MAX == SCHAR_MAX && std::is_same<T_, char>::value) || std::is_same<T_, signed char>::value,
	int,
	typename IF<
		std::is_same<T_, unsigned char>::value || std::is_same<T_, char>::value,
		unsigned int,
		T_
	>::type
>::type;
template<typename T_> T_ input(const char* echo_str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) noexcept {
	static_assert(std::is_arithmetic<T_>::value, "unexpected type T_");//T_が整数か浮動小数点型でないならばコンパイルエラーを出す
	int8_conv_buf_t<T_> buf;
	try {
		if (nullptr == echo_str) throw std::invalid_argument("echo_str is unexpected input");//エラー対策
		if ('\0' != echo_str[0]) std::cout << echo_str << std::endl;//文字列が空じゃなければ出力
		std::cin >> buf;//入力を受ける
		if (max < buf || buf < min) throw std::out_of_range("input is iligal");//範囲チェック
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;//エラーメッセージ表示
		return input("再入力してください。", max, min);//エラー時は再帰する方向で
	}
	return buf;
}
template<typename T_> T_ input(const wchar_t* echo_str, const T_ max = limit<T_>::max(), const T_ min = limit<T_>::lowest()) noexcept {
	static_assert(std::is_arithmetic<T_>::value, "unexpected type T_");//T_が整数か浮動小数点型でないならばコンパイルエラーを出す
	int8_conv_buf_t<T_> buf;
	try {
		if (nullptr == echo_str) throw std::invalid_argument("echo_str is unexpected input");//エラー対策
		if ('\0' != echo_str[0]) std::wcout << echo_str << std::endl;//文字列が空じゃなければ出力
		std::cin >> buf;//入力を受ける
		if (max < buf || buf < min) throw std::out_of_range("input is iligal");//範囲チェック
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;//エラーメッセージ表示
		return input(L"再入力してください。", max, min);//エラー時は再帰する方向で
	}
	return buf;
}

auto decide_sequence_of_questions_number(uint8_t user_level) {
	static const auto sequence_of_questions_number_table = make_array(
		make_array<size_t>(0U, 1U, 2U, 3U, 4U, 8U, 12U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 5U, 9U, 13U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 6U, 10U, 14U, 16U, 17U, 18U),
		make_array<size_t>(0U, 1U, 2U, 3U, 7U, 11U, 15U, 16U, 17U, 18U)
		);
	return sequence_of_questions_number_table[user_level - 1];
}
void show_last_message(const bool isAllcrear, const bool judge_correct, const uint8_t judge_user_level) {
	static const auto message = make_array(
		make_array(
			L"入力ミスか遊びだと信じています。",
			L"入力ミスか遊びだと信じています。",
			L"夢パティで再確認してもう一度挑戦してくださいね。",
			L"次は頑張ってくださいね。"
			),
		make_array(
			L"簡単でしたよね？簡単すぎてすいません。",
			L"簡単でしたよね？簡単すぎてすいません。",
			L"あなたは本当に夢パティ愛してますね。",
			L"クリアおめでとうございます。"
			)
		);
	std::wcout << message[isAllcrear][judge_user_level] << std::endl;
	if (judge_correct && 4 == judge_user_level && !isAllcrear) std::wcout << L"次は全問正解を目指しましょう。" << std::endl;
	pause_console();
	clear_console();
	std::wcout
		<< L"このゲームの問題は以上になります。もしこんなゲーム作ってほしい、問題を増やしてほしい等ありましたら、readmeに書かれましたメールアドレスへ。"
		<< std::endl
		<< L"新作ゲーム情報等は、readmeのリンクからアクセスできますブログに順次記載していきます。"
		<< std::endl;
}
bool ask(ans_v ans, const uint8_t user_level, const int correct_answer, const question_xml_data_c& nodelist) {
	answer re;
	//問題文表示, 回答入力
	re.user_answer = input(nodelist.question.c_str(), nodelist.choices_num, 1U);
	re.jude_correct = (correct_answer == re.user_answer);
	bool judge_continue = re.jude_correct;
	std::wcout << ((re.jude_correct) ? L"正解" : L"不正解") << L"です。" << std::endl;
	if (!re.jude_correct && 4 == user_level) {
		std::wcout << "正解は" << correct_answer << "です。" << std::endl
			<< nodelist.explanation << std::endl;////解説表示
		if (nodelist.is_not_for_biginner) judge_continue = true;
	}
	pause_console();
	clear_console();
	ans.push_back(re);
	return judge_continue;
}

int main(void) {
	try {
#ifdef _MSC_VER
		std::wcout.imbue(std::locale("japanese"));//locale設定
#else
		std::wcout.imbue(std::locale(""));//locale設定
#endif
		std::wcout << L"クイズ　～あなたはパティシエ～" << std::endl << std::endl;
		if (2 == choose_one_from_two(L"開始…１　終了…２")) return 0;
		//xml read
		const auto xml = read_question_xml(u8"question.xml");
		const auto correct_answer = make_array(3, 2, 2, 1, 3, 3, 3, 3, 1, 1, 4, 2, 1, 6, 4, 2, 2, 3, 1);
		if(xml.size() != correct_answer.size()) return -1;//xmlの要素数と正解の数すべてが一致しない場合プログラム終了
		const auto user_level = prelude_question();//事前質問
		const auto sequence_of_questions_number = decide_sequence_of_questions_number(user_level);//問題順序の決定
		std::wcout << L"回答ありがとうございます。それでは始めます。" << std::endl;
		pause_console();
		clear_console();

		//質問
		bool judge_correct = true;
		ans_v answer;
		for (size_t i = 0; judge_correct == true && i < sequence_of_questions_number.size(); ++i) {
			const auto question_id = sequence_of_questions_number[i];
			judge_correct = ask(answer, user_level, correct_answer[question_id], xml[question_id]);
		}
		bool isAllcrear = true;
		for (auto& i : answer) isAllcrear &= i.jude_correct;//全問正解か調べる
		show_last_message(isAllcrear, judge_correct, user_level);
	}
	catch (std::exception& er) {
		std::cerr << er.what() << std::endl;
		return -1;
	}
	return 0;
}