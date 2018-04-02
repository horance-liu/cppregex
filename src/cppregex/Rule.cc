#include "cppregex/Rule.h"
#include <regex>

namespace {
  using CharSpec = std::function<bool(char)>;

  Rule atom(CharSpec spec) {
    return [spec](const std::string& str, Matcher matcher) {
      if (str.empty()) return false;
      else return spec(str[0]) && matcher(str.substr(1));
    };
  }
}

Rule val(const std::string& value) {
  return [value](const std::string& str, Matcher matcher) {
    if (value.empty()) return str.empty();
    else if (str.size() < value.size()) return false;
    else
      return value == str.substr(0, value.size())
          && matcher(str.substr(value.size()));
  };
}

Rule val(char v) {
  return atom([v](char c) {
    return v == c;
  });
}

Rule any() {
  return atom([](char) {
    return true;
  });
}

Rule oneof(const std::string& str) {
  return atom([str](char c) {
    return str.find(c) != std::string::npos;
  });
}

Rule eof(Rule rule) {
  auto end = [](const std::string& str, Matcher) {
    return str.empty();
  };
  return sequence(rule, end);
}

Rule sequence(Rule first, Rule second) {
  return [first, second](const std::string& str, Matcher matcher) {
    return first(str, [second, matcher](const std::string& rest) {
      return second(rest, matcher);
    });
  };
}

Rule alternative(Rule first, Rule second) {
  return [first, second](const std::string& str, Matcher matcher) {
    return first(str, matcher) || second(str, matcher);
  };
}

Rule optional(Rule rule) {
  return [rule](const std::string& str, Matcher matcher) {
    return rule(str, matcher) || matcher(str);
  };
}

Rule zero_or_more(Rule rule) {
  return [rule](const std::string& str, Matcher matcher) {
    return rule(str, [rule, matcher](const std::string& rest) {
      return zero_or_more(rule)(rest, matcher);
    }) || matcher(str);
  };
}

Rule one_or_more(Rule rule) {
  return sequence(rule, zero_or_more(rule));
}

bool match(Rule rule, const std::string& str) {
  return rule(str, [](const std::string&) {
    return true;
  });
}
