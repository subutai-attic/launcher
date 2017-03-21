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

std::vector<std::string> SubutaiLauncher::SubutaiString::ssplit(const std::string& delims)
{
    std::vector<std::string> tokens;
    std::size_t start = _str.find_first_not_of(delims), end = 0;

    while((end = _str.find_first_of(delims, start)) != std::string::npos)
    {
        tokens.push_back(_str.substr(start, end - start));
        start = _str.find_first_not_of(delims, end);
    }
    if(start != std::string::npos)
        tokens.push_back(_str.substr(start));

    return tokens;
}

std::string SubutaiLauncher::SubutaiString::remove(const std::string& before, const std::string& after){
    std::string::size_type n = 0;
    std::string out = _str;
    while ( ( n = out.find(before, n ) ) != std::string::npos )
    {
    	out.replace( n, before.size(), after );
    	//n += after.size();
    }    
    
    return out;
}

