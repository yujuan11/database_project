import re 
import os
import pymongo
from pymongo import MongoClient

# Configuration
directory_data='../datafiles/Data/'
directory_mc='../datafiles/MC/'
mongodb_uri = 'mongodb://localhost:27017/'  # MongoDB URI
database_name_data = 'data'  # Database name
collection_name_data =['ANTI0','BEATCHFiles','CHANTI','CHOD','Cedar','GigaTracker','HAC','HLT','IRC','LAV','LKr','MUV0','MUV1',
                  'MUV2','MUV3','MagnetFields','NewCHOD','RICH','SAC','SAV','Trigger','VetoCounter']  # Collection name
##'Global','Overlay','Spectrometer',

database_name_mc = 'mc'  # Database name
collection_name_mc =['CHOD','Cedar','Datacards','Dictionaries','Geometry','HLT',
                  'LAV','MUV3','Shadows','Spectrometer','Trigger']  # Collection name
##'Beam','ExternalGenerator','GigaTracker','LKr','Overlay','RICH',

directory_name_data=[]
directory_name_mc=[]
for i in collection_name_data:
    directory_name_data.append(directory_data+i+'/')

for i in collection_name_mc:
    directory_name_mc.append(directory_mc+i+'/')

file_input_failed=[]

def parse_dat_file(file_path):
    data = {}
    data.update({"_id":file_path.split('/')[-1].replace('.dat','')})
    with open(file_path, 'r') as file:
        for line in file:
            # Remove whitespace characters at the start and end of the line
            line = line.strip()
            # Use regex to split the line into key and values
            match = re.match(r'(\w+)=\s*(.*)', line)
            if match:
                key = match.group(1)
                values_str = match.group(2).split()
                values = []
                for value in values_str:
                    try:
                        # Try to convert the value to an integer (decimal)
                        values.append(str(value))
                    except ValueError:
                        # Handle invalid values (skip or raise an error)
                         print(f"Warning: Skipping invalid value '{value}' in line: {line}")
                         continue
                data.update({key: values})
    if len(data)<2:
        file_input_failed.append(str(data.get('_id')))
    return data

def input_function(dir, coll,database_name):
    
    all_entries = os.listdir(dir)

    data_files_names= [dir+f for f in all_entries if os.path.isfile(os.path.join(dir, f))]
    parsed_data=[parse_dat_file(data_file) for data_file in data_files_names]

    # Connect to MongoDB
    client = MongoClient(mongodb_uri)
    db = client[database_name]
    collection = db[coll]


    # Insert parsed data into MongoDB

    # use pymongo bulkWrite to write multiply documents now 
    operations=[pymongo.InsertOne(data) for data in parsed_data]

    try:
        result = collection.bulk_write(operations)
        #print(f"Inserted {result.inserted_count} documents")
    except pymongo.errors.BulkWriteError as e:
        print(f"Error: {e.details}")


for dir, coll in zip(directory_name_data, collection_name_data):
    input_function(dir, coll, database_name_data)

for dir, coll in zip(directory_name_mc, collection_name_mc):
    input_function(dir, coll, database_name_mc)

with open("file_input_failed.txt", "w") as file:
    for i in file_input_failed:
       file.write(i+'\n')


