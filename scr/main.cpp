#include "crow.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>


#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <mongocxx/uri.hpp>

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// search a specific field from a document
int search_db( string uri, string database, string collection_name,string document, string search_field)
{
    mongocxx::client conn{mongocxx::uri{uri}};
    mongocxx::collection collection=conn[database][collection_name];
    mongocxx::options::find options{};
    options.projection(make_document(kvp("_id",1),kvp(search_field,1)));

    auto cursor = collection.find(make_document(kvp("_id",document)),options);
    for(auto && doc : cursor)
    {
        cout<<bsoncxx::to_json(doc)<<endl;
    }
    return EXIT_SUCCESS;
}

// insert one document into a specific collection
int insert_one_document(string uri, string database,string collection_name)
{
    mongocxx::client conn{mongocxx::uri{uri}};
    mongocxx::collection coll=conn[database][collection_name];
    auto insert_one_result = coll.insert_one(make_document(kvp("_id","insertone"),kvp("field","field_one")));
    

    return EXIT_SUCCESS;
}

// insert many document into a specific collection
int insert_many_document(string uri, string database, string collection_name)
{
    mongocxx::client conn{mongocxx::uri{uri}};
    mongocxx::collection coll=conn[database][collection_name];
    vector<bsoncxx::document::value> documents;
    documents.push_back(make_document(kvp("_id","first"),kvp("field","field_one")));
    documents.push_back(make_document(kvp("_id","second"),kvp("field","field_one"),kvp("fiels","field_two")));
    auto insert_many_results= coll.insert_many(documents);
    


    return EXIT_SUCCESS;
}
//16834
// modify one field in a specific document
int update_db(string uri, string database, string collection_name, string document, string modify_field, string modify_field_value)
{
    mongocxx::client conn{mongocxx::uri{uri}};
    mongocxx::collection coll=conn[database][collection_name];
    auto update_one_result= coll.update_one(make_document(kvp("_id",document)),
                                            make_document(kvp("$set",make_document(kvp(modify_field,modify_field_value)))));
    
    return EXIT_SUCCESS;
}

int delete_a_document_from_collection()
{
    mongocxx::client conn{mongocxx::uri{}};

    return EXIT_SUCCESS;
}


int main() {
    // The mongocxx::instance constructor and destructor initialize and shut down the driver,
    // respectively. Therefore, a mongocxx::instance must be created before using the driver and
    // must remain alive for as long as the driver is in use.
    mongocxx::instance inst{};
    
    
    //search_db("data","LKr","LKr-RawDecoderSettings","NChannels" );
    //update_db("cern_na62","LKr","LKr-RawDecoderSettings.dat","NChannels","16834");
    
    

    //create the application 
    crow::SimpleApp app;

    auto login_temp= crow::mustache::load("loginpage.html");

    // login route, default use GET method
    CROW_ROUTE(app, "/login")([login_temp](const crow::request& req){
        crow::mustache::context ctx;
	auto page= login_temp.render(ctx);
        return page;
    });
    //verify login route, POST method
    CROW_ROUTE(app,"/login").methods(crow::HTTPMethod::POST)([](const crow::request& req){
	cout << "Request Body: " << req.body << endl;
	auto full_param= req.get_body_params();
	cout<<"full_param:"<<full_param<<endl;
	const char* username= full_param.get("username");
	cout<<"username:"<<username<<endl;
	const char* pwd= full_param.get("password");
	cout<<"pwd:"<<pwd<<endl;
	
	if ( !username || !pwd ) {
           // 
           cout << "Username or password not provided" << endl;
           return crow::response(400, "Username or password missing");
	}
	
	string uri= "mongodb://" + string(username) + ":" + string(pwd) + "@localhost:27017/data";
	
	try{
	    //mongocxx::client client(uri);
	    search_db(uri,"data","LKr","LKr-RawDecoderSettings","NChannels" );
	    update_db(uri,"data","LKr","LKr-RawDecoderSettings","NChannels","16839");
	    search_db(uri,"data","LKr","LKr-RawDecoderSettings","NChannels" );
	    return crow::response(302,crow::json::wvalue({{"Location", "/selectoperation"}}));
	   

	} catch (const exception & e){
 		cerr<<"Login failed: " << e.what()<<endl;
		return crow::response(403, "Invalid username or password");

	};
    });
    app.port(18080).multithreaded().run();
    
    return EXIT_SUCCESS;

}



