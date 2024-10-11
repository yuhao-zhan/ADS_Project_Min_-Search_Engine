# ADS_Project_Mini_Search_Engine
This is our first ads project: ==Mini Search Engine==.

For the `documents` directory, there are totally three items: README.md, report.pdf and a folder `results`. 

For the  folder(.zip file) `results`, there are several things needing your attention:

1. One subdirectory is `results(with_stemming)`, which stores all the results tested in the situation of **applying** `stemming` function.
2. The other subdirectory is `ablation_results(without_stemming)`, which contains all the results tested in the situation of **without** applying `stemming` function.

And there are 2 subdirectories both in directory `results(with_stemming)` and  `ablation_results(without_stemming)`, which are respectively `csv` and `visualization`. 

*  `csv` stores all the results in the "test" part. And there are totally **42** .csv files in it for different query types and thresholds.
* `visualization` contains all the visulized results of the `.csv` files with bar charts form. And there are totally **84** `.png` images in its, 2 times as the number of `.csv` files due to the split of single-word queries and two-word queries.
