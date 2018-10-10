# Usage

## Compile WordCount.java and create a jar:

$> hadoop com.sun.tools.javac.Main WordCount.java
$> jar cf wc.jar WordCount*.class


## Assuming that:

    /user/{user_id}/input - input directory in HDFS
    /user/{user_id}/output - output directory in HDFS

## Sample text-files as input:

$> hadoop fs -ls /user/joe/wordcount/input/
    /user/{user_id}/input/file01
    /user/{user_id}/input/file02

$> hadoop fs -cat /user/{user_id}/input/file01
Hello World Bye World

$> bin/hadoop fs -cat /user/{user_id}/input/file02
Hello Hadoop Goodbye Hadoop

## Run the application:
$> hadoop jar wc.jar WordCount /user/{user_id}/input /user/{user_id}/output

## Output:

$> hadoop fs -cat /user/{user_id}/output/part-r-00000
Bye 1
Goodbye 1
Hadoop 2
Hello 2
World 2

# For more information, refer to MapReduce Tutorial as follow:
https://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html

