Compile time-defined representation-dependent operations.

By now, it is unclear which memory representation will be used (log2, fixed point, or something
else). Therefore it's been decided to decompose the entire process into the atomic operations
(multiplication, complex multiplication, summation) and substitute the set of appropriate operations at 
compile time using template black magic.
