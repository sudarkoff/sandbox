class MyController;

template <class ButtonArg>
class Button { };

template <class Window, class Controller>
class Widget { };

template <class Window>
class Widget<Window, MyController> { };

template <class ButtonArg>
class Widget<Button<ButtonArg>, MyController> { };

int main(int argc, char** argv, char** env)
{
	return (0);
}
