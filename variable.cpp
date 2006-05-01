class Variable { 
    class Read { 
    public:
    	class Undefined { };

    	virtual ~Read() { }
    	virtual std::auto_ptr<Variable> read(std::istream&) = 0;
    };
public:
    virtual Variable* clone() const = 0;
};

class Integer : public Variable { 
public:
	Integer* clone() const { return new Integer(*this); }
};

class Reader : public Variable::Read {
public:
	std::auto_ptr<Variable> Reader::read(std::istream& strm) {
    	Variable* variable = 0;
    	std::string text;

    	strm >> text;

    	if (false) ;
    	else if (Integer::is(text)) variable = new Integer(text);
    	else throw Undefined();

    	std::auto_ptr<Variable> out(variable);
    	return out;
	}
};


typedef std::map<std::string, Variable*> symbol_table_t;


int main()
{
	ifstream cfg;
	symbol_table_t symbol_table;
    symbol_table[name] = Reader().read(cfg).release();

	return 0;
}
