#include <map>
#include <functional>
#include <sys/time.h>

template <typename T, timespec& now>
class TaskExpired : public std::unary_function<T, bool> {
    public: 

    bool operator () (argument_type& val)
    { 
        return (true);
    } 
}; 

int main(void)
{
	typedef int TaskInfo;
	std::map<int, TaskInfo> tasks;
    timespec now;

    std::remove_if(tasks.begin(), tasks.end(), TaskExpired<TaskInfo, now>);

    return 1;
}
