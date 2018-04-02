#ifndef H2F91267D_249C_4761_925D_4163DA86F677
#define H2F91267D_249C_4761_925D_4163DA86F677

#include <functional>
#include <string>

using Matcher = std::function<bool(const std::string&)>;
using Rule = std::function<bool(const std::string&, Matcher)>;

Rule val(char);
Rule val(const std::string&);

Rule any();
Rule eof(Rule rule);
Rule oneof(const std::string&);

Rule sequence(Rule first, Rule second);
Rule alternative(Rule first, Rule second);

Rule optional(Rule rule);
Rule zero_or_more(Rule rule);
Rule one_or_more(Rule rule);

bool match(Rule rule, const std::string& str);

#endif
