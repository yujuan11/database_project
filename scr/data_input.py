import re 
import os
import pymongo
from pymongo import MongoClient

# Configuration
directory_data='../datafiles/Data/'
directory_test='../datafiles/Data/'
# write data to the database as a root user
mongodb_uri = 'mongodb://admin:admin_123@localhost:27017/'  # MongoDB URI
database_name_data = 'data'  # Database name

# use every folders name as collections' name in database
collection_name_data =['ANTI0','BEATCHFiles','CHANTI','CHOD','Cedar','GigaTracker','HAC','HLT','IRC','LAV','LKr','MUV0','MUV1',
                  'MUV2','MUV3','MagnetFields','NewCHOD','RICH','SAC','SAV','Trigger','VetoCounter']  # Collection name
##'Global','Overlay','Spectrometer',

database_name_test = 'test'  # Database name
collection_name_test  = ['ANTI0','BEATCHFiles','CHANTI','CHOD','Cedar','GigaTracker','HAC','HLT','IRC','LAV','LKr','MUV0','MUV1',
                  'MUV2','MUV3','MagnetFields','NewCHOD','RICH','SAC','SAV','Trigger','VetoCounter']  # Collection name
##'Global','Overlay','Spectrometer',

#empty list to save datafiles' path
directory_name_data=[]
directory_name_test=[]
for i in collection_name_data:
    directory_name_data.append(directory_data+i+'/')

for i in collection_name_test:
    directory_name_test.append(directory_test+i+'/')

# empty list to record datafiles' name which are not input successfully
file_input_failed=[]

#--------------------------------------------------------------------------------------------

def parse_dat_file(file_path):
    """
        This function is used to parse the data format into key-value pairs.
        Arg:
            file_path: the file's path to parse.
        Return:
            data: a dictionary which contains the parsed key-value pairs.
    """
    data = {}  # dictionary for saving the fields
    # want to use the files' name as unique id. split it from the file path and input as the '_id' in document later
    data.update({"_id":file_path.split('/')[-1].replace('.dat','')})
    data.update({"filename":file_path.split('/')[-1].replace('.dat','')})
    with open(file_path, 'r') as file:
        for line in file:
            # Remove whitespace characters at the start and end of the line
            line = line.strip()
            # Use regex to split the line into key and values, according to '='
            match = re.match(r'(\w+)=\s*(.*)', line)
            if match:
                key = match.group(1)
                values_str = match.group(2).split()
                values = []
                for value in values_str:
                    try:
                        # Try to convert the value to string
                        values.append(str(value))
                    except ValueError:
                        # Handle invalid values (skip or raise an error)
                         print(f"Warning: Skipping invalid value '{value}' in line: {line}")
                         continue
                data.update({key: values})
    #record the file's name which is not parsed properly
    if len(data)<2:
        file_input_failed.append(str(data.get('_id')))
    return data

# put filename in 'filename' field rather than _id field
def parse_dat_file_test(file_path):
    """
        This function is used to parse the data format into key-value pairs.
        Arg:
            file_path: the file's path to parse.
        Return:
            data: a dictionary which contains the parsed key-value pairs.
    """
    data = {}  # dictionary for saving the fields
    # want to use the files' name as unique id. split it from the file path and input as the '_id' in document later
    data.update({"filename":file_path.split('/')[-1].replace('.dat','')})
    with open(file_path, 'r') as file:
        for line in file:
            # Remove whitespace characters at the start and end of the line
            line = line.strip()
            # Use regex to split the line into key and values, according to '='
            match = re.match(r'(\w+)=\s*(.*)', line)
            if match:
                key = match.group(1)
                values_str = match.group(2).split()
                values = []
                for value in values_str:
                    try:
                        # Try to convert the value to string
                        values.append(str(value))
                    except ValueError:
                        # Handle invalid values (skip or raise an error)
                         print(f"Warning: Skipping invalid value '{value}' in line: {line}")
                         continue
                data.update({key: values})
    #record the file's name which is not parsed properly
    if len(data)<2:
        file_input_failed.append(str(data.get('_id')))
    return data


#---------------------------------------------------------------------------------------------------------------------

def input_function(dir, coll,database_name):
    """
        This function is used to bulkwrite data into database.
        Arg:
            dir: the directory's name
            coll: the collection's name
            database_name: which database to write in
        Returns:
        ...
    
    """
    # list all files' name under this directory
    all_entries = os.listdir(dir)
    # get all files' path
    data_files_names= [dir+f for f in all_entries if os.path.isfile(os.path.join(dir, f))]
    #parse datafile and transform the data format into key-value pairs
    if (database_name=='data'):
        parsed_data=[parse_dat_file(data_file) for data_file in data_files_names]
    else: parsed_data=[parse_dat_file_test(data_file) for data_file in data_files_names]
    

    # Connect to MongoDB
    client = MongoClient(mongodb_uri)
    db = client[database_name]
    collection = db[coll]


    # Insert parsed data into MongoDB

    # save all write steps into 'operations' for bulkwriting
    operations=[pymongo.InsertOne(data) for data in parsed_data]
    # use pymongo bulkWrite to write multiply documents now 
    try:
        result = collection.bulk_write(operations)
        #print(f"Inserted {result.inserted_count} documents")
    except pymongo.errors.BulkWriteError as e:
        print(f"Error: {e.details}")
#-------------------------------------------------------------------------------------------

# use a loop to write all dicrectorys' files
for dir, coll in zip(directory_name_data, collection_name_data):
    input_function(dir, coll, database_name_data)
#

# for dir, coll in zip(directory_name_test, collection_name_test):
#     input_function(dir, coll, database_name_test)


file_path="file_input_failed.txt"
if os.path.exists(file_path):
    os.remove(file_path)


# write the datafiles' name into 'file_input_failed' for which are not input successfully
with open(file_path, "w") as file:
    for i in file_input_failed:
       file.write(i+'\n')


