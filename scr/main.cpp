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

// //A function to query a field in a document from a collection
string search_db( string uri, string database, string collection_name,string documentid, string search_field)
{
    string mongouri=uri + "/" +database;
    mongocxx::client conn{mongocxx::uri{mongouri}};
    mongocxx::collection collection=conn[database][collection_name];
    mongocxx::options::find options{};
    options.projection(make_document(kvp("_id",1),kvp(search_field,1)));

    auto cursor = collection.find(make_document(kvp("_id",documentid)),options);
    string search_results;
    for(auto && doc : cursor)
    {
        cout<<bsoncxx::to_json(doc)<<endl;
        search_results= search_results + string(bsoncxx::to_json(doc));
    }
    return search_results;
}

// //A function to insert a document from a collection
string insert_db(string uri, string database,string collection_name, string documentid, string fieldname, string fieldvalue)
{
    string mongouri=uri + "/" +database;
    mongocxx::client conn{mongocxx::uri{mongouri}};
    mongocxx::collection coll=conn[database][collection_name];
    auto insert_one_result = coll.insert_one(make_document(kvp("_id",documentid),kvp(fieldname,fieldvalue)));
    //
    string insert_result;
    if (insert_one_result) {
    // 
    //auto inserted_id = insert_one_result->inserted_id().get_oid().value.to_string();
    insert_result= "Insert document ID: " + documentid;
    } else {
    // 
        insert_result = "Insert operation failed." ;
    }

    return insert_result;
}

// insert function for future use
// insert many document into a specific collection
int insert_many_document(string uri, string database, string collection_name)
{
    string mongouri=uri + "/" +database;
    mongocxx::client conn{mongocxx::uri{mongouri}};
    mongocxx::collection coll=conn[database][collection_name];
    vector<bsoncxx::document::value> documents;
    documents.push_back(make_document(kvp("_id","first"),kvp("field","field_one")));
    documents.push_back(make_document(kvp("_id","second"),kvp("field","field_one"),kvp("fiels","field_two")));
    auto insert_many_results= coll.insert_many(documents);
    


    return EXIT_SUCCESS;
}

// //A function to update a document from a collection
string update_db(string uri, string database, string collection_name, string documentid, string modify_field, string modify_field_value)
{
    string mongouri=uri + "/" +database;
    mongocxx::client conn{mongocxx::uri{mongouri}};
    mongocxx::collection coll=conn[database][collection_name];
    auto update_one_result= coll.update_one(make_document(kvp("_id",documentid)),
                                            make_document(kvp("$set",make_document(kvp(modify_field,modify_field_value)))));
    //
    string update_result;
    if(update_one_result){
        update_result= "Update document ID: " + documentid + ";" + "Update field: {" + modify_field + ":"+modify_field_value+"}";
    } else {
    // 
        update_result = "Update operation failed." ;
    }

    return update_result;
}

//A function to delete a document from a collection
string delete_db(string uri, string database, string collection_name, string documentid)
{
    string mongouri=uri + "/" +database;
    mongocxx::client conn{mongocxx::uri{mongouri}};
    mongocxx::collection coll=conn[database][collection_name];
    auto find_result=coll.find_one(make_document(kvp("_id",documentid)));

    //
    string delete_result;
    if(find_result){
        auto delete_one_result = coll.delete_one(make_document(kvp("_id", documentid)));
        delete_result= "Delete document: "+documentid + " from database:"+database+"collection: "+collection_name;
    } else {
    // 
        delete_result= "Document not found.";
    }

    return delete_result;
}

string global_uri;

