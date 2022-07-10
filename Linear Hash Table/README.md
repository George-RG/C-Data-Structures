# Linear Hash Table

## This module implements a [Linear Hash Table](https://en.wikipedia.org/wiki/Linear_hashing) data structure.

#### Advantages of Linear Hash Table
>The main advantage of this data structure over a tranditional hash table is that the worst case complexity of insertion is **O(1)**.

This is achieved by using a linear extension technique for the buckets.

The algorithm of insertion is as follows:
    
    - Insert the element in its desired bucket.
    - If the bucket is "full", insert the element in the overflow section of the buget and trigger a split.
    - If a split is needed, split the first bucket of the hash table into two buckets. Then mark the second bucket as "next to split".
    - If all the buckets are splited then double the size of the hash table.

>Normally, the doubling in size of the hash table is causing a rehash of the elements and a slow down in the insertion. But using this technique, the rehash is avoided because when the bucket is splited, the new buckets are created in the new double in size hash table.

#### Interfaces

>**This module provides the following interfaces:**

1. `LH_Initialize`: Initializes the hash table.
2. `LH_Insert`: Inserts a key-value element in the hash table. In case the key already exists, the value is updated. If the insertion is successful, the function returns `true`. Otherwise, it returns `false`.
3. `LH_Find`: Searches for a key in the hash table. If the key is found, the node is returned. Otherwise, `NULL` is returned.
4. `LH_Remove`: Deletes a key-value element from the hash table. if the deletion is successful, the function returns `true`. Otherwise, it returns `false`.
5. `LH_destroy`: Destroys the hash table. And frees all the memory allocated for the hash table.
6. `LH_Print`: Prints the hash table.
7. `LH_size`: Returns the number of elements in the hash table.


>**Some auxiliary functions are also provided:**
1. `LH_Get_Key`: Returns the key of the node.
2. `LH_Get_Value`: Returns the value of the node.
3. `LH_set_hash_function`: Sets the hash function.

>**Some not yet implemented functions that are planed for the future:**
3. `LH_Get_Statistics`: Returns the statistics of the hash table.
4. `LH_Reset_Statistics`: Resets the statistics of the hash table.
5. `LH_Split`: Splits the "next to split" bucket.