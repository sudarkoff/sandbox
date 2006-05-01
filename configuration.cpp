#include <map>
#include <string>
#include <fstream>

class configuration
{
  private:
    std::map<std::string, std::string> m;

    template <class T> void convert(const std::string &, T& out);

  public:
    void read(std::istream &in)
    {
    	while (!in.eof()) {
	        std::string var, val;
	        std::getline(in, var, '=');
	        in >> val;
	        in.ignore(INT_MAX,'\n');
        	m[var] = val;
        }
    }

    template <class T>
    T get(const std::string &varname, const T& def = T())
    {
        std::map<std::string, std::string>::iterator i = m.find(varname);

        // if not found in symbol table
        if(i==m.end()) return def;

        T retval;
        convert((*i).second, retval);
        return retval;
    }
};

template <>
void configuration::convert<int> (const std::string &s,int& out)
{
  out = atoi(s.c_str());
}

template <>
void configuration::convert<double> (const std::string &s, double &out)
{
  out = atof(s.c_str());
}

template <>
void configuration::convert<std::string> (const std::string &s, string &out)
{
  out = s;
}

template <>
void configuration::convert<bool> (const std::string &s, bool &out)
{
  out = ((s=="yes")||(s=="Y")||(s=="y")||(s=="1"))? true: false;
}

int main()
{
  configuration config;
  std::ifstream config_file("configuration.cfg");
  if (config_file.is_open()) {
	  config.read(config_file);
  }

  std::cout 
  	<< "http://www."
  	<< config.get<string>("host","cia.com") << ":"
  	<< config.get<int>("port") << "/?check="
 	<< config.get<bool>("check", false) << std::endl;
}