int main() {
    // The mongocxx::instance constructor and destructor initialize and shut down the driver,
    // respectively. Therefore, a mongocxx::instance must be created before using the driver and
    // must remain alive for as long as the driver is in use.
    mongocxx::instance inst{};
    

    //create the application 
    crow::SimpleApp app;

    

    // login route, default use GET method
    CROW_ROUTE(app, "/login")([](const crow::request& req){
        auto login_temp= crow::mustache::load("loginpage.html");
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
	
	string uri= "mongodb://" + string(username) + ":" + string(pwd) + "@localhost:27017";
	global_uri=uri;
	try{
	    //mongocxx::client client(uri);
	   
        string loc="/select";
	    crow::response res;
        res.redirect(loc);
        return res;

	   
	} catch (const exception & e){
 		cerr<<"Login failed: " << e.what()<<endl;
		return crow::response(403, "Invalid username or password");

	};
    });

    // input search information to get the field's value
    CROW_ROUTE(app,"/select").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto selection_operation= crow::mustache::load("selectoperation.html");
        crow::mustache::context ctx;
	    auto page= selection_operation.render(ctx);
        return page;

    });

    CROW_ROUTE(app,"/query").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto query= crow::mustache::load("query.html");
        crow::mustache::context ctx;
	    auto page= query.render(ctx);
        return page;

    });
    CROW_ROUTE(app,"/queryresult").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        cout << "Request Body: " << req.body << endl;
        auto full_param= req.get_body_params();
        cout<<"full_param:"<<full_param<<endl;
        const char* database_name= full_param.get("databasename");
        cout<<"database_name:"<<database_name<<endl;
        const char* collection_name= full_param.get("collectionname");
        const char* document_id= full_param.get("documentid");
        const char* field_name= full_param.get("fieldname");
        try {
            string results=search_db(global_uri,string(database_name),string(collection_name),string(document_id),string(field_name) );
            auto query_results= crow::mustache::load("query_results.html");
            crow::mustache::context ctx;
            ctx["query_results"]=results;
            auto page= query_results.render(ctx);
            return crow::response{page};    //try ,catch should return the same type
        } catch(const exception & e){
            crow::json::wvalue error_response;
            error_response["error"] =  e.what();
            return crow::response(500, error_response);
        };


    });

    CROW_ROUTE(app,"/update").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto update= crow::mustache::load("update.html");
        crow::mustache::context ctx;
	    auto page= update.render(ctx);
        return page;

    });
    CROW_ROUTE(app,"/updateresult").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        cout << "Request Body: " << req.body << endl;
        auto full_param= req.get_body_params();
        cout<<"full_param:"<<full_param<<endl;
        const char* database_name= full_param.get("databasename");
        cout<<"database_name:"<<database_name<<endl;
        const char* collection_name= full_param.get("collectionname");
        const char* document_id= full_param.get("documentid");
        const char* field_name= full_param.get("fieldname");
        const char* field_value= full_param.get("fieldvalue");
        try {
            string results=update_db(global_uri,string(database_name),string(collection_name),string(document_id),string(field_name),string(field_value));

            auto update_results= crow::mustache::load("update_results.html");
            crow::mustache::context ctx;
            ctx["update_results"]=results;
            auto page= update_results.render(ctx);
            return crow::response{page};    //try ,catch should return the same type
        } catch(const exception & e){
            crow::json::wvalue error_response;
            error_response["error"] =  e.what();
            return crow::response(500, error_response);
        };
        
    });
    //
    CROW_ROUTE(app,"/insert").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto insert= crow::mustache::load("insert.html");
        crow::mustache::context ctx;
	    auto page= insert.render(ctx);
        return page;

    });
    CROW_ROUTE(app,"/insertresult").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        cout << "Request Body: " << req.body << endl;
        auto full_param= req.get_body_params();
        cout<<"full_param:"<<full_param<<endl;
        const char* database_name= full_param.get("databasename");
        cout<<"database_name:"<<database_name<<endl;
        const char* collection_name= full_param.get("collectionname");
        const char* document_id= full_param.get("documentid");
        const char* field_name= full_param.get("fieldname");
        const char* field_value= full_param.get("fieldvalue");
        try {
            string results=insert_db(global_uri,string(database_name),string(collection_name),string(document_id),string(field_name),string(field_value));
            auto insert_results= crow::mustache::load("insert_results.html");
            crow::mustache::context ctx;
            ctx["insert_results"]=results;
            auto page= insert_results.render(ctx);
            return crow::response{page};    //try ,catch should return the same type
        } catch(const exception & e){
            crow::json::wvalue error_response;
            error_response["error"] =  e.what();
            return crow::response(500, error_response);
        };
        
    
    });


    CROW_ROUTE(app,"/delete").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        auto deletetemp= crow::mustache::load("delete.html");
        crow::mustache::context ctx;
	    auto page= deletetemp.render(ctx);
        return page;

    });

    CROW_ROUTE(app,"/deleteresult").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        cout << "Request Body: " << req.body << endl;
        auto full_param= req.get_body_params();
        cout<<"full_param:"<<full_param<<endl;
        const char* database_name= full_param.get("databasename");
        cout<<"database_name:"<<database_name<<endl;
        const char* collection_name= full_param.get("collectionname");
        const char* document_id= full_param.get("documentid");
        try {
            string results=delete_db(global_uri,string(database_name),string(collection_name),string(document_id));

            auto delete_results= crow::mustache::load("delete_results.html");
            crow::mustache::context ctx;
            ctx["delete_results"]=results;
            auto page= delete_results.render(ctx);
            return crow::response{page};    //try ,catch should return the same type
        } catch(const exception & e){
            crow::json::wvalue error_response;
            error_response["error"] =  e.what();
            return crow::response(500, error_response);
        };
        
    });


    //Run the server on multiple threads using all available threads.
    app.port(18080).multithreaded().run();

    
    return EXIT_SUCCESS;

}



