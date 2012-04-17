int i;
std::cout << typeid(i).name();  // int
std::cout << typeid((int &)i).name(); // int (??)
std::cout << typeid(int &).name(); // int &