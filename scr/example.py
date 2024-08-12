import re 
import os
import pymongo
from pymongo import MongoClient

# Configuration
directory='../datafiles/'
data_file1 = '../datafiles/LKr-RawDecoderSettings.dat'  # Path to .dat file
data_file2 = '../datafiles/LKr-RawDecoderSettings.run001000_0000-run004399_9999.dat'  # Path to .dat file
mongodb_uri = 'mongodb://localhost:27017/'  # MongoDB URI
database_name = 'cern_na62'  # Database name
collection_name = 'LKr'  # Collection name

def parse_dat_file(file_path):
    data = {}
    data.update({"_id":file_path.replace('../datafiles/','')})
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
    return data

all_entries = os.listdir(directory)


data_files_names= ["../datafiles"+"/"+f for f in all_entries if os.path.isfile(os.path.join(directory, f))]
parsed_data=[parse_dat_file(data_file) for data_file in data_files_names]
# parsed_data1 = parse_dat_file(data_file1)
# parsed_data2 = parse_dat_file(data_file2)

print(data_files_names)

# Connect to MongoDB
client = MongoClient(mongodb_uri)
db = client[database_name]
collection = db[collection_name]


# Insert parsed data into MongoDB
#collection.insert_one(parsed_data)
# use pymongo bulkWrite to write multiply documents now 
operations=[pymongo.InsertOne(data) for data in parsed_data]


try:
    result = collection.bulk_write(operations)
    #print(f"Inserted {result.inserted_count} documents")
except pymongo.errors.BulkWriteError as e:
    print(f"Error: {e.details}")


#print(f"{data_file} loaded into {database_name}.{collection_name} successfully.")




