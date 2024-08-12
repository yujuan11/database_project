
#include "crow.h"

// 编译: g++ main.cpp -o main -lpthread
// 运行: ./main
int main()
{
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){
        return "Hello world!!!!!!!!!!!!!!!";
    });
    app.port(18080).run();
}
