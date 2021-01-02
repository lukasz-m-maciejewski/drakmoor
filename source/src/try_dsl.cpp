#include <iostream>
#include <string>

class Name
{
public:
    std::string name;

    Name& operator=(std::string&& nn)
    {
        name = std::move(nn);
        return *this;
    }
};

void print_name(Name n)
{
    std::cout << "We've got a name: " << n.name << '\n';
}

int main()
{
    std::cout << "Hello!\n";
    print_name( Name() = "Stefan" );
    return 0;
}
