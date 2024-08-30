import pandas as pd
import matplotlib.pyplot as plt
     

# record the average response time and total response time when concurrent queries happen
     

#average and total response time, 1000,2000, 3000,4000, 5000,6000  concurrent queries with index
query_numbers= [1000,2000,3000,4000,5000,6000]
ave_indexed= [1.13666,1.55465,2.44036,3.18424,3.84536,5.16678]
response_indexed = [2.26447,4.36796,7.06615,9.29517,12.0573,14.7225]
     

#average and total response time, 1000,2000, 3000,4000, 5000,6000 concurrent queries without index
ave = [0.824433,2.07294,2.60222,3.59463,4.36483,5.9015] 
response = [2.60559,5.59538,7.46116,10.2252,12.404,16.1915]
     

ave_df = pd.DataFrame({
    'queries':query_numbers,
    'ave_indexed':ave_indexed,
    'ave':ave
})

response_df = pd.DataFrame({
    'queries':query_numbers,
    'response_indexed':response_indexed,
    'response':response
})
     

# plot the trend of average response time in different concurrent queries with index and without index
     

plt.figure(figsize=(10, 6))
plt.plot(ave_df['queries'],ave_df['ave_indexed'], label='indexed', marker='o')
plt.plot(ave_df['queries'],ave_df['ave'], label='not indexed', marker='s')
plt.xlabel('Query number')
plt.ylabel('Average Response Time (s)')
plt.legend()
plt.savefig('average')
     

# plot the trend of total response time in different concurrent queries with index and without index
     

plt.figure(figsize=(10, 6))
plt.plot(response_df['queries'],response_df['response_indexed'], label='indexed', marker='o')
plt.plot(response_df['queries'],response_df['response'], label='not indexed', marker='s')
plt.xlabel('Query number')
plt.ylabel('Total Response Time (s)')
plt.legend()
plt.savefig('total')