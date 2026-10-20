# Lab6_Vector_Lab

Calculates basic vector math (+, -, scalar) from user input.

## Local Testing

### Build
```bash
make
```

### Run

```bash
./minimat
```
Or, to get a help message of program commands

```bash
./minimat -h
```

## Program Commands

* `quit` - exits the program.
* `list` - displays all vectors in storage.
* `clear` - clears all vectors in storage.
* `load <filename>` - loads all vectors from `<filename>`. File must be formatted like a csv file.
* `save <filename>` - saves all vectors to `<filename>`. File will be written to in csv format.

**Assignment:**
```
minimat> vectName = float1 float2 float3
```
If an assignment is made to a vector who's name already exists (including through `load <filename>`), the old vector will be overwritten.

**Addition/Subtraction:**
```
// prints output
minimat> vectName1 + vectName2
minimat> vectName1 - vectName2

// assigns output to vector
minimat> vectName3 = vectName1 + vectName2
minimat> vectName3 = vectName1 - vectName2
```

**Scalar Multiplication:**
```
// prints output
minimat> vectName1 * float
minimat> float * vectName

// assigns output to vector
minimat> vectName2 = vectName1 * float
minimat> vectName2 = float * vectName
```

## Dynamic Memory

`vector_storage.c` uses a dynamic array to store all vectors.

`init_storage()` must be called with a successful return code to allocate the array. The array begins at the length specified by `VECTOR_ARRAY_INITIAL_LENGTH` at the top of the file.

Whenever the length will be overflown, the storage file makes one call to `reallocate()` to attempt to double the size of the array. If this fails, the error code is returned outside of the file (e.g. the function `assign()` would immediately return an error without retrying).

The file also has a `clear()` function to free all memory. It is the UI's job to call `clear()` on exit to avoid memory leaks. To continue using the storage array after calling `clear()`, a call to `init_storage()` must be successful.