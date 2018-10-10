#!/bin/bash
hadoop com.sun.tools.javac.Main WordCount.java
jar cf wc.jar *.class

