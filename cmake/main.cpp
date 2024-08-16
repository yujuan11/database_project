
#include "crow.h"

// compile: g++ main.cpp -o main -lpthread
// execute: ./main
int main()
{
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){
        return "Hello world!!!!!!!!!!!!!!!";
    });
    app.port(18080).run();
}
