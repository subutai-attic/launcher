#ifndef __STRING_H__
#define __STRING_H__

#include <string>
#include <vector>
#include <sstream>

namespace SubutaiLauncher {

	class SubutaiString {
	public:
		SubutaiString(const std::string& str);
		~SubutaiString();
		void split(char delim, std::vector<std::string> &items);
	private:
		std::string _str;
	};

}

#endif
