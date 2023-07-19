# CMP2003-Project
# Movie Recommendation System

This project is a movie recommendation system that uses Collaborative Filtering to predict user preferences. The system is built in C++.

## Overview

The recommendation algorithm operates on user-item rating data, represented as pairs of `<user, item>` and their associated ratings. The system can identify the top users and movies by their watch count.

The recommendation system uses two files: `train.csv` and `test.csv`. The `train.csv` file is used for training the model, while the `test.csv` file is used to evaluate its performance. The final predictions are written to `submission.csv`.

## How to Run

The project can be run by compiling and executing the main.cpp file. Make sure the `train.csv` and `test.csv` files are in the same directory as the main.cpp file.

Please note that the project also includes `createAllList.h` and `ratePredictor.h` header files which should also be in the same directory.

```
g++ main.cpp -o main
./main
```

This will run the recommendation system and write the predictions to `submission.csv`.

## Performance

The system includes performance timing, outputting the time taken to find the top 10 users and movies as well as the total running time of the algorithm.

The average time for it to find Top 10 Users/Movies after creating the necessary data structures is 1.3 milliseconds for 35200 users and 7331 films.

The average time for the algorithm to make 5,000 predictions, given ~1.1 million training data, is 125 seconds.

Performance measurements have been made with 8GB/256GB M1 Macbook Air.

## Dependencies

- C++ Standard Library
- `<chrono>` for performance timing
- `<fstream>` for file input/output
- `<vector>` and `<pair>` for data structure management
- `<iostream>` for console input/output
- `createAllList.h` for creating the necessary data structures
- `ratePredictor.h` for making predictions with processed data

## Detailed Description

Please refer to the code comments in `main.cpp` for a more detailed explanation of the system's workings.
