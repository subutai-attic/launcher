#include "SubutaiString.h"

SubutaiLauncher::SubutaiString::SubutaiString(const std::string& str) : _str(str) {

}

SubutaiLauncher::SubutaiString::~SubutaiString() {

}

void SubutaiLauncher::SubutaiString::split(char delim, std::vector<std::string> &items) {
	std::stringstream ss(_str);
	std::string item;
	while (getline(ss, item, delim)) {
		items.push_back(item);
	}
}

