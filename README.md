# bloomfilter

The following is an implementation of a bloom filter in C

A bloom filter is a probabilistic data structure that is based on hashing. It is extremely space efficient and is typically used to add elements
to a set and test if an element is in a set. Though, the elements themselves are not added to a set. Instead a hash of the elements is added to the set.

When testing if an element is in the bloom filter, false positives are possible. It will either say that an element is definitely not in the set or that it is possible the element is in the set.

A bloom filter is very much like a hash table in that it will use a hash function to map a key to a bucket. However, it will not store that key in that bucket, 
it will simply mark it as filled.  So, many keys might map to same filled bucket, creating false positives.

An empty bloom filter is a bit array of m bits, all of which are initially set to zero. A bit array is an extremely space-efficient data structure that has each 
position in the array set to either 0 or 1.

The applications of Bloom Filter are:

-Weak password detection
-Internet Cache Protocol
-Safe browsing in Google Chrome
-Wallet synchronization in Bitcoin
-Hash based IP Traceback
-Cyber security like virus scanning
