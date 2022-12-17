#include "include/JoInterpreter.h"
#include "include/IO.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        Error("Usage Jolang <file>");
        return 1;
    }

    JoIn_InterpretFile(argv[1]);

    return 0;
}
