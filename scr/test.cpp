#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>


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
// a specific query parameters, for testing 
string query_uri= "mongodb://user1:user1_123@localhost:27017";
string database="data";
string collection_name="LKr" ;
string documentid= "LKr-RawDecoderSettings" ;
string search_field ="NChannels" ;

//create a function to time every query operation
void timing_query(int id, std::vector<double>& response_times, std::mutex& mtx) {

    auto start = std::chrono::high_resolution_clock::now();

    // a specific query operation
    string query_result= search_db( query_uri,  database,  collection_name, documentid,  search_field);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // lock share data structure to protect it  
    std::lock_guard<std::mutex> lock(mtx);
    response_times.push_back(duration.count());

    std::cout << "Query " << id << " took " << duration.count() << " seconds.\n";
}


int main() {

    
    mongocxx::instance inst{};
    

    vector<thread> threads;
    vector<double> response_times;
    mutex mtx;

    auto start = std::chrono::high_resolution_clock::now();

    // create 30000 threads, one query operation for each thread
    for (int i = 0; i < 3000; ++i) {
        threads.push_back(thread(timing_query, i, std::ref(response_times), std::ref(mtx)));
    }

    // wait for all threads 
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double net_time=duration.count();
    // calculate the average response time
    double total_time = 0;
    for (double time : response_times) {
        total_time += time;
    }
    double average_time = total_time / response_times.size();

    cout << "Average response time: " << average_time << " seconds.\n";
    cout << "Total response time: " << total_time << " seconds.\n";
    cout << "Response time: " << net_time << " seconds.\n";

    return 0;
}
